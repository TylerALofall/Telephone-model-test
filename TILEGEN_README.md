# tilegen — SVG Tile Generator (v10)

## Build

```sh
cc -std=c99 -O2 -Wall tilegen.c -lm -o tilegen
```

## Usage

```sh
./tilegen <colour-word> <hex>
```

Valid colour words:

- red
- orange
- yellow
- green
- blue
- violet
- brown
- black

Run all required sets:

```sh
./tilegen red #ED0A3F
./tilegen orange #FF8833
./tilegen yellow #FBE870
./tilegen green #01A638
./tilegen blue #0066FF
./tilegen violet #8359A3
./tilegen brown #AF593E
./tilegen black #000000
```

## Output

Per colour, 59 files are written under `out/03_colors/<colour>/`:

- solid: 1
- ABC: 26
- abc: 26
- ball: 1
- balls: 1
- shapes: 4

Total after all 8 colours: 472 SVG files.
