#!/usr/bin/env node
/**
 * telephone_chain.js
 *
 * Telephone Model Test
 * --------------------
 * Passes one initial message through a chain of AI models.
 * Each model in the chain receives:
 *   1. The original message
 *   2. Every previous model's full response (the growing transcript)
 *
 * This makes the whole chain transparent — you can see exactly how each
 * model interpreted and transformed what it was given.
 *
 * Usage:
 *   node telephone_chain.js --message "Your initial message here"
 *   node telephone_chain.js --message "..." --config path/to/chain_config.json
 *   node telephone_chain.js --message "..." --output results/my_run.json
 *   node telephone_chain.js --message "..." --mock   (uses mock responses, no API keys needed)
 *
 * API keys are read from environment variables defined in chain_config.json.
 * Set ANTHROPIC_API_KEY and/or OPENAI_API_KEY before running.
 */

import { readFileSync, writeFileSync, mkdirSync } from "fs";
import { resolve, dirname } from "path";
import { fileURLToPath } from "url";
import https from "https";

// ---------------------------------------------------------------------------
// Argument parsing
// ---------------------------------------------------------------------------
function parseArgs(argv) {
  const args = { message: null, config: null, output: null, mock: false };
  for (let i = 2; i < argv.length; i++) {
    if (argv[i] === "--message" && argv[i + 1]) {
      args.message = argv[++i];
    } else if (argv[i] === "--config" && argv[i + 1]) {
      args.config = argv[++i];
    } else if (argv[i] === "--output" && argv[i + 1]) {
      args.output = argv[++i];
    } else if (argv[i] === "--mock") {
      args.mock = true;
    }
  }
  return args;
}

// ---------------------------------------------------------------------------
// Simple HTTPS POST helper (no external dependencies)
// ---------------------------------------------------------------------------
function httpsPost(url, headers, body) {
  return new Promise((resolve, reject) => {
    const parsed = new URL(url);
    const data = JSON.stringify(body);
    const options = {
      hostname: parsed.hostname,
      path: parsed.pathname + parsed.search,
      method: "POST",
      headers: {
        "Content-Type": "application/json",
        "Content-Length": Buffer.byteLength(data),
        ...headers,
      },
    };
    const req = https.request(options, (res) => {
      let raw = "";
      res.on("data", (chunk) => (raw += chunk));
      res.on("end", () => {
        try {
          resolve({ status: res.statusCode, body: JSON.parse(raw) });
        } catch {
          resolve({ status: res.statusCode, body: raw });
        }
      });
    });
    req.on("error", reject);
    req.write(data);
    req.end();
  });
}

// ---------------------------------------------------------------------------
// Model callers
// ---------------------------------------------------------------------------

/** Call the Anthropic Messages API */
async function callAnthropic(modelCfg, messagesContext) {
  const apiKey = process.env[modelCfg.api_key_env];
  if (!apiKey) {
    throw new Error(
      `Missing API key: set the environment variable "${modelCfg.api_key_env}"`
    );
  }

  const resp = await httpsPost(
    "https://api.anthropic.com/v1/messages",
    {
      "x-api-key": apiKey,
      "anthropic-version": "2023-06-01",
    },
    {
      model: modelCfg.model,
      max_tokens: 1024,
      messages: messagesContext,
    }
  );

  if (resp.status !== 200) {
    throw new Error(
      `Anthropic API error ${resp.status}: ${JSON.stringify(resp.body)}`
    );
  }
  return resp.body.content[0].text;
}

/** Call the OpenAI Chat Completions API */
async function callOpenAI(modelCfg, messagesContext) {
  const apiKey = process.env[modelCfg.api_key_env];
  if (!apiKey) {
    throw new Error(
      `Missing API key: set the environment variable "${modelCfg.api_key_env}"`
    );
  }

  const resp = await httpsPost(
    "https://api.openai.com/v1/chat/completions",
    {
      Authorization: "Bearer " + apiKey,
    },
    {
      model: modelCfg.model,
      max_tokens: 1024,
      messages: messagesContext,
    }
  );

  if (resp.status !== 200) {
    throw new Error(
      `OpenAI API error ${resp.status}: ${JSON.stringify(resp.body)}`
    );
  }
  return resp.body.choices[0].message.content;
}

/** Return a deterministic mock response (no API key required) */
function callMock(modelCfg, messagesContext) {
  const prior = messagesContext.filter((m) => m.role === "assistant").length;
  return Promise.resolve(
    `[MOCK] ${modelCfg.label} received ${messagesContext.length} message(s). ` +
      `This is mock response #${prior + 1} in the chain. ` +
      `In a real run this model would process the full conversation history shown above.`
  );
}

