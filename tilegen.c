/* tilegen.c — SVG tile generator, C99
 *
 * Usage:   ./tilegen <colour>
 *   colour: red | orange | yellow | green | blue | violet | brown | black
 *
 * Writes 59 SVG files under out/03_colors/<colour>/
 * Named: 03_colors_<colour>_<group>_<item>.svg
 *
 * Build:  cc -std=c99 -O2 -Wall tilegen.c -lm -o tilegen
 *
 * SVG format: geometry only.
 * Root element is exactly: <svg viewBox="0 0 512 512">
 * No XML declaration, no xmlns, no width, no height.
 * Forbidden inside every file: xmlns, xmlns:xlink, xlink:href, href,
 * <title>, <desc>, <metadata>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ───── colour table ───── */
static const struct { const char *word; const char *hex; } COLOURS[] = {
    {"red",    "#ED0A3F"},
    {"orange", "#FF8833"},
    {"yellow", "#FBE870"},
    {"green",  "#01A638"},
    {"blue",   "#0066FF"},
    {"violet", "#8359A3"},
    {"brown",  "#AF593E"},
    {"black",  "#000000"},
};
#define N_COLOURS 8

/* ───── letter bitmaps ─────
 * GLYPHS[0..25]  = A..Z (uppercase)
 * GLYPHS[26..51] = a..z (lowercase)
 * Each entry: 8 rows; each row: 8 bits.
 * Bit 7 = column 0 (leftmost), bit 0 = column 7 (rightmost).
 * A set bit means that cell is filled with the colour.          */
