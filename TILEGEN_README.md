# tilegen — orange SVG tile generator

## Build
```
cc -std=c99 -O2 -Wall tilegen.c -lm -o tilegen
```

## Run
```
./tilegen
```

Writes **59 SVG files** to `out/03_colors/orange/`.

## Output
| Group  | Items                     | Count |
|--------|---------------------------|-------|
| solid  | screen                    | 1     |
| ABC    | A – Z                     | 26    |
| abc    | a – z                     | 26    |
| ball   | one                       | 1     |
| balls  | seven                     | 1     |
| shapes | triangle square pentagon hexagon | 4 |
| **Total** |                        | **59** |

## Spec
- Colour: `#FF8833`
- Canvas: `viewBox="0 0 512 512"` only — no `width=` or `height=` on root
- Letters: 8×8 bitmap, cell 40×40, grid origin (96, 96)
- Shapes: radius-180 bounding circle centred at (256, 256)

## Delivery
- `tilegen.c` — source
- `out/03_colors/orange/` — 59 SVGs
- `out.zip` — zip of the `out/` directory
