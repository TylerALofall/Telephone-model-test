/*
 * tilegen.c — SVG tile generator, RED (#ED0A3F), C99
 *
 * Writes 59 SVG files to out/03_colors/red/
 *   1 solid screen
 *  26 uppercase letters (group ABC)
 *  26 lowercase letters (group abc)
 *   1 ball (one)
 *   1 seven-balls (seven)
 *   4 shapes (triangle, square, pentagon, hexagon)
 *
 * Build:  cc -std=c99 -o tilegen tilegen.c -lm
 * Run:    ./tilegen
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* -------- constants -------- */
#define COLOR    "#ED0A3F"
#define WHITE    "#FFFFFF"
#define CELL     40          /* glyph cell size in pixels          */
#define GCOLS    8           /* glyph grid columns                 */
#define GROWS    8           /* glyph grid rows                    */
#define GOFF     96          /* (512 - 8*40)/2 = 96                */
#define OUT_DIR  "out/03_colors/red"
#define PREFIX   "03_colors_red"
#define SVG_HDR  "<svg xmlns=\"http://www.w3.org/2000/svg\" " \
                 "width=\"512\" height=\"512\" viewBox=\"0 0 512 512\">"

static int file_count = 0;

/* -------- directory helpers -------- */
static void mkdir_p(const char *path)
{
    char tmp[PATH_MAX];
    char *p;
    int n = snprintf(tmp, sizeof(tmp), "%s", path);
    if (n < 0 || (size_t)n >= sizeof(tmp)) {
        fprintf(stderr, "ERROR: path too long: %s\n", path);
        exit(1);
    }
    for (p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            mkdir(tmp, 0755);
            *p = '/';
        }
    }
    mkdir(tmp, 0755);
}

/* -------- file open/close -------- */
static FILE *open_tile(const char *leaf)
{
    char path[PATH_MAX];
    int n = snprintf(path, sizeof(path), "%s/%s_%s.svg", OUT_DIR, PREFIX, leaf);
    if (n < 0 || (size_t)n >= sizeof(path)) {
        fprintf(stderr, "ERROR: path too long for leaf: %s\n", leaf);
        exit(1);
    }
    FILE *f = fopen(path, "w");
    if (!f) { perror(path); exit(1); }
    fprintf(f, "%s\n", SVG_HDR);
    return f;
}

static void close_tile(FILE *f)
{
    fprintf(f, "</svg>\n");
    fclose(f);
    file_count++;
}

/* -------- solid screen -------- */
static void write_solid(void)
{
    FILE *f = open_tile("solid_screen");
    fprintf(f, "<rect width=\"512\" height=\"512\" fill=\"%s\"/>\n", COLOR);
    close_tile(f);
}

/* ================================================================
 * 8x8 glyph bitmaps
 *
 * Each entry is 8 bytes, one per row (top to bottom).
 * Within a byte: bit 7 = column 0 (leftmost),
 *                bit 0 = column 7 (rightmost).
 * A 1-bit means that cell is lit (colour on white background).
 * ================================================================ */

static const unsigned char UPPER[26][8] = {
    /* A */ {0x3C,0x42,0x42,0x7E,0x42,0x42,0x42,0x00},
    /* B */ {0x7C,0x42,0x42,0x7C,0x42,0x42,0x7C,0x00},
    /* C */ {0x3C,0x42,0x40,0x40,0x40,0x42,0x3C,0x00},
    /* D */ {0x7C,0x42,0x41,0x41,0x41,0x42,0x7C,0x00},
    /* E */ {0x7E,0x40,0x40,0x7C,0x40,0x40,0x7E,0x00},
    /* F */ {0x7E,0x40,0x40,0x7C,0x40,0x40,0x40,0x00},
    /* G */ {0x3C,0x42,0x40,0x4E,0x42,0x42,0x3C,0x00},
    /* H */ {0x42,0x42,0x42,0x7E,0x42,0x42,0x42,0x00},
    /* I */ {0x7E,0x18,0x18,0x18,0x18,0x18,0x7E,0x00},
    /* J */ {0x0E,0x04,0x04,0x04,0x44,0x44,0x38,0x00},
    /* K */ {0x42,0x44,0x48,0x70,0x48,0x44,0x42,0x00},
    /* L */ {0x40,0x40,0x40,0x40,0x40,0x40,0x7E,0x00},
    /* M */ {0x42,0x66,0x5A,0x42,0x42,0x42,0x42,0x00},
    /* N */ {0x42,0x62,0x52,0x4A,0x46,0x42,0x42,0x00},
    /* O */ {0x3C,0x42,0x42,0x42,0x42,0x42,0x3C,0x00},
    /* P */ {0x7C,0x42,0x42,0x7C,0x40,0x40,0x40,0x00},
    /* Q */ {0x3C,0x42,0x42,0x42,0x4A,0x44,0x3A,0x00},
    /* R */ {0x7C,0x42,0x42,0x7C,0x48,0x44,0x42,0x00},
    /* S */ {0x3C,0x42,0x40,0x3C,0x02,0x42,0x3C,0x00},
    /* T */ {0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x00},
    /* U */ {0x42,0x42,0x42,0x42,0x42,0x42,0x3C,0x00},
    /* V */ {0x42,0x42,0x42,0x42,0x42,0x24,0x18,0x00},
    /* W */ {0x42,0x42,0x42,0x42,0x5A,0x66,0x42,0x00},
    /* X */ {0x42,0x42,0x24,0x18,0x24,0x42,0x42,0x00},
    /* Y */ {0x42,0x42,0x24,0x18,0x18,0x18,0x18,0x00},
    /* Z */ {0x7E,0x04,0x08,0x10,0x20,0x40,0x7E,0x00},
};

