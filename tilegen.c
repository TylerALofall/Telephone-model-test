/* tilegen.c — SVG tile generator, C99
 *
 * Usage:   ./tilegen <colour-word> <hex>
 *   colour-word: red | orange | yellow | green | blue | violet | brown | black
 *   hex: #RRGGBB
 *
 * Writes 59 SVG files under out/03_colors/<colour>/
 * Named: 03_colors_<colour>_<group>_<item>.svg
 *
 * Build:  cc -std=c99 -O2 -Wall tilegen.c -o tilegen
 *
 * SVG format: geometry only.
 * Root element is exactly: <svg viewBox="0 0 8 8">
 * No XML declaration, no xmlns, no width, no height.
 * Forbidden inside every file: xmlns, xmlns:xlink, xlink:href, href,
 * <a>, <use>, <image>, <script>, <style>, <metadata>, <title>, <desc>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#define N_COLOUR_WORDS 8
#define CANVAS_UNITS 8
#define MAX_PATH_LEN 512
#define BALL_RADIUS 2.8
#define SEVEN_BALL_RADIUS 0.94

/* ───── valid colour words ───── */
static const char *COLOUR_WORDS[N_COLOUR_WORDS] = {
    "red", "orange", "yellow", "green", "blue", "violet", "brown", "black"
};

/* ───── letter bitmaps ─────
 * GLYPHS[0..25]  = A..Z (uppercase)
 * GLYPHS[26..51] = a..z (lowercase)
 * Each entry: 8 rows; each row: 8 bits.
 * Bit 7 = column 0 (leftmost), bit 0 = column 7 (rightmost).
 * A set bit means that cell is filled with the colour.          */
static const unsigned char GLYPHS[52][8] = {
    /* A */ {0x38,0x44,0x44,0x7C,0x44,0x44,0x44,0x00},
    /* B */ {0x78,0x44,0x44,0x78,0x44,0x44,0x78,0x00},
    /* C */ {0x3C,0x40,0x40,0x40,0x40,0x40,0x3C,0x00},
    /* D */ {0x78,0x44,0x44,0x44,0x44,0x44,0x78,0x00},
    /* E */ {0x7C,0x40,0x40,0x78,0x40,0x40,0x7C,0x00},
    /* F */ {0x7C,0x40,0x40,0x78,0x40,0x40,0x40,0x00},
    /* G */ {0x3C,0x40,0x40,0x5C,0x44,0x44,0x3C,0x00},
    /* H */ {0x44,0x44,0x44,0x7C,0x44,0x44,0x44,0x00},
    /* I */ {0x7C,0x10,0x10,0x10,0x10,0x10,0x7C,0x00},
    /* J */ {0x1C,0x08,0x08,0x08,0x48,0x48,0x30,0x00},
    /* K */ {0x44,0x48,0x50,0x60,0x50,0x48,0x44,0x00},
    /* L */ {0x40,0x40,0x40,0x40,0x40,0x40,0x7C,0x00},
    /* M */ {0x44,0x6C,0x54,0x54,0x44,0x44,0x44,0x00},
    /* N */ {0x44,0x64,0x54,0x4C,0x44,0x44,0x44,0x00},
    /* O */ {0x38,0x44,0x44,0x44,0x44,0x44,0x38,0x00},
    /* P */ {0x78,0x44,0x44,0x78,0x40,0x40,0x40,0x00},
    /* Q */ {0x38,0x44,0x44,0x44,0x54,0x48,0x34,0x00},
    /* R */ {0x78,0x44,0x44,0x78,0x50,0x48,0x44,0x00},
    /* S */ {0x3C,0x40,0x40,0x38,0x04,0x04,0x78,0x00},
    /* T */ {0x7C,0x10,0x10,0x10,0x10,0x10,0x10,0x00},
    /* U */ {0x44,0x44,0x44,0x44,0x44,0x44,0x38,0x00},
    /* V */ {0x44,0x44,0x44,0x44,0x44,0x28,0x10,0x00},
    /* W */ {0x44,0x44,0x44,0x54,0x54,0x54,0x28,0x00},
    /* X */ {0x44,0x44,0x28,0x10,0x28,0x44,0x44,0x00},
    /* Y */ {0x44,0x44,0x28,0x10,0x10,0x10,0x10,0x00},
    /* Z */ {0x7C,0x04,0x08,0x10,0x20,0x40,0x7C,0x00},

    /* a */ {0x00,0x00,0x38,0x04,0x3C,0x44,0x3C,0x00},
    /* b */ {0x40,0x40,0x58,0x64,0x44,0x44,0x78,0x00},
    /* c */ {0x00,0x00,0x3C,0x40,0x40,0x40,0x3C,0x00},
    /* d */ {0x04,0x04,0x34,0x4C,0x44,0x44,0x3C,0x00},
    /* e */ {0x00,0x00,0x38,0x44,0x7C,0x40,0x3C,0x00},
    /* f */ {0x18,0x24,0x20,0x70,0x20,0x20,0x20,0x00},
    /* g */ {0x00,0x3C,0x44,0x44,0x3C,0x04,0x78,0x00},
    /* h */ {0x40,0x40,0x58,0x64,0x44,0x44,0x44,0x00},
    /* i */ {0x10,0x00,0x30,0x10,0x10,0x10,0x38,0x00},
    /* j */ {0x08,0x00,0x18,0x08,0x08,0x48,0x30,0x00},
    /* k */ {0x40,0x40,0x48,0x50,0x60,0x50,0x48,0x00},
    /* l */ {0x30,0x10,0x10,0x10,0x10,0x10,0x38,0x00},
    /* m */ {0x00,0x00,0x68,0x54,0x54,0x54,0x54,0x00},
    /* n */ {0x00,0x00,0x58,0x64,0x44,0x44,0x44,0x00},
    /* o */ {0x00,0x00,0x38,0x44,0x44,0x44,0x38,0x00},
    /* p */ {0x00,0x78,0x44,0x44,0x78,0x40,0x40,0x00},
    /* q */ {0x00,0x3C,0x44,0x44,0x3C,0x04,0x04,0x00},
    /* r */ {0x00,0x00,0x58,0x64,0x40,0x40,0x40,0x00},
    /* s */ {0x00,0x00,0x3C,0x40,0x38,0x04,0x78,0x00},
    /* t */ {0x20,0x20,0x70,0x20,0x20,0x24,0x18,0x00},
    /* u */ {0x00,0x00,0x44,0x44,0x44,0x4C,0x34,0x00},
    /* v */ {0x00,0x00,0x44,0x44,0x44,0x28,0x10,0x00},
    /* w */ {0x00,0x00,0x44,0x44,0x54,0x54,0x28,0x00},
    /* x */ {0x00,0x00,0x44,0x28,0x10,0x28,0x44,0x00},
    /* y */ {0x00,0x44,0x44,0x44,0x3C,0x04,0x78,0x00},
    /* z */ {0x00,0x00,0x7C,0x08,0x10,0x20,0x7C,0x00},
};

