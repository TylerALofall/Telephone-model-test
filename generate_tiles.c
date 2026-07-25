/* generate_tiles.c — Colour + Letter + Shape SVG tile generator
 * COLOUR CHOSEN: blue #0000FF
 *
 * C99, standard library + POSIX mkdir.
 * Generates 58 SVG files in the specified output directory (default: 03_colors_blue).
 *
 * Usage: ./generate_tiles [output_dir]
 *
 * Files produced:
 *   03_colors_blue_solid_screen.svg          (1)
 *   03_colors_blue_ABC_Letter_A.svg … Z.svg  (26)
 *   03_colors_blue_abc_Letter_a.svg … z.svg  (26)
 *   03_colors_blue_ball_ball.svg             (1)
 *   03_colors_blue_shapes_triangle.svg       (1)
 *   03_colors_blue_shapes_square.svg         (1)
 *   03_colors_blue_shapes_pentagon.svg       (1)
 *   03_colors_blue_shapes_hexagon.svg        (1)
 *   Total: 58 SVG files
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>

#define COLOR_HEX  "#0000FF"
#define COLOR_NAME "blue"
#define PI         3.14159265358979323846

/* ---------------------------------------------------------------
 * 8×8 bitmap font data.
 * Each entry is 8 bytes, one per row (top to bottom).
 * Within each byte, bit 7 (MSB) = leftmost column (col 0),
 * bit 0 (LSB) = rightmost column (col 7).
 * A set bit means a coloured pixel; clear bit means white.
 * --------------------------------------------------------------- */

/* Uppercase A–Z */
static const unsigned char font_upper[26][8] = {
    /* A */ {0x38, 0x44, 0x44, 0x7C, 0x44, 0x44, 0x44, 0x00},
    /* B */ {0x78, 0x44, 0x44, 0x78, 0x44, 0x44, 0x78, 0x00},
    /* C */ {0x38, 0x44, 0x40, 0x40, 0x40, 0x44, 0x38, 0x00},
    /* D */ {0x78, 0x44, 0x44, 0x44, 0x44, 0x44, 0x78, 0x00},
    /* E */ {0x7C, 0x40, 0x40, 0x78, 0x40, 0x40, 0x7C, 0x00},
    /* F */ {0x7C, 0x40, 0x40, 0x78, 0x40, 0x40, 0x40, 0x00},
    /* G */ {0x38, 0x44, 0x40, 0x4C, 0x44, 0x44, 0x38, 0x00},
    /* H */ {0x44, 0x44, 0x44, 0x7C, 0x44, 0x44, 0x44, 0x00},
    /* I */ {0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7C, 0x00},
    /* J */ {0x3C, 0x08, 0x08, 0x08, 0x08, 0x48, 0x38, 0x00},
    /* K */ {0x44, 0x48, 0x50, 0x60, 0x50, 0x48, 0x44, 0x00},
    /* L */ {0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x7C, 0x00},
    /* M */ {0x44, 0x6C, 0x54, 0x44, 0x44, 0x44, 0x44, 0x00},
    /* N */ {0x44, 0x64, 0x54, 0x4C, 0x44, 0x44, 0x44, 0x00},
    /* O */ {0x38, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00},
    /* P */ {0x78, 0x44, 0x44, 0x78, 0x40, 0x40, 0x40, 0x00},
    /* Q */ {0x38, 0x44, 0x44, 0x44, 0x54, 0x4C, 0x3C, 0x00},
    /* R */ {0x78, 0x44, 0x44, 0x78, 0x50, 0x48, 0x44, 0x00},
    /* S */ {0x38, 0x44, 0x40, 0x38, 0x04, 0x44, 0x38, 0x00},
    /* T */ {0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},
    /* U */ {0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00},
    /* V */ {0x44, 0x44, 0x44, 0x44, 0x44, 0x28, 0x10, 0x00},
    /* W */ {0x44, 0x44, 0x44, 0x54, 0x54, 0x6C, 0x44, 0x00},
    /* X */ {0x44, 0x44, 0x28, 0x10, 0x28, 0x44, 0x44, 0x00},
    /* Y */ {0x44, 0x44, 0x28, 0x10, 0x10, 0x10, 0x10, 0x00},
    /* Z */ {0x7C, 0x04, 0x08, 0x10, 0x20, 0x40, 0x7C, 0x00},
};

