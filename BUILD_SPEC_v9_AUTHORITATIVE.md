# BUILD SPEC v9 — AUTHORITATIVE
# SVG Tile Generator (write in C)
#
# This file supersedes BUILD_REQUEST*.md and BUILD_SPEC_v4..v8.
# Those are archived history. Follow ONLY this file.

You are Opus 4.8, working alone, from this specification. Produce a C99
program, run it, verify, deliver. Examples show the required RESULT.

Colour is a runtime argument, not a source edit. Invoke:

    ./tilegen <colour-word> <hex>

Run it once per colour listed in section 11. The same binary produces every
colour set, so the sets cannot drift apart.

## 1. GOAL
One C99 program, tilegen.c, that writes a complete tile set for whichever
colour it is invoked with.
Shakti will hold 45,000+ of these. Keep every file minimal and every tile the
exact same size.

## 2. FILES CONTAIN GEOMETRY ONLY — NO LINKS, NO REFERENCES, NO METADATA
Each SVG contains ONLY shape primitives and their literal attributes. Nothing
in any file may reference, link to, or depend on anything outside that file.

Every file uses this root and only this root:
    <svg viewBox="0 0 8 8">
There is NO xmlns attribute and NO width or height attribute. A pixel width
forces a fixed render size and breaks uniform scaling in a grid.

TRADEOFF, DECIDED AND INTENTIONAL: omitting xmlns makes these files
non-conformant as standalone SVG documents under strict XML/SVG parsers. Most
renderers are lenient; some are not, and a browser opening one directly may
refuse it. That cost is accepted. These are internal training artifacts parsed
as geometry, not web assets. No external URI belongs inside them, including a
namespace identifier. Do not add xmlns back to improve interoperability.

The following are forbidden in every file. A file containing any of these is
wrong and must be rebuilt:
    xmlns, xmlns:xlink, xlink:href, href
    <a>, <use>, <image>, <script>, <style>, <metadata>, <title>, <desc>
    external CSS, @import, url( ... ), data: URIs
    XML comments, processing instructions, DOCTYPE, <?xml ?> declaration

Allowed elements: only <svg>, <rect>, <circle>, <polygon>.
Allowed attributes: only viewBox, x, y, width, height, cx, cy, r, points, fill.
Nothing else appears in any file.

## 3. CANVAS
viewBox 0 0 8 8 for every file. All tiles identical in size, scaling to
whatever grid cell holds them.

The canvas is 8 units because the bitmap is 8x8. One unit is one grid cell, so
a coordinate IS a grid position: x="3" y="2" is column 3, row 2. No offset and
no scale factor sits between the file and the grid, so nothing can drift.
SVG is vector: an 8-unit tile renders sharp at any display size.

## 4. OUTPUT AND NAMING
All files in:  out/03_colors/<colour>/
Filename:  03_colors_<colour>_[group]_[item].svg
  [group] = solid | ABC | abc | ball | balls | shapes
Examples:
    03_colors_<colour>_solid_screen.svg
    03_colors_<colour>_ABC_A.svg
    03_colors_<colour>_abc_a.svg
    03_colors_<colour>_ball_one.svg
    03_colors_<colour>_balls_seven.svg
    03_colors_<colour>_shapes_pentagon.svg

## 5. COLOUR RULE
Hex <hex> fills coloured elements. Non-colour backgrounds are white #FFFFFF.

## 6. THE TILES

### 6.1 SOLID SCREEN (solid, screen) — 1 file
    <svg viewBox="0 0 8 8"><rect width="8" height="8" fill="<hex>"/></svg>

### 6.2 LETTERS (ABC for A-Z, abc for a-z) — 52 files
Item is the literal character. Reproduce the EXACT bitmap from section 12. Do
not design letterforms. Match cell for cell even if rough: the same glyph feeds
the written letter, the sound link, and later drawing, so recognition depends
on an exact match.

Reading section 12 (unambiguous):
  - Each line is one glyph: the character, a TAB, then exactly 64 characters
    of 1 or 0.
  - The 64 characters are the 8x8 grid row-major: first 8 = row 0 (cols 0..7),
    next 8 = row 1, and so on for 8 rows. 1 = lit (emit a rect), 0 = empty.