static const unsigned char LOWER[26][8] = {
    /* a */ {0x00,0x3C,0x42,0x02,0x3E,0x42,0x3E,0x00},
    /* b */ {0x40,0x40,0x7C,0x42,0x42,0x42,0x7C,0x00},
    /* c */ {0x00,0x3C,0x42,0x40,0x40,0x42,0x3C,0x00},
    /* d */ {0x02,0x02,0x3E,0x42,0x42,0x42,0x3E,0x00},
    /* e */ {0x00,0x3C,0x42,0x7E,0x40,0x42,0x3C,0x00},
    /* f */ {0x1C,0x20,0x7C,0x20,0x20,0x20,0x20,0x00},
    /* g */ {0x00,0x3E,0x42,0x42,0x3E,0x02,0x3C,0x00},
    /* h */ {0x40,0x40,0x7C,0x42,0x42,0x42,0x42,0x00},
    /* i */ {0x18,0x00,0x38,0x10,0x10,0x10,0x38,0x00},
    /* j */ {0x06,0x00,0x06,0x04,0x04,0x44,0x38,0x00},
    /* k */ {0x40,0x40,0x44,0x48,0x70,0x48,0x44,0x00},
    /* l */ {0x38,0x10,0x10,0x10,0x10,0x10,0x38,0x00},
    /* m */ {0x00,0xD8,0xA8,0xA8,0xA8,0xA8,0xA8,0x00},
    /* n */ {0x00,0x7E,0x42,0x42,0x42,0x42,0x42,0x00},
    /* o */ {0x00,0x3C,0x42,0x42,0x42,0x42,0x3C,0x00},
    /* p */ {0x00,0x7C,0x42,0x42,0x7C,0x40,0x40,0x00},
    /* q */ {0x00,0x3E,0x42,0x42,0x3E,0x02,0x02,0x00},
    /* r */ {0x00,0x74,0x40,0x40,0x40,0x40,0x40,0x00},
    /* s */ {0x00,0x3C,0x42,0x3C,0x02,0x42,0x3C,0x00},
    /* t */ {0x10,0x10,0x7C,0x10,0x10,0x10,0x0E,0x00},
    /* u */ {0x00,0x42,0x42,0x42,0x42,0x42,0x3E,0x00},
    /* v */ {0x00,0x42,0x42,0x42,0x24,0x24,0x18,0x00},
    /* w */ {0x00,0x42,0x42,0x42,0x5A,0x66,0x42,0x00},
    /* x */ {0x00,0x42,0x24,0x18,0x18,0x24,0x42,0x00},
    /* y */ {0x00,0x42,0x42,0x42,0x3E,0x02,0x3C,0x00},
    /* z */ {0x00,0x7E,0x04,0x08,0x10,0x20,0x7E,0x00},
};

/* -------- write one letter tile -------- */
static void write_letter(const char *group, const char *ch,
                         const unsigned char bits[8])
{
    char leaf[64];
    int n = snprintf(leaf, sizeof(leaf), "%s_%s", group, ch);
    if (n < 0 || (size_t)n >= sizeof(leaf)) {
        fprintf(stderr, "ERROR: leaf name too long: %s_%s\n", group, ch);
        exit(1);
    }
    FILE *f = open_tile(leaf);
    fprintf(f, "<rect width=\"512\" height=\"512\" fill=\"%s\"/>\n", WHITE);
    for (int r = 0; r < GROWS; r++) {
        for (int c = 0; c < GCOLS; c++) {
            if (bits[r] & (0x80u >> c)) {
                fprintf(f,
                    "<rect x=\"%d\" y=\"%d\" "
                    "width=\"%d\" height=\"%d\" fill=\"%s\"/>\n",
                    GOFF + c * CELL, GOFF + r * CELL,
                    CELL, CELL, COLOR);
            }
        }
    }
    close_tile(f);
}

