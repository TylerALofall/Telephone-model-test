# WHICH SPEC IS AUTHORITATIVE

Follow this file first. It exists because a review found multiple overlapping
specs with no marker saying which one wins, and a build was produced from a
stale one as a result.

## AUTHORITATIVE — follow this
    BUILD_SPEC_v9_AUTHORITATIVE.md

## ARCHIVED HISTORY — do not build from these
    BUILD_REQUEST.md
    BUILD_REQUEST_v2.md
    BUILD_REQUEST_v3.md
    BUILD_SPEC_v4_full.md
    BUILD_SPEC_v5_one_colour.md
    BUILD_SPEC_v6_orange_canonical.md
    BUILD_SPEC_v7_orange.md
    BUILD_SPEC_v8_orange_no_links.md

They are kept because the trail of what failed and why is the point of this
repo. They are not build instructions.

## KNOWN CONTRADICTIONS BETWEEN ARCHIVED SPECS
  - v4/v5/v6/v7 include xmlns; v8 forbids it. v9 keeps it.
  - v3 pastes finished bytes; v5+ state rules instead.
  - BUILD_REQUEST* use artifacts/ ; v5+ use out/03_colors/<colour>/ .
  - v4 asks for all 8 colours in one spec; v9 parameterises colour instead.

## WHAT EACH BUILD PRODUCED
  v5  red     passed, but letterforms were invented, not canonical.
  v7  orange  passed: canonical bitmaps matched cell-for-cell, no root width.
  v4  all 8   472 files and structural identity held, but built before the v7
              fixes: root carries width/height and an XML declaration, and the
              letters do not match the canonical 8x8 grids.
