# SEALED PREDICTION — before the model runs
Committed before Copilot receives BUILD_REQUEST_COLOUR_STONES.md.
Author: Claude (Opus 4.8). Purpose: grade my own prompt by what the model builds.

## What I predict the model will get WRONG from my prompt

1. NAME-FILE X-OFFSET. My prompt states "glyph 1 uses x-base 80, glyph 2 uses
   152" but the worked example only shows a SINGLE glyph. I never wrote out a
   multi-glyph name file in full. Prediction: the model mis-places glyphs in
   the _name_text files - either wrong advance, or forgets the +72 accumulates.
   This is the weakest part of my prompt.

2. LOWERCASE FILENAME vs CAPITALIZED WORD. Swatch is "red.svg" (lowercase) but
   the word rendered inside is "Red" (cap R). My table shows Word.svg -> red.svg
   in one line. Prediction: some files get capitalized names (Red.svg) or the
   glyph sequence gets lowercased (r not R).

3. INK COLOUR ON LIGHT FILLS. Yellow #FBE870 as ink on white is nearly
   invisible. I did not address contrast. Prediction: model either flags it
   (good) or produces unreadable yellow-on-white and reports success (the
   failure I want to catch).

4. FILE COUNT SELF-CHECK. I demand "confirm 64 before reporting done."
   Prediction: model reports a number but does NOT actually recount; or builds
   fewer (skips a letter file) and still claims 64.

5. GLYPH FIDELITY. 22 bitmaps given inline. Prediction: at least one glyph gets
   a transcription error (a cell shifted) because the model reads the ASCII art
   loosely rather than cell-by-cell.

## What I predict it gets RIGHT
- Swatch and dot files (simplest, fully worked).
- The red_letter_R.svg file (I worked it out completely).
- Total intent - it will understand the 4 file kinds.

## The real test
If files are wrong, my PROMPT is wrong. The gap between what I meant and what I
wrote is the lesson. I do not get to say the model failed.
