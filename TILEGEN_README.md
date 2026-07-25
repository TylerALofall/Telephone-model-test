# tilegen — SVG Tile Generator (v9)

## Overview

`tilegen.c` is a C99 program that writes a complete set of SVG tiles for one
colour per invocation.  Running it for all eight colours produces 472 SVG files.

SVG files are **geometry only**: each file opens with `<svg viewBox="0 0 512 512">`
and contains only shape primitives.  There is no XML declaration, no `xmlns`,
no `width`, no `height`, and none of the following are permitted anywhere inside
a file: `xmlns`, `xmlns:xlink`, `xlink:href`, `href`, `<title>`, `<desc>`,
`<metadata>`.

## Build

```sh
cc -std=c99 -O2 -Wall tilegen.c -lm -o tilegen
```

## Usage

```sh
./tilegen <colour>
```

`<colour>` must be one of:

| Word   | Hex       |
|--------|-----------|
| red    | `#ED0A3F` |
| orange | `#FF8833` |
| yellow | `#FBE870` |
| green  | `#01A638` |
| blue   | `#0066FF` |
| violet | `#8359A3` |
| brown  | `#AF593E` |
| black  | `#000000` |

## Generate all eight colour sets

```sh
for c in red orange yellow green blue violet brown black; do
    ./tilegen "$c"
done
```

## Output

Files are written to `out/03_colors/<colour>/` with the naming pattern:

```
03_colors_<colour>_<group>_<item>.svg
```

Groups and items per colour:

| Group  | Items                               | Count |
|--------|-------------------------------------|-------|
| solid  | screen                              | 1     |
| ABC    | A … Z                               | 26    |
| abc    | a … z                               | 26    |
| ball   | one                                 | 1     |
| balls  | seven                               | 1     |
| shapes | triangle, square, pentagon, hexagon | 4     |
| **Total** |                                  | **59** |

## Verification

The program prints a summary line and exits with a non-zero status if the count
is not exactly 59.  To verify no forbidden attributes were written:

```sh
grep -r "xmlns\|xlink\|<title\|<desc\|<metadata" out/ && echo FAIL || echo CLEAN
```

8 colours × 59 files = **472 SVG files**.
