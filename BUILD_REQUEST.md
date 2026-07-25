# BUILD REQUEST — COLOUR STONES
### Read fully before acting. Everything you need is in this message.

---

## WHO YOU ARE

You are the sole implementer on the repository `TylerALofall/Telephone-model-test`.

You have never seen this project before. That is expected. This message
contains every fact, format, and value you need. You will not receive help
after this. Complete the whole task from this message alone.

## RULES

1. **NO MOCK.** No placeholder files, stub content, sample data, or
   "example" artifacts. Every file you write is a real, final artifact.
2. **NO AUTONOMY.** Do not create GitHub Actions, workflows, scheduled jobs,
   or background agents. Write files, commit, stop.
3. **NO INVENTION.** Every glyph bitmap, colour, dimension, and filename is
   given below. Do not design your own letterforms, pick your own colours, or
   choose your own dimensions. Reproduce what is specified.
4. **STOP ON CONFLICT.** If any instruction here contradicts another, stop and
   report the contradiction. Do not resolve it by choosing.
5. **VERIFY.** "Done" means the file exists and you opened it and confirmed its
   contents match this spec. Report anything you did not verify.
6. **NO EXTERNAL LANGUAGE.** These are static `.svg` and `.txt` files. Write
   them directly. No Python, Node, or shell script is needed or wanted.

---

## THE TASK

Build the complete visual artifact set for **8 colours**. For each colour you
produce four kinds of SVG file. That is the whole job.

The 8 colours, with their exact fill values:

| Colour | Word | Fill |
|---|---|---|
| Red | `Red` | `#ED0A3F` |
| Orange | `Orange` | `#FF8833` |
| Yellow | `Yellow` | `#FBE870` |
| Green | `Green` | `#01A638` |
| Blue | `Blue` | `#0066FF` |
| Violet | `Violet` | `#8359A3` |
| Brown | `Brown` | `#AF593E` |
| Black | `Black` | `#000000` |

These are the eight colours of the original 1903 crayon box. The hex values
are the crayon spec values. Use them exactly.

---

## THE FOUR FILE KINDS

For a colour whose word is `Word` and whose fill is `#HEX`, spelled with
letters `L1 L2 L3 ...`:

| Kind | Filename | Content |
|---|---|---|
| Swatch | `[colour].svg` (e.g. `red.svg`) | solid field of `#HEX` |
| Dot | `[colour]_dot.svg` (e.g. `red_dot.svg`) | white field, one filled circle of `#HEX` |
| Name | `[colour]_name_text.svg` (e.g. `red_name_text.svg`) | the whole word spelled in glyphs, ink `#HEX` |
| Letter | `[colour]_letter_<glyph>.svg` (e.g. `red_letter_R.svg`, `red_letter_e.svg`, `red_letter_d.svg`) | one file per letter of the word, in order, each glyph ink `#HEX` |

The word is spelled with its **real spelling and real capitalization**: first
letter uppercase, rest lowercase. `Red` is `R`, `e`, `d` — three letter files.
`Orange` is `O`, `r`, `a`, `n`, `g`, `e` — six letter files. Do not add
letters. Do not double letters into upper and lower case. Spell the word as
written.

All filenames use the **lowercase colour word** as prefix: `red_`, `orange_`,
`yellow_`, `green_`, `blue_`, `violet_`, `brown_`, `black_`.

Write every file into `artifacts/` at the repository root.

---

## SVG FORMAT — MEASURED, EXACT

Every SVG uses this frame. `W` is the viewBox width, computed below.

```xml
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 W 88" role="img">
  ...
</svg>
```

### Swatch — `Word.svg`

viewBox `0 0 88 88`. One rect, full field.

```xml
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 88 88" role="img">
  <rect width="100%" height="100%" fill="#ED0A3F"/>
</svg>
```

### Dot — `red_dot.svg`

viewBox `0 0 88 88`. White field, one circle dead centre.

