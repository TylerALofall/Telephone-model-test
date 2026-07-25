# BUILD SPEC — SVG Tile Generator, one colour (write in C)

You are Opus 4.8, working alone, from this specification. Produce a C99
program, run it, verify, deliver. Examples show the required RESULT.

This colour: ORANGE, hex #FF8833.

## 1. GOAL
One C99 program, tilegen.c, that writes the complete orange tile set below.
Shakti will hold 45,000+ of these. Keep every file minimal and every tile the
exact same size.

## 2. FILES CONTAIN GEOMETRY ONLY — NO LINKS, NO REFERENCES, NO METADATA
Each SVG contains ONLY shape primitives and their literal attributes. Nothing
in any file may reference, link to, or depend on anything outside that file.

Every file uses this root and only this root:
    <svg viewBox="0 0 512 512">
There is NO xmlns attribute. There is NO width or height attribute.
Note: omitting xmlns="http://www.w3.org/2000/svg" means these files are not
self-contained SVG documents per the XML/SVG spec and may not render in strict
XML parsers or be processed by tools that require a namespace declaration.
This is an intentional tradeoff to satisfy the "no external references" and
minimal-file requirements for Shakti tile storage. Files work correctly in all
major browsers and lenient SVG renderers.

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
viewBox 0 0 512 512 for every file. All tiles identical in size, scaling to
whatever grid cell holds them.

## 4. OUTPUT AND NAMING
All files in:  out/03_colors/orange/
Filename:  03_colors_orange_[group]_[item].svg
  [group] = solid | ABC | abc | ball | balls | shapes
Examples:
    03_colors_orange_solid_screen.svg
    03_colors_orange_ABC_A.svg
    03_colors_orange_abc_a.svg
    03_colors_orange_ball_one.svg
    03_colors_orange_balls_seven.svg
    03_colors_orange_shapes_pentagon.svg

## 5. COLOUR RULE
Hex #FF8833 fills coloured elements. Non-colour backgrounds are white #FFFFFF.

## 6. THE TILES

### 6.1 SOLID SCREEN (solid, screen) — 1 file
    <svg viewBox="0 0 512 512"><rect width="512" height="512" fill="#FF8833"/></svg>

### 6.2 LETTERS (ABC for A-Z, abc for a-z) — 52 files
Item is the literal character. Reproduce the EXACT bitmap from section 11. Do
not design letterforms. Match cell for cell even if rough: the same glyph feeds
the written letter, the sound link, and later drawing, so recognition depends
on an exact match.

Reading section 11 (unambiguous):
  - Each line is one glyph: the character, a TAB, then exactly 64 characters
    of 1 or 0.
  - The 64 characters are the 8x8 grid row-major: first 8 = row 0 (cols 0..7),
    next 8 = row 1, and so on for 8 rows. 1 = lit (emit a rect), 0 = empty.

Render geometry (fixed):
  - Cell 40 by 40. Block 320. Origin (512-320)/2 = 96 for x and y.
  - Lit cell at row r, column c:
        x = 96 + c*40 , y = 96 + r*40 , width 40 , height 40 , fill #FF8833
  - White full-canvas background first, then lit cells row-major.

Required result shape (character A, first two lit cells):
    <svg viewBox="0 0 512 512"><rect width="512" height="512" fill="#FFFFFF"/><rect x="216" y="96" width="40" height="40" fill="#FF8833"/><rect x="256" y="96" width="40" height="40" fill="#FF8833"/> ...rest of A... </svg>

### 6.3 BALL (ball, one) — 1 file
    <svg viewBox="0 0 512 512"><rect width="512" height="512" fill="#FFFFFF"/><circle cx="256" cy="256" r="180" fill="#FF8833"/></svg>

### 6.4 SEVEN BALLS (balls, seven) — 1 file
Circles r=60: center (256,256), six at radius 150 at angles 0,60,120,180,240,300,
x=256+150cos, y=256+150sin, rounded.
    <svg viewBox="0 0 512 512"><rect width="512" height="512" fill="#FFFFFF"/><circle cx="256" cy="256" r="60" fill="#FF8833"/><circle cx="406" cy="256" r="60" fill="#FF8833"/><circle cx="331" cy="386" r="60" fill="#FF8833"/><circle cx="181" cy="386" r="60" fill="#FF8833"/><circle cx="106" cy="256" r="60" fill="#FF8833"/><circle cx="181" cy="126" r="60" fill="#FF8833"/><circle cx="331" cy="126" r="60" fill="#FF8833"/></svg>

### 6.5 SHAPES (shapes) — 4 files: triangle, square, pentagon, hexagon
Centered (256,256), radius-180 bounding circle.
  - triangle: 3 vertices, first -90 (up), then +120, +240.
  - square: rect x=76 y=76 width=360 height=360.
  - pentagon: 5 vertices, first -90, step 72.
  - hexagon: 6 vertices, first -90, step 60.
Polygons use <polygon points="..."> rounded to whole numbers.
    <svg viewBox="0 0 512 512"><rect width="512" height="512" fill="#FFFFFF"/><rect x="76" y="76" width="360" height="360" fill="#FF8833"/></svg>

## 7. FILE COUNT
1 + 26 + 26 + 1 + 1 + 4 = 59. Program prints the count and errors if not 59.

## 8. DELIVERY
Commit tilegen.c, all 59 SVGs under out/03_colors/orange/, one zip of out/, a
short README. Branch: colour-stones-turn-1.

## 9. SELF-CHECK (must all pass)
  - grep every SVG for: xmlns, href, <a, <use, <image, <script, <style,
    <metadata, <title, <desc, url(, data:, <!--, <?xml, <!DOCTYPE
    Every one must return ZERO matches across all 59 files.
  - Confirm character A and lowercase a match section 11 cell for cell.
  - Confirm every root is exactly <svg viewBox="0 0 512 512">.

## 10. REPORT — end with exactly this
    PROGRAM: tilegen.c , <lines> lines
    FILE COUNT: <n>  (must be 59)
    FORBIDDEN-TOKEN SCAN: matches found = <n>  (must be 0)
    ROOT IS EXACTLY <svg viewBox="0 0 512 512">: yes/no
    BITMAP MATCH A and a vs section 11: yes/no
    FILES: <name> <bytes>, one line each
    BUNDLE: <zip path> <bytes>
    ASSUMPTIONS MADE: <or "none">
    BLOCKED ON: <or "nothing">
If two requirements conflict, quote both and stop.

## 11. CANONICAL LETTER BITMAPS (reproduce exactly)
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
