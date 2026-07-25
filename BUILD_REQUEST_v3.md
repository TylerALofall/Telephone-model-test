# BUILD REQUEST — COLOUR TILES (Red)
You are Opus 4.8 working alone. Build from this message only.

This spec gives you the EXACT file contents. You do not design anything. You
copy the four blocks below verbatim into four files. The only thing you ever
change across colours is two tokens: the hex value and the colour word. For
RED they are already filled in. Copy exactly. Do not reformat, reorder, round,
recompute, or add anything.

## FILE 1 — artifacts/red_screen.svg
Copy these exact bytes:
```
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 512 512" role="img"><rect x="0" y="0" width="512" height="512" fill="#ED0A3F"/></svg>
```

## FILE 2 — artifacts/red_name.svg
Copy these exact bytes:
```
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 512 512" role="img"><rect x="0" y="0" width="512" height="512" fill="#ED0A3F"/><text x="256" y="256" text-anchor="middle" dominant-baseline="central" font-family="sans-serif" font-weight="bold" font-size="140" fill="#FFFFFF">Red</text></svg>
```

## FILE 3 — artifacts/red_ball.svg
Copy these exact bytes:
```
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 512 512" role="img"><rect x="0" y="0" width="512" height="512" fill="#FFFFFF"/><circle cx="256" cy="256" r="180" fill="#ED0A3F"/></svg>
```

## FILE 4 — artifacts/red_balls.svg
Copy these exact bytes:
```
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 512 512" role="img"><rect x="0" y="0" width="512" height="512" fill="#FFFFFF"/><circle cx="256" cy="256" r="60" fill="#ED0A3F"/><circle cx="406" cy="256" r="60" fill="#ED0A3F"/><circle cx="331" cy="386" r="60" fill="#ED0A3F"/><circle cx="181" cy="386" r="60" fill="#ED0A3F"/><circle cx="106" cy="256" r="60" fill="#ED0A3F"/><circle cx="181" cy="126" r="60" fill="#ED0A3F"/><circle cx="331" cy="126" r="60" fill="#ED0A3F"/></svg>
```

## RULES
- Copy the bytes inside each fenced block EXACTLY into the named file.
- Do not add newlines, indentation, comments, XML declarations, or attributes.
- Do not pretty-print. One line per file, as shown.
- Four files total, all in artifacts/, committed to branch colour-stones-turn-1.
- No workflows, no scripts, no agents. Write the files directly.
- If any block is unclear, STOP and quote the exact line you cannot follow.

## REPORT (end with exactly this)
FILES CREATED: <path> <bytes>
BYTE MATCH: for each file, does its content exactly equal the block above? yes/no
ASSUMPTIONS MADE: anything you changed or decided, or "none"
BLOCKED ON: or "nothing"