// ---------------------------------------------------------------------------
// Build the messages array for the next model in the chain
// ---------------------------------------------------------------------------
/**
 * Each model receives an OpenAI-style messages array:
 *
 *   user:      The original message
 *   assistant: Model 1's response    (if any)
 *   user:      "What do you make of the above? Please continue the chain."
 *   assistant: Model 2's response    (if any)
 *   user:      "What do you make of the above? ..."
 *   ...
 *
 * The alternating user/assistant pattern is compatible with both the Anthropic
 * and OpenAI message formats.
 */
function buildMessages(originalMessage, steps) {
  const messages = [{ role: "user", content: originalMessage }];

  for (const step of steps) {
    // Previous model's answer
    messages.push({ role: "assistant", content: step.response });
    // Prompt for the next model — always appended so the array ends on a user turn,
    // which is required by both the Anthropic and OpenAI APIs.
    messages.push({
      role: "user",
      content:
        "You are the next model in a telephone chain experiment. " +
        "You can see the original message and every response before yours. " +
        "Please respond to the conversation — add your perspective, correct errors you notice, " +
        "or continue the thought. Be concise.",
    });
  }

  return messages;
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------
async function main() {
  const args = parseArgs(process.argv);

  if (!args.message) {
    console.error(
      'Error: --message is required.\nExample: node telephone_chain.js --message "The sky is blue"'
    );
    process.exit(1);
  }

  // Load config
  const __dirname = dirname(fileURLToPath(import.meta.url));
  const configPath = args.config
    ? resolve(args.config)
    : resolve(__dirname, "chain_config.json");
  const config = JSON.parse(readFileSync(configPath, "utf8"));

  // Determine output path
  const timestamp = new Date().toISOString().replace(/[:.]/g, "-");
  const outputPath = args.output
    ? resolve(args.output)
    : resolve(__dirname, "results", `chain_${timestamp}.json`);

  console.log("=".repeat(60));
  console.log("  Telephone Model Chain");
  console.log("=".repeat(60));
  console.log(`  Initial message : "${args.message}"`);
  console.log(`  Models in chain : ${config.chain.length}`);
  console.log(`  Mock mode       : ${args.mock}`);
  console.log(`  Output file     : ${outputPath}`);
  console.log("=".repeat(60));
  console.log();

  const result = {
    started_at: new Date().toISOString(),
    initial_message: args.message,
    mock: args.mock,
    steps: [],
  };

  for (let i = 0; i < config.chain.length; i++) {
    const modelCfg = config.chain[i];
    const stepNumber = i + 1;

    console.log(`--- Step ${stepNumber}/${config.chain.length}: ${modelCfg.label} ---`);

    // Build the full conversation history for this model
    const messages = buildMessages(args.message, result.steps);

    console.log(
      `  Sending ${messages.length} message(s) to ${modelCfg.label}...`
    );

    let response;
    let stepError = null;
    try {
      if (args.mock) {
        response = await callMock(modelCfg, messages);
      } else if (modelCfg.provider === "anthropic") {
        response = await callAnthropic(modelCfg, messages);
      } else if (modelCfg.provider === "openai") {
        response = await callOpenAI(modelCfg, messages);
      } else {
        throw new Error(`Unknown provider: "${modelCfg.provider}"`);
      }
    } catch (err) {
      // Log only a static marker to avoid printing any API-derived data to
      // stderr. The full error message is stored in the result JSON below.
      console.error(`  ERROR at step ${stepNumber} — call to ${modelCfg.label} failed`);
      stepError = err;
      response = `[ERROR] Step ${stepNumber} failed — see the "error" field in the result JSON.`;
    }

    const step = {
      step: stepNumber,
      model_id: modelCfg.id,
      model_label: modelCfg.label,
      provider: modelCfg.provider,
      model: modelCfg.model,
      messages_received: messages.length,
      response,
      ...(stepError ? { error: stepError.message } : {}),
    };

    result.steps.push(step);

    // Print the response to stdout so it is visible in CI logs
    console.log(`  Response:\n`);
    response.split("\n").forEach((line) => console.log(`    ${line}`));
    console.log();
  }

  result.finished_at = new Date().toISOString();

  // Save results
  mkdirSync(dirname(outputPath), { recursive: true });
  writeFileSync(outputPath, JSON.stringify(result, null, 2), "utf8");
  console.log("=".repeat(60));
  console.log(`  Chain complete. Results saved to:\n  ${outputPath}`);
  console.log("=".repeat(60));

  // Also print a clean transcript to stdout
  printTranscript(result);
}

function printTranscript(result) {
  console.log();
  console.log("=".repeat(60));
  console.log("  FULL CHAIN TRANSCRIPT");
  console.log("=".repeat(60));
  console.log();
  console.log(`Original message:\n  "${result.initial_message}"`);
  console.log();
  for (const step of result.steps) {
    console.log(`[${step.step}] ${step.model_label}`);
    console.log(`    Saw: ${step.messages_received} message(s) in context`);
    console.log(`    Said:`);
    step.response.split("\n").forEach((line) => console.log(`      ${line}`));
    console.log();
  }
}

main().catch((err) => {
  console.error("Fatal error:", err);
  process.exit(1);
});
