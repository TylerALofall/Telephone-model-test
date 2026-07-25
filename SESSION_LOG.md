# SESSION LOG — colour stones
Persistent record on branch colour-stones-turn-1. Survives context loss.
Read this first on any resume.

## TASKS I GAVE COPILOT
- Issue #3: build 64 colour-stone SVGs (8 crayon colours, 4 file kinds each),
  commit to branch colour-stones-turn-1. Full spec = BUILD_REQUEST.md.
  Status: assigned to Copilot, awaiting its PR/commits.
  Note: this PR contains only spec/log documents. The SVG artifacts have not
  yet been committed; they are the expected output of running the build request.

## MY OWN ERROR — found by rendering, not by being told (though Tyler told me twice)
- In my own reference render (separate from BUILD_REQUEST.md), I used viewBox
  88x88 with 8-unit cells. WRONG for a reference render: 8x8 means 8 pixels,
  and a pixel is a TILE not 1 unit. This does NOT apply to BUILD_REQUEST.md,
  which correctly specifies 88×88 with 8-unit cells and an 8-unit border margin
  (glyph fills 64×64 units within the 88×88 canvas as intended).
- My glyph reference rendered as a smudge because it filled only ~64 units of
  an 88 box, mis-scaled in my external test, not in the spec.
- The R glyph did not read as an R in my test render. Root cause: my frame was
  not scaled to pixel size correctly, not a bug in BUILD_REQUEST.md.
- CORRECTION PENDING: I must get MY reference render correct before judging
  Copilot or changing any repo glyph.

## PREDICTION SCORING (partial, pre-fix)
- Predicted my name-spacing (+72 offset) was weakest. It was actually CORRECT.
- Real defect was the frame/cell scale, which I did not predict. Miss.

## STATE
- PREDICTION_before_run.md  committed (sealed)
- BUILD_REQUEST.md          committed (the prompt as sent)
- This log                  committed
- Next: fix my own reference render, THEN evaluate Copilot output on #3.