static const char *UPPER_NAMES[26] = {
    "A","B","C","D","E","F","G","H","I","J","K","L","M",
    "N","O","P","Q","R","S","T","U","V","W","X","Y","Z"
};
static const char *LOWER_NAMES[26] = {
    "a","b","c","d","e","f","g","h","i","j","k","l","m",
    "n","o","p","q","r","s","t","u","v","w","x","y","z"
};

static void make_dir(const char *path)
{
    if (mkdir(path, 0755) == -1 && errno != EEXIST) {
        int err = errno;
        fprintf(stderr, "Cannot create directory '%s': %s\n", path, strerror(err));
        exit(EXIT_FAILURE);
    }
}

static int is_valid_colour_word(const char *word)
{
    for (int i = 0; i < N_COLOUR_WORDS; i++) {
        if (strcmp(COLOUR_WORDS[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

static int is_valid_hex(const char *hex)
{
    if (!hex || strlen(hex) != 7 || hex[0] != '#') {
        return 0;
    }
    for (int i = 1; i < 7; i++) {
        if (!isxdigit((unsigned char)hex[i])) {
            return 0;
        }
    }
    return 1;
}

static FILE *open_tile(const char *dir, const char *colour,
                       const char *group, const char *item)
{
    char path[MAX_PATH_LEN];
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

static void write_svg_open(FILE *f)
{
    fprintf(f, "<svg viewBox=\"0 0 %d %d\">\n", CANVAS_UNITS, CANVAS_UNITS);
}

static void write_svg_close(FILE *f)
{
    fprintf(f, "</svg>\n");
}

static void write_white_bg(FILE *f)
{
    fprintf(f, "<rect width=\"%d\" height=\"%d\" fill=\"#FFFFFF\"/>\n",
            CANVAS_UNITS, CANVAS_UNITS);
}

static int write_solid(const char *dir, const char *colour, const char *hex)
{
    FILE *f = open_tile(dir, colour, "solid", "screen");
    write_svg_open(f);
    fprintf(f, "<rect width=\"%d\" height=\"%d\" fill=\"%s\"/>\n",
            CANVAS_UNITS, CANVAS_UNITS, hex);
    write_svg_close(f);
    fclose(f);
    return 1;
}

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
                    fprintf(f, "<rect x=\"%d\" y=\"%d\" width=\"1\" height=\"1\" fill=\"%s\"/>\n",
                            c, r, hex);
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
                    fprintf(f, "<rect x=\"%d\" y=\"%d\" width=\"1\" height=\"1\" fill=\"%s\"/>\n",
                            c, r, hex);
                }
            }
        }
        write_svg_close(f);
        fclose(f);
        count++;
    }

    return count;
}

static int write_ball(const char *dir, const char *colour, const char *hex)
{
    FILE *f = open_tile(dir, colour, "ball", "one");
    write_svg_open(f);
    write_white_bg(f);
    fprintf(f, "<circle cx=\"4\" cy=\"4\" r=\"%.1f\" fill=\"%s\"/>\n", BALL_RADIUS, hex);
    write_svg_close(f);
    fclose(f);
    return 1;
}

static int write_balls(const char *dir, const char *colour, const char *hex)
{
    /* Centre + six around a radius-2.34 ring, rounded to 2 decimals per spec. */
    static const char *centers[7][2] = {
        {"4",    "4"},
        {"6.34", "4"},
        {"5.17", "6.03"},
        {"2.83", "6.03"},
        {"1.66", "4"},
        {"2.83", "1.97"},
        {"5.17", "1.97"}
    };

    FILE *f = open_tile(dir, colour, "balls", "seven");
    write_svg_open(f);
    write_white_bg(f);
    for (int i = 0; i < 7; i++) {
        fprintf(f, "<circle cx=\"%s\" cy=\"%s\" r=\"%.2f\" fill=\"%s\"/>\n",
                centers[i][0], centers[i][1], SEVEN_BALL_RADIUS, hex);
    }
    write_svg_close(f);
    fclose(f);
    return 1;
}

static int write_shapes(const char *dir, const char *colour, const char *hex)
{
    int count = 0;

    {
        FILE *f = open_tile(dir, colour, "shapes", "triangle");
        write_svg_open(f);
        write_white_bg(f);
        fprintf(f, "<polygon points=\"4,1.2 6.42,5.4 1.58,5.4\" fill=\"%s\"/>\n", hex);
        write_svg_close(f);
        fclose(f);
        count++;
    }

    {
        FILE *f = open_tile(dir, colour, "shapes", "square");
        write_svg_open(f);
        write_white_bg(f);
        fprintf(f, "<rect x=\"1.2\" y=\"1.2\" width=\"5.6\" height=\"5.6\" fill=\"%s\"/>\n", hex);
        write_svg_close(f);
        fclose(f);
        count++;
    }

    {
        FILE *f = open_tile(dir, colour, "shapes", "pentagon");
        write_svg_open(f);
        write_white_bg(f);
        fprintf(f, "<polygon points=\"4,1.2 6.66,3.13 5.65,6.27 2.35,6.27 1.34,3.13\" fill=\"%s\"/>\n", hex);
        write_svg_close(f);
        fclose(f);
        count++;
    }

    {
        FILE *f = open_tile(dir, colour, "shapes", "hexagon");
        write_svg_open(f);
        write_white_bg(f);
        fprintf(f, "<polygon points=\"4,1.2 6.42,2.6 6.42,5.4 4,6.8 1.58,5.4 1.58,2.6\" fill=\"%s\"/>\n", hex);
        write_svg_close(f);
        fclose(f);
        count++;
    }

    return count;
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr,
                "Usage: tilegen <colour-word> <hex>\n"
                "colour-word: red orange yellow green blue violet brown black\n"
                "hex: #RRGGBB\n");
        return EXIT_FAILURE;
    }

    const char *word = argv[1];
    const char *hex = argv[2];

    if (!is_valid_colour_word(word)) {
        fprintf(stderr,
                "Unknown colour '%s'.\n"
                "Valid: red orange yellow green blue violet brown black\n",
                word);
        return EXIT_FAILURE;
    }

    if (!is_valid_hex(hex)) {
        fprintf(stderr, "Invalid hex '%s'. Expected #RRGGBB\n", hex);
        return EXIT_FAILURE;
    }

    char outdir[MAX_PATH_LEN];
    int n = snprintf(outdir, sizeof(outdir), "out/03_colors/%s", word);
    if (n < 0 || (size_t)n >= sizeof(outdir)) {
        fprintf(stderr, "Output path too long for colour '%s'\n", word);
        return EXIT_FAILURE;
    }

    make_dir("out");
    make_dir("out/03_colors");
    make_dir(outdir);

    int count = 0;
    count += write_solid(outdir, word, hex);    /* 1 */
    count += write_letters(outdir, word, hex);  /* 52 */
    count += write_ball(outdir, word, hex);     /* 1 */
    count += write_balls(outdir, word, hex);    /* 1 */
    count += write_shapes(outdir, word, hex);   /* 4 */

    printf("colour: %s   hex: %s   files: %d\n", word, hex, count);

    if (count != 59) {
        fprintf(stderr, "ERROR: expected 59 files, got %d\n", count);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
