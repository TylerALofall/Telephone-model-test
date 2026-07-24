#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
RESULTS="$ROOT/telephone-results"
MODEL="${COPILOT_MODEL:-claude-opus-4.8}"

if [[ -z "${TELEPHONE_REQUEST:-}" ]]; then
  echo "TELEPHONE_REQUEST is required" >&2
  exit 2
fi

rm -rf "$RESULTS"
mkdir -p "$RESULTS/director-sandbox" "$RESULTS/transcripts"

{
  cat "$ROOT/prompts/director.md"
  printf '%s\n' "$TELEPHONE_REQUEST"
} > "$RESULTS/01-director-input.md"

common_args=(
  --model "$MODEL"
  --allow-all-tools
  --disable-builtin-mcps
  --disallow-temp-dir
  --no-remote
  --no-remote-export
  --no-custom-instructions
  --no-ask-user
  --no-color
  --no-auto-update
  --secret-env-vars=GH_TOKEN,GITHUB_TOKEN,COPILOT_GITHUB_TOKEN
)

set +e
(
  cd "$RESULTS/director-sandbox"
  timeout --signal=TERM --kill-after=30s 10m \
    copilot -p "$(cat "$RESULTS/01-director-input.md")" \
    "${common_args[@]}" \
    --silent \
    --share "$RESULTS/transcripts/01-director-session.md"
) > "$RESULTS/02-director-output.md" 2> "$RESULTS/02-director-stderr.txt"
director_status=$?
set -e
printf '%s\n' "$director_status" > "$RESULTS/02-director-exit-code.txt"

if (( director_status != 0 )); then
  echo "Director session failed with exit code $director_status" >&2
  exit "$director_status"
fi

{
  cat "$ROOT/prompts/builder.md"
  cat "$RESULTS/02-director-output.md"
} > "$RESULTS/03-builder-input.md"

set +e
(
  cd "$ROOT"
  timeout --signal=TERM --kill-after=30s 25m \
    copilot -p "$(cat "$RESULTS/03-builder-input.md")" \
    "${common_args[@]}" \
    --autopilot \
    --max-autopilot-continues 5 \
    --deny-tool='shell(git push)' \
    --deny-tool='shell(gh:*)' \
    --deny-tool='shell(curl)' \
    --deny-tool='shell(wget)' \
    --deny-tool='shell(ssh)' \
    --share "$RESULTS/transcripts/02-builder-session.md"
) > "$RESULTS/04-builder-output.txt" 2> "$RESULTS/04-builder-stderr.txt"
builder_status=$?
set -e
printf '%s\n' "$builder_status" > "$RESULTS/04-builder-exit-code.txt"

git -C "$ROOT" add -A -- .
git -C "$ROOT" reset --quiet -- telephone-results
git -C "$ROOT" status --short > "$RESULTS/05-git-status.txt"
git -C "$ROOT" diff --cached --stat > "$RESULTS/06-change-stat.txt"
git -C "$ROOT" diff --cached --binary > "$RESULTS/07-changes.patch"

find "$ROOT" -type f -name '*.svg' \
  -not -path "$RESULTS/*" \
  -print | sort > "$RESULTS/08-svg-files.txt"
mkdir -p "$RESULTS/rendered-svg"
(
  cd "$ROOT"
  find . -type f -name '*.svg' \
    -not -path './telephone-results/*' \
    -print0 | xargs -0 -r cp --parents -t "$RESULTS/rendered-svg"
)

cat > "$RESULTS/round-summary.md" <<EOF
# Telephone round

- Model: \`$MODEL\`
- Director exit code: \`$director_status\`
- Builder exit code: \`$builder_status\`
- Workflow run: \`${GITHUB_SERVER_URL:-https://github.com}/${GITHUB_REPOSITORY:-unknown}/actions/runs/${GITHUB_RUN_ID:-unknown}\`

Read the numbered files in order to compare the original request, the
director's interpretation, the builder's received message, its output, and the
resulting repository patch.
EOF

exit "$builder_status"