/* Lowercase a–z */
static const unsigned char font_lower[26][8] = {
    /* a */ {0x00, 0x00, 0x38, 0x04, 0x3C, 0x44, 0x3C, 0x00},
    /* b */ {0x40, 0x40, 0x78, 0x44, 0x44, 0x44, 0x78, 0x00},
    /* c */ {0x00, 0x00, 0x38, 0x44, 0x40, 0x44, 0x38, 0x00},
    /* d */ {0x04, 0x04, 0x3C, 0x44, 0x44, 0x44, 0x3C, 0x00},
    /* e */ {0x00, 0x00, 0x38, 0x44, 0x7C, 0x40, 0x38, 0x00},
    /* f */ {0x18, 0x20, 0x70, 0x20, 0x20, 0x20, 0x20, 0x00},
    /* g */ {0x00, 0x00, 0x3C, 0x44, 0x44, 0x3C, 0x04, 0x38},
    /* h */ {0x40, 0x40, 0x78, 0x44, 0x44, 0x44, 0x44, 0x00},
    /* i */ {0x10, 0x00, 0x30, 0x10, 0x10, 0x10, 0x38, 0x00},
    /* j */ {0x08, 0x00, 0x08, 0x08, 0x08, 0x48, 0x30, 0x00},
    /* k */ {0x40, 0x40, 0x48, 0x50, 0x60, 0x50, 0x48, 0x00},
    /* l */ {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x30, 0x00},
    /* m */ {0x00, 0x00, 0x6C, 0x54, 0x54, 0x54, 0x54, 0x00},
    /* n */ {0x00, 0x00, 0x78, 0x44, 0x44, 0x44, 0x44, 0x00},
    /* o */ {0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x38, 0x00},
    /* p */ {0x00, 0x00, 0x78, 0x44, 0x44, 0x78, 0x40, 0x40},
    /* q */ {0x00, 0x00, 0x3C, 0x44, 0x44, 0x3C, 0x04, 0x04},
    /* r */ {0x00, 0x00, 0x58, 0x60, 0x40, 0x40, 0x40, 0x00},
    /* s */ {0x00, 0x00, 0x38, 0x40, 0x38, 0x04, 0x38, 0x00},
    /* t */ {0x20, 0x20, 0x70, 0x20, 0x20, 0x20, 0x18, 0x00},
    /* u */ {0x00, 0x00, 0x44, 0x44, 0x44, 0x44, 0x3C, 0x00},
    /* v */ {0x00, 0x00, 0x44, 0x44, 0x44, 0x28, 0x10, 0x00},
    /* w */ {0x00, 0x00, 0x44, 0x44, 0x54, 0x54, 0x28, 0x00},
    /* x */ {0x00, 0x00, 0x44, 0x28, 0x10, 0x28, 0x44, 0x00},
    /* y */ {0x00, 0x00, 0x44, 0x44, 0x44, 0x3C, 0x04, 0x38},
    /* z */ {0x00, 0x00, 0x7C, 0x08, 0x10, 0x20, 0x7C, 0x00},
};

/* ---------------------------------------------------------------
 * SVG helpers
 * --------------------------------------------------------------- */

static void svg_header(FILE *f) {
    fprintf(f,
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<svg xmlns=\"http://www.w3.org/2000/svg\""
        " viewBox=\"0 0 512 512\" width=\"512\" height=\"512\">\n");
}

static void svg_footer(FILE *f) {
    fprintf(f, "</svg>\n");
}

/* Open a file in outdir, abort on error. */
static FILE *open_svg(const char *outdir, const char *name) {
    char path[512];
    snprintf(path, sizeof(path), "%s/%s", outdir, name);
    FILE *f = fopen(path, "w");
    if (!f) {
        perror(path);
        exit(EXIT_FAILURE);
    }
    return f;
}

/* ---------------------------------------------------------------
 * Tile generators
 * --------------------------------------------------------------- */

/* 1. Solid colour screen */
static void write_solid_screen(const char *outdir) {
    FILE *f = open_svg(outdir, "03_colors_" COLOR_NAME "_solid_screen.svg");
    svg_header(f);
    fprintf(f, "  <rect x=\"0\" y=\"0\" width=\"512\" height=\"512\""
               " fill=\"" COLOR_HEX "\"/>\n");
    svg_footer(f);
    fclose(f);
}

/* 2. Letter tile (uppercase or lowercase).
 *    The 8×8 glyph (each cell = 8 SVG units → 64×64 block) is
 *    centred on the 512×512 canvas: offset = (512-64)/2 = 224.
 */