```xml
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 88 88" role="img">
  <rect width="100%" height="100%" fill="white"/>
  <circle cx="44" cy="44" r="28" fill="#ED0A3F"/>
</svg>
```

### Letter and Name — glyph rendering

A glyph is an 8×8 grid. Each lit cell is an 8×8 `rect`. The grid maps to SVG
coordinates like this:

```
one cell        8 units square
one glyph       64 units wide (8 cells), 64 tall
glyph advance   72 units between glyph origins
cell position   x = 8 + (charIndex * 72) + (col * 8)
                y = 8 + (row * 8)
charIndex       0 for the first glyph, 1 for the next, ...
row, col        0..7 within the glyph
```

viewBox width by glyph count `n`:

```
W = 88 + (n - 1) * 72

n=1 -> 88     n=3 -> 232    n=5 -> 376
n=2 -> 160    n=4 -> 304    n=6 -> 448
```

A **letter** file has n=1, so viewBox `0 0 88 88`, one glyph at charIndex 0.
A **name** file has n = length of the word, all glyphs left to right.

Every SVG opens with the white background rect, then one `<rect>` per lit cell.
**Ink is the colour's `#HEX`, not black.** The word appears in its own colour.

```xml
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 88 88" role="img">
  <rect width="100%" height="100%" fill="white"/>
  <rect x="8" y="8" width="8" height="8" fill="#ED0A3F"/>
  ...
</svg>
```

---

## THE GLYPHS — REPRODUCE THESE EXACTLY

Each grid is 8 rows of 8 columns. `#` is a lit cell (emit a rect). `.` is
empty (emit nothing). These are the project's real letterforms. Do not alter
them. Every glyph you need for all 8 words is here.

```
R
.####...
.#...#..
.#...#..
.####...
.#.#....
.#..#...
.#...#..
........

e
........
........
..###...
.#...#..
.#####..
.#......
..####..
........

d
.....#..
.....#..
..##.#..
.#..##..
.#...#..
.#...#..
..####..
........

O
..###...
.#...#..
.#...#..
.#...#..
.#...#..
.#...#..
..###...
........

r
........
........
.#.##...
.##..#..
.#......
.#......
.#......
........

a
........
........
..###...
.....#..
..####..
.#...#..
..####..
........

n
........
........
.#.##...
.##..#..
.#...#..
.#...#..
.#...#..
........

g
........
..####..
.#...#..
.#...#..
..####..
.....#..
.####...
........

Y
.#...#..
.#...#..
..#.#...
...#....
...#....
...#....
...#....
........

l
..##....
...#....
...#....
...#....
...#....
...#....
..###...
........

o
........
........
..###...
.#...#..
.#...#..
.#...#..
..###...
........

w
........
........
.#...#..
.#...#..
.#.#.#..
.#.#.#..
..#.#...
........

G
..####..
.#......
.#......
.#.###..
.#...#..
.#...#..
..####..
........

b
.#......
.#......
.#.##...
.##..#..
.#...#..
.#...#..
.####...
........

u
........
........
.#...#..
.#...#..
.#...#..
.#..##..
..##.#..
........

V
.#...#..
.#...#..
.#...#..
.#...#..
.#...#..
..#.#...
...#....
........

i
...#....
........
..##....
...#....
...#....
...#....
..###...
........

t
..#.....
..#.....
.###....
..#.....
..#.....
..#..#..
...##...
........

B
.####...
.#...#..
.#...#..
.####...
.#...#..
.#...#..
.####...
........

c
........
........
..####..
.#......
.#......
.#......
..####..
........

k
.#......
.#......
.#..#...
.#.#....
.##.....
.#.#....
.#..#...
........
```

Words and their glyph sequences:

```
Red     R e d
Orange  O r a n g e
Yellow  Y e l l o w
Green   G r e e n
Blue    B l u e
Violet  V i o l e t
Brown   B r o w n
Black   B l a c k
```

