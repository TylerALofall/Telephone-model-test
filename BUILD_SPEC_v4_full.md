# BUILD SPEC — Deterministic SVG Tile Generator (write in C)

You are Opus 4.8, working alone, building from this specification. This spec is
complete: it defines every output, every filename, every dimension, and every
invariant. Produce a C99 program that writes the files, run it, verify the
output, and deliver it. You author the program yourself. Every example here
shows the required RESULT so you can check your own work against it.

## 1. GOAL

Produce one C99 program, `tilegen.c`, that generates a complete set of SVG
tiles for ONE colour per run, following the exact rules below. The same program,
run with a different colour argument, produces a byte-identical set differing
only in the colour word, the colour hex, and nothing else. Eight colours will
be generated this way, and all eight sets must be structurally identical.

## 2. INVARIANT (this is the heart of the spec)

Across all eight colours, the ONLY things that change between one colour set and
another are:
  - the colour word in filenames  (red, orange, yellow, green, blue, violet, brown, black)
  - the colour hex in fill values (given in section 8)

Everything else is fixed and identical: canvas size, geometry, coordinates,
radii, font, letter bitmaps, file count, naming pattern, element order, and
byte structure. If two colour sets differ in anything other than word and hex,
the program is wrong.

## 3. CANVAS

Every SVG uses exactly this opening and viewBox:

    <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 512 512" role="img">

Canvas is 512 by 512 for every single file. No exceptions, no per-file sizing.
Tiles must be interchangeable in any grid cell, so all are the same square.

## 4. OUTPUT DIRECTORY AND NAMING

All files go in:  out/03_colors/[colour]/

Naming pattern, every file:

    03_colors_[colour]_[group]_[item].svg

  [colour] = colour word, lowercase          e.g. red
  [group]  = solid | abc | ABC | ball | balls | shapes
  [item]   = see per-group rules in section 6

Grouping matters: with thousands of these in one directory, the [group] token
must clump related tiles together alphabetically. Uppercase ABC sorts before
lowercase abc, which keeps capitals and lowercase in separate visible blocks.

Filename examples for red:

    03_colors_red_solid_screen.svg
    03_colors_red_ABC_A.svg
    03_colors_red_ABC_Z.svg
    03_colors_red_abc_a.svg
    03_colors_red_abc_z.svg
    03_colors_red_ball_one.svg
    03_colors_red_balls_seven.svg
    03_colors_red_shapes_triangle.svg
    03_colors_red_shapes_square.svg
    03_colors_red_shapes_pentagon.svg
    03_colors_red_shapes_hexagon.svg

## 5. COLOUR ELEMENT RULE