static void write_letter(const char *outdir,
                          char ch,
                          const unsigned char bitmap[8],
                          int is_upper) {
    char name[128];
    const char *group = is_upper ? "ABC" : "abc";
    snprintf(name, sizeof(name),
             "03_colors_" COLOR_NAME "_%s_Letter_%c.svg", group, ch);

    FILE *f = open_svg(outdir, name);
    svg_header(f);
    /* White background */
    fprintf(f, "  <rect x=\"0\" y=\"0\" width=\"512\" height=\"512\""
               " fill=\"white\"/>\n");

    /* Glyph origin offset so the 64×64 block is centred */
    int ox = 224, oy = 224;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if ((bitmap[row] >> (7 - col)) & 1) {
                fprintf(f,
                    "  <rect x=\"%d\" y=\"%d\" width=\"8\" height=\"8\""
                    " fill=\"" COLOR_HEX "\"/>\n",
                    ox + col * 8, oy + row * 8);
            }
        }
    }
    svg_footer(f);
    fclose(f);
}

/* 3. Ball */
static void write_ball(const char *outdir) {
    FILE *f = open_svg(outdir, "03_colors_" COLOR_NAME "_ball_ball.svg");
    svg_header(f);
    fprintf(f, "  <rect x=\"0\" y=\"0\" width=\"512\" height=\"512\""
               " fill=\"white\"/>\n");
    fprintf(f, "  <circle cx=\"256\" cy=\"256\" r=\"200\""
               " fill=\"" COLOR_HEX "\"/>\n");
    svg_footer(f);
    fclose(f);
}

/* Helper: write a regular n-gon centred at (cx,cy) with circumradius r.
 * The first vertex is at angle start_angle_deg from the positive x-axis. */
static void write_polygon_svg(const char *outdir,
                               const char *filename,
                               double cx, double cy,
                               int n,
                               double radius,
                               double start_angle_deg) {
    FILE *f = open_svg(outdir, filename);
    svg_header(f);
    fprintf(f, "  <rect x=\"0\" y=\"0\" width=\"512\" height=\"512\""
               " fill=\"white\"/>\n");
    fprintf(f, "  <polygon points=\"");
    for (int i = 0; i < n; i++) {
        double angle = (start_angle_deg + i * 360.0 / n) * PI / 180.0;
        double x = cx + radius * cos(angle);
        double y = cy + radius * sin(angle);
        if (i > 0) fprintf(f, " ");
        fprintf(f, "%.3f,%.3f", x, y);
    }
    fprintf(f, "\" fill=\"" COLOR_HEX "\"/>\n");
    svg_footer(f);
    fclose(f);
}

/* 4. Shapes */
static void write_shapes(const char *outdir) {
    /* Triangle: equilateral, first vertex pointing up (-90°) */
    write_polygon_svg(outdir,
        "03_colors_" COLOR_NAME "_shapes_triangle.svg",
        256, 256, 3, 220.0, -90.0);

    /* Square: use SVG rect for pixel-perfect edges */
    {
        FILE *f = open_svg(outdir,
                           "03_colors_" COLOR_NAME "_shapes_square.svg");
        svg_header(f);
        fprintf(f, "  <rect x=\"0\" y=\"0\" width=\"512\" height=\"512\""
                   " fill=\"white\"/>\n");
        /* 400×400 square centred at (256,256) */
        fprintf(f, "  <rect x=\"56\" y=\"56\" width=\"400\" height=\"400\""
                   " fill=\"" COLOR_HEX "\"/>\n");
        svg_footer(f);
        fclose(f);
    }

    /* Pentagon: first vertex pointing up (-90°) */
    write_polygon_svg(outdir,
        "03_colors_" COLOR_NAME "_shapes_pentagon.svg",
        256, 256, 5, 210.0, -90.0);

    /* Hexagon: flat-top orientation (first vertex at 0°) */
    write_polygon_svg(outdir,
        "03_colors_" COLOR_NAME "_shapes_hexagon.svg",
        256, 256, 6, 210.0, 0.0);
}

/* ---------------------------------------------------------------
 * main
 * --------------------------------------------------------------- */

int main(int argc, char *argv[]) {
    const char *outdir = (argc > 1) ? argv[1] : "03_colors_blue";

    /* Create output directory (no-op if it already exists) */
    mkdir(outdir, 0755);

    /* 1. Solid screen */
    write_solid_screen(outdir);

    /* 2. Uppercase letters A–Z */
    for (int i = 0; i < 26; i++) {
        write_letter(outdir, (char)('A' + i), font_upper[i], 1);
    }

    /* 3. Lowercase letters a–z */
    for (int i = 0; i < 26; i++) {
        write_letter(outdir, (char)('a' + i), font_lower[i], 0);
    }

    /* 4. Ball */
    write_ball(outdir);

    /* 5. Shapes */
    write_shapes(outdir);

    printf("Generated 58 SVG files in '%s/'\n", outdir);
    printf("COLOUR CHOSEN: blue #0000FF\n");
    return 0;
}