---

## WORKED EXAMPLE — `red_letter_R.svg`

One glyph, charIndex 0, ink `#ED0A3F`. From the `R` grid, the lit cells and
their coordinates (`x = 8 + col*8`, `y = 8 + row*8`):

```
row 0: cols 1,2,3,4   -> x=16,24,32,40  y=8
row 1: cols 1,5       -> x=16,48        y=16
row 2: cols 1,5       -> x=16,48        y=24
row 3: cols 1,2,3,4   -> x=16,24,32,40  y=32
row 4: cols 1,3       -> x=16,32        y=40
row 5: cols 1,4       -> x=16,40        y=48
row 6: cols 1,5       -> x=16,48        y=56
row 7: (none)
```

```xml
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 88 88" role="img">
  <rect width="100%" height="100%" fill="white"/>
  <rect x="16" y="8" width="8" height="8" fill="#ED0A3F"/>
  <rect x="24" y="8" width="8" height="8" fill="#ED0A3F"/>
  <rect x="32" y="8" width="8" height="8" fill="#ED0A3F"/>
  <rect x="40" y="8" width="8" height="8" fill="#ED0A3F"/>
  <rect x="16" y="16" width="8" height="8" fill="#ED0A3F"/>
  <rect x="48" y="16" width="8" height="8" fill="#ED0A3F"/>
  <rect x="16" y="24" width="8" height="8" fill="#ED0A3F"/>
  <rect x="48" y="24" width="8" height="8" fill="#ED0A3F"/>
  <rect x="16" y="32" width="8" height="8" fill="#ED0A3F"/>
  <rect x="24" y="32" width="8" height="8" fill="#ED0A3F"/>
  <rect x="32" y="32" width="8" height="8" fill="#ED0A3F"/>
  <rect x="40" y="32" width="8" height="8" fill="#ED0A3F"/>
  <rect x="16" y="40" width="8" height="8" fill="#ED0A3F"/>
  <rect x="32" y="40" width="8" height="8" fill="#ED0A3F"/>
  <rect x="16" y="48" width="8" height="8" fill="#ED0A3F"/>
  <rect x="40" y="48" width="8" height="8" fill="#ED0A3F"/>
  <rect x="16" y="56" width="8" height="8" fill="#ED0A3F"/>
  <rect x="48" y="56" width="8" height="8" fill="#ED0A3F"/>
</svg>
```

For a **name** file, the same, but each successive glyph adds `72` to every x.
Glyph 0 uses x-base 8, glyph 1 uses x-base 80, glyph 2 uses x-base 152, and so
on. The white background rect is written once, first.

---

## FILE COUNT

Per colour: 1 swatch + 1 dot + 1 name + (letters). Word lengths: Red 3,
Orange 6, Yellow 6, Green 5, Blue 4, Violet 6, Brown 5, Black 5.

```
Red     3 + 3 = 6
Orange  3 + 6 = 9
Yellow  3 + 6 = 9
Green   3 + 5 = 8
Blue    3 + 4 = 7
Violet  3 + 6 = 9
Brown   3 + 5 = 8
Black   3 + 5 = 8
                 ---
Total            64 files
```

Confirm your output totals 64 files before you report done.

---

## COMMIT

Commit all files in one commit to a new branch `colour-stones-turn-1`. Commit
message: `Build colour stone SVG artifacts for 8 crayon colours`.

---

## REPORT — end your turn with exactly this

```
FILE COUNT
  <total>   (must be 64)

FILES CREATED
  <path>  <bytes>     one line each

VERIFICATION
  <how you confirmed contents match spec — name specific files you opened>

ASSUMPTIONS MADE
  <anything not stated here that you decided>
  <"none" only if truly none>

BLOCKED ON
  <anything you could not complete and why>
  <"nothing" only if truly nothing>
```

Then stop.
