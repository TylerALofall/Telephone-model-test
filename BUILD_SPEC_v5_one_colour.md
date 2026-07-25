# BUILD SPEC — SVG Tile Generator, one colour (write in C)

You are Opus 4.8, working alone, building from this specification. It defines
every output, dimension, and filename for ONE colour. Produce a C99 program,
run it, verify the output, deliver it. You author the program. Every example
shows the required RESULT so you can check your own work.

This colour: RED, hex #ED0A3F.

## 1. GOAL

One C99 program, tilegen.c, that writes a complete set of SVG tiles for red,
following the rules below exactly.

## 2. CANVAS

Every SVG opens with, and uses, this exact line:

    <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 512 512" role="img">

512 by 512 for every file, so all tiles are interchangeable in a grid.

## 3. OUTPUT AND NAMING

All files in:  out/03_colors/red/

Every filename:  03_colors_red_[group]_[item].svg

  [group] = solid | ABC | abc | ball | balls | shapes
  [item]  = per section 5

Uppercase ABC sorts before lowercase abc, keeping the two blocks separate in a
large directory. Examples:

    03_colors_red_solid_screen.svg
    03_colors_red_ABC_A.svg
    03_colors_red_abc_a.svg
    03_colors_red_ball_one.svg
    03_colors_red_balls_seven.svg
    03_colors_red_shapes_pentagon.svg

## 4. COLOUR RULE

The colour hex #ED0A3F fills every coloured element. Backgrounds that are not
the colour are white #FFFFFF. Letters and shapes render in the colour on white.
The solid screen is entirely the colour.

## 5. THE TILES

### 5.1 SOLID SCREEN  (group solid, item screen) — 1 file
One rect covering the canvas, filled with the colour. Required result:

    <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 512 512" role="img"><rect x="0" y="0" width="512" height="512" fill="#ED0A3F"/></svg>

### 5.2 LETTERS  (group ABC for A-Z, group abc for a-z) — 52 files
Item is the character itself: A ... Z, a ... z.

Each letter is an 8 by 8 bitmap: 8 columns, 8 rows, each lit cell a square rect,
colour on white. Fixed geometry for every letter:

  - 8x8 grid centered on the canvas.
  - Each cell 40 by 40. Glyph block 8*40 = 320.
  - Grid origin: (512 - 320) / 2 = 96, for both x and y.
  - For grid row r (0..7), column c (0..7), a lit cell is:
        x = 96 + c*40 , y = 96 + r*40 , width 40 , height 40 , fill #ED0A3F
  - Emit the white full-canvas background rect first, then one rect per lit
    cell in row-major order (row 0 left to right, then row 1, and so on).

You define the 8x8 pattern for all 52 characters as data in the program. Each
must be readable as its character. Uppercase and lowercase are distinct designs.

Required result shape (example, one lit cell shown):

    <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 512 512" role="img"><rect x="0" y="0" width="512" height="512" fill="#FFFFFF"/><rect x="96" y="96" width="40" height="40" fill="#ED0A3F"/> ...more lit cells... </svg>

### 5.3 BALL  (group ball, item one) — 1 file
White background, one centered circle, colour fill, large and smooth:

    <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 512 512" role="img"><rect x="0" y="0" width="512" height="512" fill="#FFFFFF"/><circle cx="256" cy="256" r="180" fill="#ED0A3F"/></svg>

### 5.4 SEVEN BALLS  (group balls, item seven) — 1 file
White background, seven circles r=60: one centered at (256,256), six at radius
150 from center at angles 0, 60, 120, 180, 240, 300 degrees, computed as
x = 256 + 150*cos(angle), y = 256 + 150*sin(angle), rounded to whole numbers.
Background first, then center, then the six in angle order. Required result:

    <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 512 512" role="img"><rect x="0" y="0" width="512" height="512" fill="#FFFFFF"/><circle cx="256" cy="256" r="60" fill="#ED0A3F"/><circle cx="406" cy="256" r="60" fill="#ED0A3F"/><circle cx="331" cy="386" r="60" fill="#ED0A3F"/><circle cx="181" cy="386" r="60" fill="#ED0A3F"/><circle cx="106" cy="256" r="60" fill="#ED0A3F"/><circle cx="181" cy="126" r="60" fill="#ED0A3F"/><circle cx="331" cy="126" r="60" fill="#ED0A3F"/></svg>

### 5.5 SHAPES  (group shapes) — 4 files: triangle, square, pentagon, hexagon
White background, one shape centered at (256,256), colour fill, each sized to a
bounding circle of radius 180 so they read at comparable size.

  - triangle: 3 vertices on the radius-180 circle, first at angle -90 (point up),
    then +120, +240.
  - square: a rect 360 by 360 centered: x=76 y=76 width=360 height=360.
  - pentagon: 5 vertices on the radius-180 circle, first at -90, step 72.
  - hexagon: 6 vertices on the radius-180 circle, first at -90, step 60.

Polygons use <polygon points="..."> with vertices rounded to whole numbers.
Background first, then the shape. Required result (square example):

    <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 512 512" role="img"><rect x="0" y="0" width="512" height="512" fill="#FFFFFF"/><rect x="76" y="76" width="360" height="360" fill="#ED0A3F"/></svg>

## 6. FILE COUNT

    1 solid + 26 uppercase + 26 lowercase + 1 ball + 1 seven balls + 4 shapes = 59

The program prints the count and stops with an error if it is not 59.

## 7. DELIVERY

  - Commit tilegen.c.
  - Commit all 59 SVG files under out/03_colors/red/.
  - Commit one zip of out/.
  - Commit a short README: how to run tilegen.c, and the file count.
  - Target branch: colour-stones-turn-1.

## 8. SELF-CHECK BEFORE REPORTING

Render and confirm readable: the lowercase a, the seven-ball tile, the pentagon.

## 9. REPORT — end with exactly this

    PROGRAM: tilegen.c , <lines> lines
    FILE COUNT: <n>  (must be 59)
    FILES: <name> <bytes>, one line each
    LETTERS VERIFIED READABLE: <which you rendered and confirmed>
    BUNDLE: <zip path> <bytes>
    ASSUMPTIONS MADE: <anything this spec left open that you decided, or "none">
    BLOCKED ON: <or "nothing">

If two requirements conflict, quote both lines and stop. A clear stop beats a
wrong guess.
