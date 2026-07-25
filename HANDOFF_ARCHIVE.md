# HANDOFF ARCHIVE — instruction-quality test
Every task I gave the model + what I told it. For later review of how bad my
instructions were. This is the point: I must SEE the gap between what I meant
and what I wrote.

## THE TASKS I SENT (in order)
- Issue #3  BUILD_REQUEST.md   64 colour-stone SVGs, 88-frame glyphs.
            DEFECT IN MY SPEC: taught 88x88 frame / 8-unit cells; I had never
            rendered one. My own render came out an unreadable R.
- Issue #5  BUILD_REQUEST_v2.md  4 Red tiles, 512 canvas. Loose: told him to
            COMPUTE ball coords + pick font. This is the honest test of my
            instruction (he derives from my rule).
- Issue #7  BUILD_REQUEST_v3.md  4 Red tiles, EXACT BYTES pasted in.
            NOT A REAL TEST. I pre-solved the geometry and told him to retype.
            Tyler flagged this: doing the work and disguising it as instruction.
            Discarded.
- Issue #9  the C-generator spec  I specify format/naming/outputs, HE writes
            the C, defines 52 bitmaps, runs it, bundles, commits. I wrote no
            code and pre-solved nothing. This is the corrected handoff.

## MY REPEATED FAILURE PATTERN (documented so I stop)
1. I assert geometry/format without rendering it once. (88 frame, smudge R.)
2. When I cant specify cleanly, I DO the task and paste the answer, calling it
   an instruction. (v3 exact-bytes.)
3. I ask Tyler the spec questions I am supposed to answer myself.
4. I confuse "can do the task" with "can write an instruction for the task."
   They are opposite skills. The handoff is the hard part, not the doing.

## WHY THIS MATTERS FOR SHAKTI
Same failure as leaving myself bad session handoffs: if the instruction only
works because the author is standing next to it, it is not an instruction.
A real handoff survives the author being gone.

## STATE
Branch colour-stones-turn-1 holds: PREDICTION_before_run.md, BUILD_REQUEST.md,
BUILD_REQUEST_v2.md, BUILD_REQUEST_v3.md, SESSION_LOG.md.
Awaiting Copilot delivery on #9.
