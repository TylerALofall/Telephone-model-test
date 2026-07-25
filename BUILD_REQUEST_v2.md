# BUILD REQUEST — COLOUR TILES (Red first)
You are Opus 4.8 working alone. Build from this message only. No help follows.

## WHAT TO BUILD
Four SVG files for the colour RED (fill #ED0A3F). Commit to branch
colour-stones-turn-1, folder artifacts/.

Every SVG uses a fixed square canvas: viewBox="0 0 512 512". All four are the
same size so any tile is interchangeable on a grid.

1. artifacts/red_screen.svg
   Solid fill. One rect, full canvas, fill #ED0A3F. Nothing else. No grid.

2. artifacts/red_name.svg
   Solid #ED0A3F background covering the full canvas, then the word "Red"
   centered, large and bold, in WHITE. Use a single <text> element:
   x="256" y="256" text-anchor="middle" dominant-baseline="central"
   font-family="sans-serif" font-weight="bold" font-size="140" fill="white".

3. artifacts/red_ball.svg
   White background rect full canvas. One circle centered: cx="256" cy="256"
   r="180" fill="#ED0A3F".

4. artifacts/red_balls.svg
   White background rect full canvas. Seven circles r="60" fill="#ED0A3F"
   arranged: one center (256,256), six around it at distance 150, at angles
   0,60,120,180,240,300 degrees. Compute each cx,cy and write them out.

## RULES
- No mock, no placeholder. Real final files.
- No workflows, no agents, no scripts. Write the four SVG files directly.
- viewBox is 512x512 on all four. Do not change canvas size between files.
- Do not add a grid, border, or extra shapes not listed.
- If anything here is unclear, STOP and say so. Do not guess.

## REPORT (end with exactly this)
FILES CREATED: <path> <bytes> each
VERIFICATION: which files you opened and confirmed
ASSUMPTIONS MADE: anything not stated that you decided, or "none"
BLOCKED ON: anything incomplete, or "nothing"