Render geometry (fixed):
  - Cell 1 by 1. Eight cells span the 8-unit canvas exactly.
  - Lit cell at row r, column c:
        x = c , y = r , width 1 , height 1 , fill <hex>
    The coordinate is the grid position. No origin offset, no multiplier.
  - White full-canvas background first, then lit cells row-major.

Required result shape (character A, first two lit cells):
    <svg viewBox="0 0 8 8"><rect width="8" height="8" fill="#FFFFFF"/><rect x="2" y="0" width="1" height="1" fill="<hex>"/><rect x="3" y="0" width="1" height="1" fill="<hex>"/> ...rest of A... </svg>

### 6.3 BALL (ball, one) — 1 file
    <svg viewBox="0 0 8 8"><rect width="8" height="8" fill="#FFFFFF"/><circle cx="4" cy="4" r="2.8" fill="<hex>"/></svg>

### 6.4 SEVEN BALLS (balls, seven) — 1 file
Circles r=0.94: centre (4,4), six at ring radius 2.34 from centre at angles
0,60,120,180,240,300, x=4+2.34cos, y=4+2.34sin, rounded to 2 decimals.
    <svg viewBox="0 0 8 8"><rect width="8" height="8" fill="#FFFFFF"/><circle cx="4" cy="4" r="0.94" fill="<hex>"/><circle cx="6.34" cy="4" r="0.94" fill="<hex>"/><circle cx="5.17" cy="6.03" r="0.94" fill="<hex>"/><circle cx="2.83" cy="6.03" r="0.94" fill="<hex>"/><circle cx="1.66" cy="4" r="0.94" fill="<hex>"/><circle cx="2.83" cy="1.97" r="0.94" fill="<hex>"/><circle cx="5.17" cy="1.97" r="0.94" fill="<hex>"/></svg>

### 6.5 SHAPES (shapes) — 4 files: triangle, square, pentagon, hexagon
Centred (4,4) on a radius-2.8 bounding circle, so all four read at comparable
size. Vertices rounded to 2 decimals.
  - triangle: <polygon points="4,1.2 6.42,5.4 1.58,5.4" fill="<hex>"/>
  - square:   <rect x="1.2" y="1.2" width="5.6" height="5.6" fill="<hex>"/>
  - pentagon: <polygon points="4,1.2 6.66,3.13 5.65,6.27 2.35,6.27 1.34,3.13" fill="<hex>"/>
  - hexagon:  <polygon points="4,1.2 6.42,2.6 6.42,5.4 4,6.8 1.58,5.4 1.58,2.6" fill="<hex>"/>
White background rect first, then the shape.
    <svg viewBox="0 0 8 8"><rect width="8" height="8" fill="#FFFFFF"/><rect x="1.2" y="1.2" width="5.6" height="5.6" fill="<hex>"/></svg>

## 7. FILE COUNT
1 + 26 + 26 + 1 + 1 + 4 = 59. Program prints the count and errors if not 59.

## 8. DELIVERY
Commit tilegen.c, all 472 SVGs (59 per colour x 8 colours) under
out/03_colors/<colour>/, one zip of out/, and a short README stating how to
invoke tilegen and the per-colour and total file counts.

