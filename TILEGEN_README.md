# tilegen — SVG Tile Generator (RED)

Generates a complete set of SVG tiles for the colour RED (`#ED0A3F`).

## How to build

```sh
cc -std=c99 -o tilegen tilegen.c -lm
```

Any C99-conformant compiler works (gcc, clang, etc.).

## How to run

```sh
./tilegen
```

Output is written to `out/03_colors/red/`.  
The directory is created automatically if it does not exist.

## File count

**59 files** — the program prints the count and exits with an error if it is
not exactly 59:

| Group       | Count | Example filename                        |
|-------------|-------|-----------------------------------------|
| solid       | 1     | `03_colors_red_solid_screen.svg`        |
| ABC (A–Z)   | 26    | `03_colors_red_ABC_A.svg`               |
| abc (a–z)   | 26    | `03_colors_red_abc_a.svg`               |
| ball        | 1     | `03_colors_red_ball_one.svg`            |
| balls       | 1     | `03_colors_red_balls_seven.svg`         |
| shapes      | 4     | `03_colors_red_shapes_pentagon.svg`     |
| **Total**   | **59**|                                         |

## Bundle

`out.zip` contains the complete `out/` tree.
