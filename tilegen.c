/* tilegen.c — orange SVG tile generator, C99
 * Colour: #FF8833
 * Output: out/03_colors/orange/ (59 files)
 * Build:  cc -std=c99 -O2 -Wall tilegen.c -lm -o tilegen
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>

#define OUTDIR "out/03_colors/orange"
#define COL    "#FF8833"
#define WHITE  "#FFFFFF"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static int nfiles  = 0;
static int nerrors = 0;

/* ---- directory helper ---- */
static void mkdirp(const char *path)
{
    char tmp[512];
    size_t len = strlen(path);
    if (len >= sizeof(tmp)) {
        fprintf(stderr, "ERROR: path too long: %s\n", path);
        return;
    }
    memcpy(tmp, path, len + 1);
    for (size_t i = 1; i <= len; i++) {
        if (tmp[i] == '/' || tmp[i] == '\0') {
            char save = tmp[i];
            tmp[i] = '\0';
            mkdir(tmp, 0755);
            tmp[i] = save;
        }
    }
}

/* ---- open/close SVG ---- */
static FILE *open_tile(const char *group, const char *item)
{
    char path[512];
    snprintf(path, sizeof(path),
             "%s/03_colors_orange_%s_%s.svg", OUTDIR, group, item);
    FILE *f = fopen(path, "w");
    if (!f) {
        fprintf(stderr, "ERROR: cannot create %s\n", path);
        nerrors++;
        return NULL;
    }
    fprintf(f, "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 512 512\">\n");
    return f;
}

static void close_tile(FILE *f)
{
    if (!f) return;
    fprintf(f, "</svg>\n");
    fclose(f);
    nfiles++;
}

/* ---- canonical letter bitmaps (section 10) ---- */
/* 64 chars each, row-major 8x8, '1'=lit cell, '0'=empty */
static const char *UPPER[26] = {
    "0011100001000100010001000111110001000100010001000100010000000000", /* A */
    "0111100001000100010001000111100001000100010001000111100000000000", /* B */
    "0011110001000000010000000100000001000000010000000011110000000000", /* C */
    "0111100001000100010001000100010001000100010001000111100000000000", /* D */
    "0111110001000000010000000111100001000000010000000111110000000000", /* E */
    "0111110001000000010000000111100001000000010000000100000000000000", /* F */
    "0011110001000000010000000101110001000100010001000011110000000000", /* G */
    "0100010001000100010001000111110001000100010001000100010000000000", /* H */
    "0111110000010000000100000001000000010000000100000111110000000000", /* I */
    "0001110000001000000010000000100001001000010010000011000000000000", /* J */
    "0100010001001000010100000110000001010000010010000100010000000000", /* K */
    "0100000001000000010000000100000001000000010000000111110000000000", /* L */
    "0100010001101100010101000101010001000100010001000100010000000000", /* M */
    "0100010001100100010101000100110001000100010001000100010000000000", /* N */
    "0011100001000100010001000100010001000100010001000011100000000000", /* O */
    "0111100001000100010001000111100001000000010000000100000000000000", /* P */
    "0011100001000100010001000100010001010100010010000011010000000000", /* Q */
    "0111100001000100010001000111100001010000010010000100010000000000", /* R */
    "0011110001000000010000000011100000000100000001000111100000000000", /* S */
    "0111110000010000000100000001000000010000000100000001000000000000", /* T */
    "0100010001000100010001000100010001000100010001000011100000000000", /* U */
    "0100010001000100010001000100010001000100001010000001000000000000", /* V */
    "0100010001000100010001000101010001010100010101000010100000000000", /* W */
    "0100010001000100001010000001000000101000010001000100010000000000", /* X */
    "0100010001000100001010000001000000010000000100000001000000000000", /* Y */
    "0111110000000100000010000001000000100000010000000111110000000000", /* Z */
};

static const char *LOWER[26] = {
    "0000000000000000001110000000010000111100010001000011110000000000", /* a */
    "0100000001000000010110000110010001000100010001000111100000000000", /* b */
    "0000000000000000001111000100000001000000010000000011110000000000", /* c */
    "0000010000000100001101000100110001000100010001000011110000000000", /* d */
    "0000000000000000001110000100010001111100010000000011110000000000", /* e */
    "0001100000100100001000000111000000100000001000000010000000000000", /* f */
    "0000000000111100010001000100010000111100000001000111100000000000", /* g */
    "0100000001000000010110000110010001000100010001000100010000000000", /* h */
    "0001000000000000001100000001000000010000000100000011100000000000", /* i */
    "0000100000000000000110000000100000001000010010000011000000000000", /* j */
    "0100000001000000010010000101000001100000010100000100100000000000", /* k */
    "0011000000010000000100000001000000010000000100000011100000000000", /* l */
    "0000000000000000011010000101010001010100010101000101010000000000", /* m */
    "0000000000000000010110000110010001000100010001000100010000000000", /* n */
    "0000000000000000001110000100010001000100010001000011100000000000", /* o */
    "0000000001111000010001000100010001111000010000000100000000000000", /* p */
    "0000000000111100010001000100010000111100000001000000010000000000", /* q */
    "0000000000000000010110000110010001000000010000000100000000000000", /* r */
    "0000000000000000001111000100000000111000000001000111100000000000", /* s */
    "0010000000100000011100000010000000100000001001000001100000000000", /* t */
    "0000000000000000010001000100010001000100010011000011010000000000", /* u */
    "0000000000000000010001000100010001000100001010000001000000000000", /* v */
    "0000000000000000010001000100010001010100010101000010100000000000", /* w */
    "0000000000000000010001000010100000010000001010000100010000000000", /* x */
    "0000000001000100010001000100010000111100000001000111100000000000", /* y */
    "0000000000000000011111000000100000010000001000000111110000000000", /* z */
};