static const unsigned char GLYPHS[52][8] = {
    /* A */ {0x18,0x24,0x42,0x81,0xFF,0x81,0x81,0x00},
    /* B */ {0xFC,0x82,0x82,0xFC,0x82,0x82,0xFC,0x00},
    /* C */ {0x3E,0x40,0x80,0x80,0x80,0x40,0x3E,0x00},
    /* D */ {0xF8,0x84,0x82,0x82,0x82,0x84,0xF8,0x00},
    /* E */ {0xFF,0x80,0x80,0xFE,0x80,0x80,0xFF,0x00},
    /* F */ {0xFF,0x80,0x80,0xFE,0x80,0x80,0x80,0x00},
    /* G */ {0x3C,0x42,0x80,0x8F,0x81,0x42,0x3C,0x00},
    /* H */ {0x81,0x81,0x81,0xFF,0x81,0x81,0x81,0x00},
    /* I */ {0xFF,0x18,0x18,0x18,0x18,0x18,0xFF,0x00},
    /* J */ {0x0F,0x01,0x01,0x01,0x81,0x42,0x3C,0x00},
    /* K */ {0x84,0x88,0x90,0xE0,0x90,0x88,0x84,0x00},
    /* L */ {0x80,0x80,0x80,0x80,0x80,0x80,0xFF,0x00},
    /* M */ {0x81,0xC3,0xA5,0x99,0x81,0x81,0x81,0x00},
    /* N */ {0x81,0xC1,0xA1,0x91,0x89,0x85,0x83,0x00},
    /* O */ {0x3C,0x42,0x81,0x81,0x81,0x42,0x3C,0x00},
    /* P */ {0xFC,0x82,0x82,0xFC,0x80,0x80,0x80,0x00},
    /* Q */ {0x3C,0x42,0x81,0x81,0x89,0x46,0x3D,0x00},
    /* R */ {0xFC,0x82,0x82,0xFC,0x90,0x88,0x84,0x00},
    /* S */ {0x3E,0x40,0x40,0x3C,0x02,0x02,0xFC,0x00},
    /* T */ {0xFF,0x18,0x18,0x18,0x18,0x18,0x18,0x00},
    /* U */ {0x81,0x81,0x81,0x81,0x81,0x42,0x3C,0x00},
    /* V */ {0x81,0x81,0x42,0x42,0x24,0x24,0x18,0x00},
    /* W */ {0x81,0x81,0x81,0x99,0xA5,0xC3,0x00,0x00},
    /* X */ {0x81,0x42,0x24,0x18,0x24,0x42,0x81,0x00},
    /* Y */ {0x81,0x42,0x24,0x18,0x18,0x18,0x18,0x00},
    /* Z */ {0xFF,0x06,0x0C,0x18,0x30,0x60,0xFF,0x00},

    /* a */ {0x00,0x3C,0x02,0x3E,0x42,0x42,0x3E,0x00},
    /* b */ {0x80,0x80,0xBC,0xC2,0x82,0xC2,0xBC,0x00},
    /* c */ {0x00,0x00,0x3C,0x40,0x40,0x40,0x3C,0x00},
    /* d */ {0x02,0x02,0x3E,0x42,0x82,0x42,0x3E,0x00},
    /* e */ {0x00,0x3C,0x42,0xFE,0x40,0x40,0x3C,0x00},
    /* f */ {0x0E,0x08,0x3E,0x08,0x08,0x08,0x08,0x00},
    /* g */ {0x00,0x3E,0x42,0x42,0x3E,0x02,0x3C,0x00},
    /* h */ {0x80,0x80,0xBC,0xC2,0x82,0x82,0x82,0x00},
    /* i */ {0x10,0x00,0x30,0x10,0x10,0x10,0x3C,0x00},
    /* j */ {0x04,0x00,0x04,0x04,0x04,0x44,0x38,0x00},
    /* k */ {0x80,0x80,0x88,0x90,0xE0,0x90,0x88,0x00},
    /* l */ {0x60,0x20,0x20,0x20,0x20,0x20,0x7C,0x00},
    /* m */ {0x00,0x00,0xDA,0x92,0x92,0x92,0x92,0x00},
    /* n */ {0x00,0x00,0xBC,0xC2,0x82,0x82,0x82,0x00},
    /* o */ {0x00,0x00,0x3C,0x42,0x81,0x42,0x3C,0x00},
    /* p */ {0x00,0xBC,0xC2,0x82,0xC2,0xBC,0x80,0x00},
    /* q */ {0x00,0x3C,0x42,0x82,0x42,0x3C,0x02,0x00},
    /* r */ {0x00,0x00,0xBC,0xC2,0x80,0x80,0x80,0x00},
    /* s */ {0x00,0x3E,0x40,0x3C,0x02,0x02,0x7C,0x00},
    /* t */ {0x20,0x20,0xF8,0x20,0x20,0x20,0x1E,0x00},
    /* u */ {0x00,0x00,0x82,0x82,0x82,0x42,0x3E,0x00},
    /* v */ {0x00,0x00,0x42,0x42,0x24,0x24,0x18,0x00},
    /* w */ {0x00,0x00,0x92,0x92,0xAA,0xAA,0x44,0x00},
    /* x */ {0x00,0x00,0x42,0x24,0x18,0x24,0x42,0x00},
    /* y */ {0x00,0x42,0x42,0x42,0x3E,0x02,0x3C,0x00},
    /* z */ {0x00,0x7E,0x04,0x08,0x10,0x20,0x7E,0x00},
};

static const char *UPPER_NAMES[26] = {
    "A","B","C","D","E","F","G","H","I","J","K","L","M",
    "N","O","P","Q","R","S","T","U","V","W","X","Y","Z"
};
static const char *LOWER_NAMES[26] = {
    "a","b","c","d","e","f","g","h","i","j","k","l","m",
    "n","o","p","q","r","s","t","u","v","w","x","y","z"
};

/* ───── helpers ───── */