## 9. SELF-CHECK (must all pass)
  - grep every SVG for: xmlns, href, <a, <use, <image, <script, <style,
    <metadata, <title, <desc, url(, data:, <!--, <?xml, <!DOCTYPE
    Every one must return ZERO matches across all 59 files.
  - Confirm character A and lowercase a match section 12 cell for cell.
  - Confirm every root is exactly <svg viewBox="0 0 8 8">.

## 10. REPORT — end with exactly this
    PROGRAM: tilegen.c , <lines> lines
    FILE COUNT PER COLOUR: <n>  (must be 59)
    TOTAL FILES: <n>            (must be 472)
    FORBIDDEN-TOKEN SCAN: matches found = <n>  (must be 0)
    ROOT IS EXACTLY <svg viewBox="0 0 8 8">: yes/no
    BITMAP MATCH A and a vs section 12: yes/no
    FILES: <name> <bytes>, one line each
    BUNDLE: <zip path> <bytes>
    ASSUMPTIONS MADE: <or "none">
    BLOCKED ON: <or "nothing">
If two requirements conflict, quote both and stop.

## 11. COLOURS TO RUN

    red #ED0A3F   orange #FF8833   yellow #FBE870   green #01A638
    blue #0066FF  violet #8359A3   brown #AF593E    black #000000

Total output: 8 colours x 59 files = 472.

## 12. CANONICAL LETTER BITMAPS (reproduce exactly)
One glyph per line: CHARACTER, TAB, 64 chars (1=lit, 0=empty), row-major.

A	0011100001000100010001000111110001000100010001000100010000000000
B	0111100001000100010001000111100001000100010001000111100000000000
C	0011110001000000010000000100000001000000010000000011110000000000
D	0111100001000100010001000100010001000100010001000111100000000000
E	0111110001000000010000000111100001000000010000000111110000000000
F	0111110001000000010000000111100001000000010000000100000000000000
G	0011110001000000010000000101110001000100010001000011110000000000
H	0100010001000100010001000111110001000100010001000100010000000000
I	0111110000010000000100000001000000010000000100000111110000000000
J	0001110000001000000010000000100001001000010010000011000000000000
K	0100010001001000010100000110000001010000010010000100010000000000
L	0100000001000000010000000100000001000000010000000111110000000000
M	0100010001101100010101000101010001000100010001000100010000000000
N	0100010001100100010101000100110001000100010001000100010000000000
O	0011100001000100010001000100010001000100010001000011100000000000
P	0111100001000100010001000111100001000000010000000100000000000000
Q	0011100001000100010001000100010001010100010010000011010000000000
R	0111100001000100010001000111100001010000010010000100010000000000
S	0011110001000000010000000011100000000100000001000111100000000000
T	0111110000010000000100000001000000010000000100000001000000000000
U	0100010001000100010001000100010001000100010001000011100000000000
V	0100010001000100010001000100010001000100001010000001000000000000
W	0100010001000100010001000101010001010100010101000010100000000000
X	0100010001000100001010000001000000101000010001000100010000000000
Y	0100010001000100001010000001000000010000000100000001000000000000
Z	0111110000000100000010000001000000100000010000000111110000000000
a	0000000000000000001110000000010000111100010001000011110000000000
b	0100000001000000010110000110010001000100010001000111100000000000
c	0000000000000000001111000100000001000000010000000011110000000000
d	0000010000000100001101000100110001000100010001000011110000000000
e	0000000000000000001110000100010001111100010000000011110000000000
f	0001100000100100001000000111000000100000001000000010000000000000
g	0000000000111100010001000100010000111100000001000111100000000000
h	0100000001000000010110000110010001000100010001000100010000000000
i	0001000000000000001100000001000000010000000100000011100000000000
j	0000100000000000000110000000100000001000010010000011000000000000
k	0100000001000000010010000101000001100000010100000100100000000000
l	0011000000010000000100000001000000010000000100000011100000000000
m	0000000000000000011010000101010001010100010101000101010000000000
n	0000000000000000010110000110010001000100010001000100010000000000
o	0000000000000000001110000100010001000100010001000011100000000000
p	0000000001111000010001000100010001111000010000000100000000000000
q	0000000000111100010001000100010000111100000001000000010000000000
r	0000000000000000010110000110010001000000010000000100000000000000
s	0000000000000000001111000100000000111000000001000111100000000000
t	0010000000100000011100000010000000100000001001000001100000000000
u	0000000000000000010001000100010001000100010011000011010000000000
v	0000000000000000010001000100010001000100001010000001000000000000
w	0000000000000000010001000100010001010100010101000010100000000000
x	0000000000000000010001000010100000010000001010000100010000000000
y	0000000001000100010001000100010000111100000001000111100000000000
z	0000000000000000011111000000100000010000001000000111110000000000
