# Telephone Model Test

> *Learn how a message changes — or survives — as it passes through a chain of AI models.*

This repo runs a **telephone-game experiment** on large language models.
One initial message is handed to Model 1. Model 2 receives the original message
**plus** Model 1's full response. Model 3 receives everything Model 2 saw, and
so on. The complete conversation history grows at every step so each model
has a transparent view of what came before.

---

## How it works

```
Initial message
    │
    ▼
┌─────────────┐
│   Model 1   │  sees: original message only
└──────┬──────┘
       │ response
       ▼
┌─────────────┐
│   Model 2   │  sees: original message + Model 1's response
└──────┬──────┘
       │ response
       ▼
┌─────────────┐
│   Model 3   │  sees: original message + Model 1's response + Model 2's response
└──────┬──────┘
       │
      ...
```

All responses are saved to `results/chain_<timestamp>.json` for review.

---

## Files

| File | Purpose |
|------|---------|
| `telephone_chain.js` | Core script — runs the chain, records every step |
| `chain_config.json` | Defines the ordered list of models in the chain |
| `.github/workflows/telephone-chain.yml` | GitHub Actions workflow — trigger via UI or API |
| `results/` | Saved chain transcripts (JSON) |

---

## Running locally

### Prerequisites
- Node.js 18 or later (uses built-in `https` module — no `npm install` needed)
- API keys exported as environment variables (see below)

### Set API keys
```bash
export ANTHROPIC_API_KEY="sk-ant-..."   # for Claude models
export OPENAI_API_KEY="sk-..."          # for GPT models
```

### Run with real models
```bash
node telephone_chain.js --message "The sky is blue"
```

### Run in mock mode (no API keys needed — great for testing the chain logic)
```bash
node telephone_chain.js --message "The sky is blue" --mock
```

### Custom config or output path
```bash
node telephone_chain.js \
  --message "Explain recursion" \
  --config path/to/my_config.json \
  --output results/my_run.json
```

---

## Running via GitHub Actions

1. Go to **Actions → Telephone Chain → Run workflow** in the GitHub UI.
2. Enter your initial message and choose whether to use mock mode.
3. Click **Run workflow**.

The workflow can also be triggered programmatically via the GitHub REST API
(e.g. from Claude.ai or any HTTP client) — replace `YOUR_TOKEN` with a
personal access token that has `repo` scope:

```bash
curl -X POST \
  -H "Authorization: ******" \
  -H "Accept: application/vnd.github+json" \
  https://api.github.com/repos/TylerALofall/Telephone-model-test/actions/workflows/telephone-chain.yml/dispatches \
  -d '{
    "ref": "main",
    "inputs": {
      "message": "The sky is blue",
      "mock": "false"
    }
  }'
```

Results are uploaded as a workflow artifact named **chain-results** and, for
real (non-mock) runs, are also committed back to the `results/` directory.

---

## Configuring the model chain

Edit `chain_config.json` to change which models participate and in what order.

```json
{
  "chain": [
    {
      "id": "model-1",
      "label": "Claude 3.5 Haiku",
      "provider": "anthropic",
      "model": "claude-3-5-haiku-20241022",
      "api_key_env": "ANTHROPIC_API_KEY"
    },
    {
      "id": "model-2",
      "label": "GPT-4o Mini",
      "provider": "openai",
      "model": "gpt-4o-mini",
      "api_key_env": "OPENAI_API_KEY"
    }
  ]
}
```

Supported providers: `anthropic`, `openai`.
Add more entries to the `chain` array to extend the chain.

---

## Adding API keys to GitHub Actions

Go to **Settings → Secrets and variables → Actions** in this repository and add:

| Secret name | Value |
|-------------|-------|
| `ANTHROPIC_API_KEY` | Your Anthropic API key |
| `OPENAI_API_KEY` | Your OpenAI API key |

---

## Result format

Each run produces a JSON file in `results/`:

```json
{
  "started_at": "2026-07-24T17:40:42.172Z",
  "initial_message": "The sky is blue",
  "mock": false,
  "steps": [
    {
      "step": 1,
      "model_label": "Claude 3.5 Haiku",
      "provider": "anthropic",
      "messages_received": 1,
      "response": "The sky appears blue because..."
    },
    {
      "step": 2,
      "model_label": "GPT-4o Mini",
      "provider": "openai",
      "messages_received": 3,
      "response": "Building on what was said above..."
    }
  ],
  "finished_at": "2026-07-24T17:40:45.001Z"
}
```

`messages_received` shows exactly how many messages each model was given —
confirming the growing context window at each step.