Wherever a tile shows the colour, use the colour hex as the fill. Backgrounds
that are not the colour are white (#FFFFFF). Letters and shapes render IN the
colour on a white background. The solid screen is entirely the colour.

## 6. THE TILES

### 6.1 SOLID SCREEN  — 1 file
Group: solid   Item: screen
One rectangle covering the whole canvas, filled with the colour.

Required result (example shown for red, #ED0A3F):

    <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 512 512" role="img"><rect x="0" y="0" width="512" height="512" fill="#ED0A3F"/></svg>

### 6.2 LETTERS  — 52 files
Group: ABC for uppercase A-Z (26 files), abc for lowercase a-z (26 files)
Item:  the single character, e.g. A, B, ... Z, a, b, ... z

Each letter is drawn as an 8x8 bitmap: 8 columns by 8 rows of square cells.
Each lit cell is one <rect>. The letter renders in the colour on white.

Cell geometry, fixed for every letter:
  - The 8x8 grid is centered on the 512 canvas.
  - Each cell is 40 by 40 units. The glyph block is 8*40 = 320 units.
  - Left/top origin of the grid: (512 - 320) / 2 = 96.
  - For grid row r (0..7) and column c (0..7), a lit cell is:
        x = 96 + c*40 , y = 96 + r*40 , width=40 , height=40 , fill=colour hex
  - Emit the white full-canvas background rect first, then one rect per lit cell,
    in row-major order (row 0 left to right, then row 1, and so on).

You must define the 8x8 bitmap for all 52 characters yourself, as data in the C
program (an 8-row pattern of on/off per character). The letters must be readable
as their character. Uppercase and lowercase are distinct designs, not scaled
copies.

Required result shape (example, uppercase I on red, showing the pattern idea
only — you supply real bitmaps for all 52):

    <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 512 512" role="img"><rect x="0" y="0" width="512" height="512" fill="#FFFFFF"/><rect x="96" y="96" width="40" height="40" fill="#ED0A3F"/> ...one rect per lit cell... </svg>

Verify each letter by rendering it and confirming a human can read the character.

### 6.3 BALL  — 1 file
Group: ball   Item: one
White background, one circle centered, colour fill, large and smooth.

Required result (example for red):

    <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 512 512" role="img"><rect x="0" y="0" width="512" height="512" fill="#FFFFFF"/><circle cx="256" cy="256" r="180" fill="#ED0A3F"/></svg>

### 6.4 SEVEN BALLS  — 1 file
Group: balls   Item: seven
White background, seven circles: one centered, six evenly spaced around it.
Fixed geometry: each circle r=60. Center circle at (256,256). The six outer
circles are at radius 150 from center, at angles 0, 60, 120, 180, 240, 300
degrees, x = 256 + 150*cos(angle), y = 256 + 150*sin(angle). Round to whole
numbers. Emit the white background, then center circle, then the six in angle
order.

Required result (example for red — these exact coordinates):

    <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 512 512" role="img"><rect x="0" y="0" width="512" height="512" fill="#FFFFFF"/><circle cx="256" cy="256" r="60" fill="#ED0A3F"/><circle cx="406" cy="256" r="60" fill="#ED0A3F"/><circle cx="331" cy="386" r="60" fill="#ED0A3F"/><circle cx="181" cy="386" r="60" fill="#ED0A3F"/><circle cx="106" cy="256" r="60" fill="#ED0A3F"/><circle cx="181" cy="126" r="60" fill="#ED0A3F"/><circle cx="331" cy="126" r="60" fill="#ED0A3F"/></svg>

### 6.5 SHAPES  — 4 files
Group: shapes   Item: triangle | square | pentagon | hexagon
White background, one shape centered, colour fill. All four sized to a bounding
circle of radius 180 around center (256,256), so they read at comparable size.

  - triangle: equilateral, 3 vertices on the radius-180 circle starting at angle
    -90 degrees (point up), then +120, +240.
  - square: use a <rect> 360 by 360 centered, i.e. x=76 y=76 width=360 height=360.
  - pentagon: 5 vertices on the radius-180 circle starting at -90 degrees, step 72.
  - hexagon: 6 vertices on the radius-180 circle starting at -90 degrees, step 60.

For polygons use a <polygon points="..."> element with vertices rounded to whole
numbers. Emit white background first, then the shape.

Required result shape (example, square on red):

    <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 512 512" role="img"><rect x="0" y="0" width="512" height="512" fill="#FFFFFF"/><rect x="76" y="76" width="360" height="360" fill="#ED0A3F"/></svg>

## 7. FILE COUNT

Per colour, exactly:
    1  solid screen
   26  uppercase letters
   26  lowercase letters
    1  ball
    1  seven balls
    4  shapes
   --
   59  files

The program prints the count and stops with an error if it is not 59.

## 8. THE EIGHT COLOURS

Run the program once per colour. Colour word and hex:

    red     #ED0A3F
    orange  #FF8833
    yellow  #FBE870
    green   #01A638
    blue    #0066FF
    violet  #8359A3
    brown   #AF593E
    black   #000000

For this delivery, generate ALL EIGHT. 59 files each, 472 total. Confirm every
set is structurally identical by comparing two sets and showing the only
differences are the word and the hex.

## 9. DELIVERY

  - Commit tilegen.c (the program source).
  - Commit all 472 generated SVG files under out/03_colors/.
  - Commit one zip of the full out/ tree.
  - Target branch: colour-stones-turn-1.
  - Commit a short README stating how to run tilegen.c and the file count.

## 10. SELF-CHECK BEFORE REPORTING

Render at least these and confirm they read correctly: one letter you consider
hardest (e.g. lowercase a), the seven-ball tile, and the pentagon. Compare the
red set and the blue set file-by-file to confirm structural identity.

## 11. REPORT — end with exactly this

    PROGRAM: tilegen.c , <lines> lines
    COLOURS GENERATED: <list>
    FILE COUNT PER COLOUR: <n>   (must be 59)
    TOTAL FILES: <n>             (must be 472)
    STRUCTURAL IDENTITY: comparing red vs blue, differences found = <describe; must be only word+hex>
    LETTERS VERIFIED READABLE: <which you rendered and confirmed>
    BUNDLE: <zip path> <bytes>
    ASSUMPTIONS MADE: <anything you decided that this spec left open, or "none">
    BLOCKED ON: <or "nothing">

If any requirement conflicts with another, quote both lines and stop before
building. A clear stop beats a wrong guess.
