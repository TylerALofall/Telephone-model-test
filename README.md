# Telephone-model-test
Learn how instructions change as they pass between models.

## Opus telephone game

The **Opus telephone round** Actions workflow runs two separate Copilot CLI
sessions using `claude-opus-4.8`:

1. The director turns your request into a build assignment.
2. The builder receives that response verbatim and implements its
   interpretation in this repository.

The builder can edit this checkout and run local scripts, including scripts
that generate SVG files. Built-in GitHub tools, remote session export, common
network commands, `git push`, and persisted checkout credentials are disabled
during model execution. Only the final workflow step can push, and only when
you explicitly select **publish_changes**. Published work goes to
`telephone-round/<run-id>` in this repository; nothing opens a pull request or
deploys elsewhere.

Runaway work is bounded: the director has 10 minutes, the builder has 25
minutes and at most five automatic continuations, and the entire job has a
45-minute timeout.

### Setup

Create a fine-grained personal access token with only the **Copilot Requests**
permission and save it as the repository Actions secret
`COPILOT_GITHUB_TOKEN`. Do not give that token repository write permissions.

From the repository's **Actions** tab, choose **Opus telephone round**, select
**Run workflow**, enter the initial request, and decide whether the resulting
changes should be pushed to a run-specific branch.

### Transparent round record

Every run uploads a `telephone-round-<run-id>` artifact containing:

- the exact director input and output;
- the exact builder input and terminal output;
- Copilot's shared Markdown session transcripts;
- stderr and exit codes from both sessions;
- Git status, a change summary, and a binary-capable patch; and
- a list and browsable copies of SVG files present after the build.

Artifacts are retained for 30 days. They can contain the complete prompt,
model responses, commands, and generated output, so review them before sharing
their contents elsewhere.