static void write_letters(void)
{
    char ch[2];
    for (int i = 0; i < 26; i++) {
        ch[0] = (char)('A' + i); ch[1] = '\0';
        write_letter("ABC", ch, UPPER[i]);
    }
    for (int i = 0; i < 26; i++) {
        ch[0] = (char)('a' + i); ch[1] = '\0';
        write_letter("abc", ch, LOWER[i]);
    }
}

/* -------- ball -------- */
static void write_ball(void)
{
    FILE *f = open_tile("ball_one");
    fprintf(f, "<rect width=\"512\" height=\"512\" fill=\"%s\"/>\n", WHITE);
    fprintf(f, "<circle cx=\"256\" cy=\"256\" r=\"200\" fill=\"%s\"/>\n", COLOR);
    close_tile(f);
}

/* -------- seven balls -------- */
static void write_seven_balls(void)
{
    FILE *f = open_tile("balls_seven");
    fprintf(f, "<rect width=\"512\" height=\"512\" fill=\"%s\"/>\n", WHITE);
    /* centre */
    fprintf(f, "<circle cx=\"256\" cy=\"256\" r=\"60\" fill=\"%s\"/>\n", COLOR);
    /* six outer: angles 0, 60, 120, 180, 240, 300 degrees */
    int angles[6] = {0, 60, 120, 180, 240, 300};
    for (int i = 0; i < 6; i++) {
        double rad = angles[i] * M_PI / 180.0;
        int cx = (int)round(256.0 + 150.0 * cos(rad));
        int cy = (int)round(256.0 + 150.0 * sin(rad));
        fprintf(f, "<circle cx=\"%d\" cy=\"%d\" r=\"60\" fill=\"%s\"/>\n",
                cx, cy, COLOR);
    }
    close_tile(f);
}

/* -------- shapes -------- */
static void write_polygon(FILE *f, int n, double r,
                          double start_deg, double step_deg)
{
    fprintf(f, "<polygon points=\"");
    for (int i = 0; i < n; i++) {
        double rad = (start_deg + i * step_deg) * M_PI / 180.0;
        int x = (int)round(256.0 + r * cos(rad));
        int y = (int)round(256.0 + r * sin(rad));
        if (i > 0) fputc(' ', f);
        fprintf(f, "%d,%d", x, y);
    }
    fprintf(f, "\" fill=\"%s\"/>\n", COLOR);
}

static void write_shapes(void)
{
    /* triangle: 3 vertices, first at -90 deg, step 120 */
    {
        FILE *f = open_tile("shapes_triangle");
        fprintf(f, "<rect width=\"512\" height=\"512\" fill=\"%s\"/>\n", WHITE);
        write_polygon(f, 3, 180.0, -90.0, 120.0);
        close_tile(f);
    }
    /* square: axis-aligned rect 360x360 centred */
    {
        FILE *f = open_tile("shapes_square");
        fprintf(f, "<rect width=\"512\" height=\"512\" fill=\"%s\"/>\n", WHITE);
        fprintf(f,
            "<rect x=\"76\" y=\"76\" width=\"360\" height=\"360\" "
            "fill=\"%s\"/>\n", COLOR);
        close_tile(f);
    }
    /* pentagon: 5 vertices, first at -90 deg, step 72 */
    {
        FILE *f = open_tile("shapes_pentagon");
        fprintf(f, "<rect width=\"512\" height=\"512\" fill=\"%s\"/>\n", WHITE);
        write_polygon(f, 5, 180.0, -90.0, 72.0);
        close_tile(f);
    }
    /* hexagon: 6 vertices, first at -90 deg, step 60 */
    {
        FILE *f = open_tile("shapes_hexagon");
        fprintf(f, "<rect width=\"512\" height=\"512\" fill=\"%s\"/>\n", WHITE);
        write_polygon(f, 6, 180.0, -90.0, 60.0);
        close_tile(f);
    }
}

/* -------- main -------- */
int main(void)
{
    mkdir_p(OUT_DIR);

    write_solid();        /* 1  */
    write_letters();      /* 52 */
    write_ball();         /* 1  */
    write_seven_balls();  /* 1  */
    write_shapes();       /* 4  */

    printf("File count: %d\n", file_count);
    if (file_count != 59) {
        fprintf(stderr, "ERROR: expected 59 files, got %d\n", file_count);
        return 1;
    }
    printf("All 59 files written to %s/\n", OUT_DIR);
    return 0;
}