static void make_dir(const char *path)
{
    if (mkdir(path, 0755) == -1 && errno != EEXIST) {
        fprintf(stderr, "Cannot create directory '%s': %s\n", path, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

static int iround(double x)
{
    return (int)round(x);
}

static FILE *open_tile(const char *dir, const char *colour,
                       const char *group, const char *item)
{
    char path[512];
    int n = snprintf(path, sizeof(path), "%s/03_colors_%s_%s_%s.svg",
                     dir, colour, group, item);
    if (n < 0 || (size_t)n >= sizeof(path)) {
        fprintf(stderr, "Path too long for colour='%s' group='%s' item='%s'\n",
                colour, group, item);
        exit(EXIT_FAILURE);
    }
    FILE *f = fopen(path, "w");
    if (!f) {
        fprintf(stderr, "Cannot open '%s': %s\n", path, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return f;
}

/* Geometry-only SVG root — no xmlns, no width, no height */
static void write_svg_open(FILE *f)
{
    fprintf(f, "<svg viewBox=\"0 0 512 512\">\n");
}

static void write_svg_close(FILE *f)
{
    fprintf(f, "</svg>\n");
}

static void write_white_bg(FILE *f)
{
    fprintf(f, "<rect width=\"512\" height=\"512\" fill=\"#FFFFFF\"/>\n");
}

/* ───── tile writers ───── */

/* 1 solid screen */
static int write_solid(const char *dir, const char *colour, const char *hex)
{
    FILE *f = open_tile(dir, colour, "solid", "screen");
    write_svg_open(f);
    fprintf(f, "<rect width=\"512\" height=\"512\" fill=\"%s\"/>\n", hex);
    write_svg_close(f);
    fclose(f);
    return 1;
}

/* 52 letter tiles: A–Z (group "ABC") + a–z (group "abc") */
static int write_letters(const char *dir, const char *colour, const char *hex)
{
    int count = 0;

    for (int ch = 0; ch < 26; ch++) {
        FILE *f = open_tile(dir, colour, "ABC", UPPER_NAMES[ch]);
        write_svg_open(f);
        write_white_bg(f);
        const unsigned char *rows = GLYPHS[ch];
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                if ((rows[r] >> (7 - c)) & 1) {
                    fprintf(f, "<rect x=\"%d\" y=\"%d\" width=\"40\" height=\"40\""
                               " fill=\"%s\"/>\n",
                               96 + c * 40, 96 + r * 40, hex);
                }
            }
        }
        write_svg_close(f);
        fclose(f);
        count++;
    }

    for (int ch = 0; ch < 26; ch++) {
        FILE *f = open_tile(dir, colour, "abc", LOWER_NAMES[ch]);
        write_svg_open(f);
        write_white_bg(f);
        const unsigned char *rows = GLYPHS[26 + ch];
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                if ((rows[r] >> (7 - c)) & 1) {
                    fprintf(f, "<rect x=\"%d\" y=\"%d\" width=\"40\" height=\"40\""
                               " fill=\"%s\"/>\n",
                               96 + c * 40, 96 + r * 40, hex);
                }
            }
        }
        write_svg_close(f);
        fclose(f);
        count++;
    }

    return count;
}

/* 1 ball tile */
static int write_ball(const char *dir, const char *colour, const char *hex)
{
    FILE *f = open_tile(dir, colour, "ball", "one");
    write_svg_open(f);
    write_white_bg(f);
    fprintf(f, "<circle cx=\"256\" cy=\"256\" r=\"200\" fill=\"%s\"/>\n", hex);
    write_svg_close(f);
    fclose(f);
    return 1;
}

/* 1 seven-balls tile: centre + 6 surrounding at r=150 */
static int write_balls(const char *dir, const char *colour, const char *hex)
{
    FILE *f = open_tile(dir, colour, "balls", "seven");
    write_svg_open(f);
    write_white_bg(f);
    fprintf(f, "<circle cx=\"256\" cy=\"256\" r=\"60\" fill=\"%s\"/>\n", hex);
    for (int i = 0; i < 6; i++) {
        double a = i * 60.0 * M_PI / 180.0;
        fprintf(f, "<circle cx=\"%d\" cy=\"%d\" r=\"60\" fill=\"%s\"/>\n",
                iround(256.0 + 150.0 * cos(a)),
                iround(256.0 + 150.0 * sin(a)),
                hex);
    }
    write_svg_close(f);
    fclose(f);
    return 1;
}

/* 4 shape tiles: triangle, square, pentagon, hexagon */
static int write_shapes(const char *dir, const char *colour, const char *hex)
{
    int count = 0;

    /* triangle — 3 vertices on r=180, start=-90°, step=120° */
    {
        FILE *f = open_tile(dir, colour, "shapes", "triangle");
        write_svg_open(f);
        write_white_bg(f);
        int px[3], py[3];
        for (int i = 0; i < 3; i++) {
            double a = (-90.0 + i * 120.0) * M_PI / 180.0;
            px[i] = iround(256.0 + 180.0 * cos(a));
            py[i] = iround(256.0 + 180.0 * sin(a));
        }
        fprintf(f, "<polygon points=\"%d,%d %d,%d %d,%d\" fill=\"%s\"/>\n",
                px[0],py[0], px[1],py[1], px[2],py[2], hex);
        write_svg_close(f);
        fclose(f);
        count++;
    }

    /* square — axis-aligned, 76px inset on each side */
    {
        FILE *f = open_tile(dir, colour, "shapes", "square");
        write_svg_open(f);
        write_white_bg(f);
        fprintf(f, "<rect x=\"76\" y=\"76\" width=\"360\" height=\"360\""
                   " fill=\"%s\"/>\n", hex);
        write_svg_close(f);
        fclose(f);
        count++;
    }

    /* pentagon — 5 vertices on r=180, start=-90°, step=72° */
    {
        FILE *f = open_tile(dir, colour, "shapes", "pentagon");
        write_svg_open(f);
        write_white_bg(f);
        int px[5], py[5];
        for (int i = 0; i < 5; i++) {
            double a = (-90.0 + i * 72.0) * M_PI / 180.0;
            px[i] = iround(256.0 + 180.0 * cos(a));
            py[i] = iround(256.0 + 180.0 * sin(a));
        }
        fprintf(f, "<polygon points=\"%d,%d %d,%d %d,%d %d,%d %d,%d\""
                   " fill=\"%s\"/>\n",
                px[0],py[0], px[1],py[1], px[2],py[2],
                px[3],py[3], px[4],py[4], hex);
        write_svg_close(f);
        fclose(f);
        count++;
    }

    /* hexagon — 6 vertices on r=180, start=-90°, step=60° */
    {
        FILE *f = open_tile(dir, colour, "shapes", "hexagon");
        write_svg_open(f);
        write_white_bg(f);
        int px[6], py[6];
        for (int i = 0; i < 6; i++) {
            double a = (-90.0 + i * 60.0) * M_PI / 180.0;
            px[i] = iround(256.0 + 180.0 * cos(a));
            py[i] = iround(256.0 + 180.0 * sin(a));
        }
        fprintf(f, "<polygon points=\"%d,%d %d,%d %d,%d %d,%d %d,%d %d,%d\""
                   " fill=\"%s\"/>\n",
                px[0],py[0], px[1],py[1], px[2],py[2],
                px[3],py[3], px[4],py[4], px[5],py[5], hex);
        write_svg_close(f);
        fclose(f);
        count++;
    }

    return count;
}

/* ───── main ───── */

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr,
                "Usage: tilegen <colour>\n"
                "Colours: red orange yellow green blue violet brown black\n");
        return EXIT_FAILURE;
    }

    const char *word = argv[1];
    const char *hex  = NULL;

    for (int i = 0; i < N_COLOURS; i++) {
        if (strcmp(COLOURS[i].word, word) == 0) {
            hex = COLOURS[i].hex;
            break;
        }
    }
    if (!hex) {
        fprintf(stderr,
                "Unknown colour '%s'.\n"
                "Valid: red orange yellow green blue violet brown black\n",
                word);
        return EXIT_FAILURE;
    }

    char outdir[512];
    int n = snprintf(outdir, sizeof(outdir), "out/03_colors/%s", word);
    if (n < 0 || (size_t)n >= sizeof(outdir)) {
        fprintf(stderr, "Output path too long for colour '%s'\n", word);
        return EXIT_FAILURE;
    }

    make_dir("out");
    make_dir("out/03_colors");
    make_dir(outdir);

    int count = 0;
    count += write_solid  (outdir, word, hex);   /*  1 */
    count += write_letters(outdir, word, hex);   /* 52 */
    count += write_ball   (outdir, word, hex);   /*  1 */
    count += write_balls  (outdir, word, hex);   /*  1 */
    count += write_shapes (outdir, word, hex);   /*  4 */
    /* total: 59 */

    printf("colour: %s   hex: %s   files: %d\n", word, hex, count);

    if (count != 59) {
        fprintf(stderr, "ERROR: expected 59 files, got %d\n", count);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