static const char ALPHA_UP[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char ALPHA_LO[] = "abcdefghijklmnopqrstuvwxyz";

/* ---- tile writers ---- */

static void tile_solid(void)
{
    FILE *f = open_tile("solid", "screen");
    if (!f) return;
    fprintf(f, "<rect width=\"512\" height=\"512\" fill=\"" COL "\"/>\n");
    close_tile(f);
}

static void tile_letter(const char *group, char name, const char *bitmap)
{
    char item[2] = { name, '\0' };
    FILE *f = open_tile(group, item);
    if (!f) return;
    fprintf(f, "<rect width=\"512\" height=\"512\" fill=\"" WHITE "\"/>\n");
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (bitmap[r * 8 + c] == '1') {
                fprintf(f,
                    "<rect x=\"%d\" y=\"%d\" width=\"40\" height=\"40\""
                    " fill=\"" COL "\"/>\n",
                    96 + c * 40, 96 + r * 40);
            }
        }
    }
    close_tile(f);
}

static void tile_ball(void)
{
    FILE *f = open_tile("ball", "one");
    if (!f) return;
    fprintf(f, "<rect width=\"512\" height=\"512\" fill=\"" WHITE "\"/>\n");
    fprintf(f, "<circle cx=\"256\" cy=\"256\" r=\"180\" fill=\"" COL "\"/>\n");
    close_tile(f);
}

static void tile_balls(void)
{
    FILE *f = open_tile("balls", "seven");
    if (!f) return;
    fprintf(f, "<rect width=\"512\" height=\"512\" fill=\"" WHITE "\"/>\n");
    /* center ball */
    fprintf(f, "<circle cx=\"256\" cy=\"256\" r=\"60\" fill=\"" COL "\"/>\n");
    /* six surrounding balls at radius 150, angles 0 60 120 180 240 300 */
    for (int i = 0; i < 6; i++) {
        double ang = i * M_PI / 3.0;
        int cx = (int)round(256.0 + 150.0 * cos(ang));
        int cy = (int)round(256.0 + 150.0 * sin(ang));
        fprintf(f, "<circle cx=\"%d\" cy=\"%d\" r=\"60\" fill=\"" COL "\"/>\n",
                cx, cy);
    }
    close_tile(f);
}

static void write_polygon(FILE *f, double cx, double cy,
                          double r, int n, double start_deg)
{
    fprintf(f, "<polygon points=\"");
    for (int i = 0; i < n; i++) {
        double ang = (start_deg + i * 360.0 / n) * M_PI / 180.0;
        int x = (int)round(cx + r * cos(ang));
        int y = (int)round(cy + r * sin(ang));
        if (i) fprintf(f, " ");
        fprintf(f, "%d,%d", x, y);
    }
    fprintf(f, "\" fill=\"" COL "\"/>\n");
}

static void tile_shapes(void)
{
    FILE *f;

    /* triangle: 3 vertices, first at -90, step 120 */
    f = open_tile("shapes", "triangle");
    if (f) {
        fprintf(f, "<rect width=\"512\" height=\"512\" fill=\"" WHITE "\"/>\n");
        write_polygon(f, 256, 256, 180, 3, -90);
        close_tile(f);
    }

    /* square: rect x=76 y=76 width=360 height=360 */
    f = open_tile("shapes", "square");
    if (f) {
        fprintf(f, "<rect width=\"512\" height=\"512\" fill=\"" WHITE "\"/>\n");
        fprintf(f, "<rect x=\"76\" y=\"76\" width=\"360\" height=\"360\""
                   " fill=\"" COL "\"/>\n");
        close_tile(f);
    }

    /* pentagon: 5 vertices, first at -90, step 72 */
    f = open_tile("shapes", "pentagon");
    if (f) {
        fprintf(f, "<rect width=\"512\" height=\"512\" fill=\"" WHITE "\"/>\n");
        write_polygon(f, 256, 256, 180, 5, -90);
        close_tile(f);
    }

    /* hexagon: 6 vertices, first at -90, step 60 */
    f = open_tile("shapes", "hexagon");
    if (f) {
        fprintf(f, "<rect width=\"512\" height=\"512\" fill=\"" WHITE "\"/>\n");
        write_polygon(f, 256, 256, 180, 6, -90);
        close_tile(f);
    }
}

/* ---- main ---- */
int main(void)
{
    mkdirp(OUTDIR);

    tile_solid();

    for (int i = 0; i < 26; i++)
        tile_letter("ABC", ALPHA_UP[i], UPPER[i]);

    for (int i = 0; i < 26; i++)
        tile_letter("abc", ALPHA_LO[i], LOWER[i]);

    tile_ball();
    tile_balls();
    tile_shapes();

    if (nfiles != 59) {
        fprintf(stderr, "ERROR: expected 59 files, wrote %d\n", nfiles);
        return 1;
    }
    if (nerrors) {
        fprintf(stderr, "ERROR: %d file(s) failed to write\n", nerrors);
        return 1;
    }

    printf("FILE COUNT: %d (must be 59)\n", nfiles);
    return 0;
}
