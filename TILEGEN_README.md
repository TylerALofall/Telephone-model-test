# tilegen — SVG Tile Generator

## Overview

`tilegen.c` is a C99 program that generates a complete set of SVG tiles for one
colour per run.  Running it for all eight supported colours produces 472 SVG
files that are structurally identical — the only differences between colour sets
are the colour word in filenames and the colour hex in fill values.

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

| Group  | Items                             | Count |
|--------|-----------------------------------|-------|
| solid  | screen                            | 1     |
| ABC    | A … Z                             | 26    |
| abc    | a … z                             | 26    |
| ball   | one                               | 1     |
| balls  | seven                             | 1     |
| shapes | triangle, square, pentagon, hexagon | 4   |
| **Total** |                                | **59** |

## File count

The program prints the count at the end and exits with a non-zero status if it
is not exactly 59.

## Verified output

- 8 colours × 59 files = **472 SVG files**
- All tiles use a 512 × 512 canvas (`viewBox="0 0 512 512"`)
- Structural identity confirmed: `red` vs `blue` differ only in colour word
  and colour hex — zero structural differences
- Bundle: `out_03_colors.zip` (full `out/` tree)
