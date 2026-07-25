#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define COLOR "#FF8833"
#define WHITE "#FFFFFF"
#define CANVAS 512
#define GLYPH_ORIGIN 96
#define CELL 40
#define EXPECTED_FILE_COUNT 59
#define PI 3.14159265358979323846

typedef struct {
    const char *name;
    const char *rows[8];
} Glyph;

static const Glyph UPPERCASE_GLYPHS[] = {
    {"A", {"..###...", ".#...#..", ".#...#..", ".#####..", ".#...#..", ".#...#..", ".#...#..", "........"}},
    {"B", {".####...", ".#...#..", ".#...#..", ".####...", ".#...#..", ".#...#..", ".####...", "........"}},
    {"C", {"..####..", ".#......", ".#......", ".#......", ".#......", ".#......", "..####..", "........"}},
    {"D", {".####...", ".#...#..", ".#...#..", ".#...#..", ".#...#..", ".#...#..", ".####...", "........"}},
    {"E", {".#####..", ".#......", ".#......", ".####...", ".#......", ".#......", ".#####..", "........"}},
    {"F", {".#####..", ".#......", ".#......", ".####...", ".#......", ".#......", ".#......", "........"}},
    {"G", {"..####..", ".#......", ".#......", ".#.###..", ".#...#..", ".#...#..", "..####..", "........"}},
    {"H", {".#...#..", ".#...#..", ".#...#..", ".#####..", ".#...#..", ".#...#..", ".#...#..", "........"}},
    {"I", {".#####..", "...#....", "...#....", "...#....", "...#....", "...#....", ".#####..", "........"}},
    {"J", {"...###..", "....#...", "....#...", "....#...", ".#..#...", ".#..#...", "..##....", "........"}},
    {"K", {".#...#..", ".#..#...", ".#.#....", ".##.....", ".#.#....", ".#..#...", ".#...#..", "........"}},
    {"L", {".#......", ".#......", ".#......", ".#......", ".#......", ".#......", ".#####..", "........"}},
    {"M", {".#...#..", ".##.##..", ".#.#.#..", ".#.#.#..", ".#...#..", ".#...#..", ".#...#..", "........"}},
    {"N", {".#...#..", ".##..#..", ".#.#.#..", ".#..##..", ".#...#..", ".#...#..", ".#...#..", "........"}},
    {"O", {"..###...", ".#...#..", ".#...#..", ".#...#..", ".#...#..", ".#...#..", "..###...", "........"}},
    {"P", {".####...", ".#...#..", ".#...#..", ".####...", ".#......", ".#......", ".#......", "........"}},
    {"Q", {"..###...", ".#...#..", ".#...#..", ".#...#..", ".#.#.#..", ".#..#...", "..##.#..", "........"}},
    {"R", {".####...", ".#...#..", ".#...#..", ".####...", ".#.#....", ".#..#...", ".#...#..", "........"}},
    {"S", {"..####..", ".#......", ".#......", "..###...", ".....#..", ".....#..", ".####...", "........"}},
    {"T", {".#####..", "...#....", "...#....", "...#....", "...#....", "...#....", "...#....", "........"}},
    {"U", {".#...#..", ".#...#..", ".#...#..", ".#...#..", ".#...#..", ".#...#..", "..###...", "........"}},
    {"V", {".#...#..", ".#...#..", ".#...#..", ".#...#..", ".#...#..", "..#.#...", "...#....", "........"}},
    {"W", {".#...#..", ".#...#..", ".#...#..", ".#.#.#..", ".#.#.#..", ".#.#.#..", "..#.#...", "........"}},
    {"X", {".#...#..", ".#...#..", "..#.#...", "...#....", "..#.#...", ".#...#..", ".#...#..", "........"}},
    {"Y", {".#...#..", ".#...#..", "..#.#...", "...#....", "...#....", "...#....", "...#....", "........"}},
    {"Z", {".#####..", ".....#..", "....#...", "...#....", "..#.....", ".#......", ".#####..", "........"}},
};

static const Glyph LOWERCASE_GLYPHS[] = {
    {"a", {"........", "........", "..###...", ".....#..", "..####..", ".#...#..", "..####..", "........"}},
    {"b", {".#......", ".#......", ".#.##...", ".##..#..", ".#...#..", ".#...#..", ".####...", "........"}},
    {"c", {"........", "........", "..####..", ".#......", ".#......", ".#......", "..####..", "........"}},
    {"d", {".....#..", ".....#..", "..##.#..", ".#..##..", ".#...#..", ".#...#..", "..####..", "........"}},
    {"e", {"........", "........", "..###...", ".#...#..", ".#####..", ".#......", "..####..", "........"}},
    {"f", {"...##...", "..#..#..", "..#.....", ".###....", "..#.....", "..#.....", "..#.....", "........"}},
    {"g", {"........", "..####..", ".#...#..", ".#...#..", "..####..", ".....#..", ".####...", "........"}},
    {"h", {".#......", ".#......", ".#.##...", ".##..#..", ".#...#..", ".#...#..", ".#...#..", "........"}},
    {"i", {"...#....", "........", "..##....", "...#....", "...#....", "...#....", "..###...", "........"}},
    {"j", {"....#...", "........", "...##...", "....#...", "....#...", ".#..#...", "..##....", "........"}},
    {"k", {".#......", ".#......", ".#..#...", ".#.#....", ".##.....", ".#.#....", ".#..#...", "........"}},
    {"l", {"..##....", "...#....", "...#....", "...#....", "...#....", "...#....", "..###...", "........"}},
    {"m", {"........", "........", ".##.#...", ".#.#.#..", ".#.#.#..", ".#.#.#..", ".#.#.#..", "........"}},
    {"n", {"........", "........", ".#.##...", ".##..#..", ".#...#..", ".#...#..", ".#...#..", "........"}},
    {"o", {"........", "........", "..###...", ".#...#..", ".#...#..", ".#...#..", "..###...", "........"}},
    {"p", {"........", ".####...", ".#...#..", ".#...#..", ".####...", ".#......", ".#......", "........"}},
    {"q", {"........", "..####..", ".#...#..", ".#...#..", "..####..", ".....#..", ".....#..", "........"}},
    {"r", {"........", "........", ".#.##...", ".##..#..", ".#......", ".#......", ".#......", "........"}},
    {"s", {"........", "........", "..####..", ".#......", "..###...", ".....#..", ".####...", "........"}},
    {"t", {"..#.....", "..#.....", ".###....", "..#.....", "..#.....", "..#..#..", "...##...", "........"}},
    {"u", {"........", "........", ".#...#..", ".#...#..", ".#...#..", ".#..##..", "..##.#..", "........"}},
    {"v", {"........", "........", ".#...#..", ".#...#..", ".#...#..", "..#.#...", "...#....", "........"}},
    {"w", {"........", "........", ".#...#..", ".#...#..", ".#.#.#..", ".#.#.#..", "..#.#...", "........"}},
    {"x", {"........", "........", ".#...#..", "..#.#...", "...#....", "..#.#...", ".#...#..", "........"}},
    {"y", {"........", ".#...#..", ".#...#..", ".#...#..", "..####..", ".....#..", ".####...", "........"}},
    {"z", {"........", "........", ".#####..", "....#...", "...#....", "..#.....", ".#####..", "........"}},
};

static int created_files = 0;

static bool ensure_dir(const char *path) {
    if (mkdir(path, 0777) == 0) {
        return true;
    }
    if (errno == EEXIST) {
        return true;
    }
    perror(path);
    return false;
}

static FILE *open_svg(const char *path) {
    FILE *fp = fopen(path, "w");
    if (!fp) {
        perror(path);
        return NULL;
    }
    fputs("<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"512\" height=\"512\" viewBox=\"0 0 512 512\">\n", fp);
    return fp;
}

static void close_svg(FILE *fp) {
    fputs("</svg>\n", fp);
    fclose(fp);
}

static void write_bg(FILE *fp) {
    fprintf(fp, "<rect width=\"%d\" height=\"%d\" fill=\"%s\"/>\n", CANVAS, CANVAS, WHITE);
}

static bool write_solid_screen(const char *path) {
    FILE *fp = open_svg(path);
    if (!fp) {
        return false;
    }
    fprintf(fp, "<rect width=\"%d\" height=\"%d\" fill=\"%s\"/>\n", CANVAS, CANVAS, COLOR);
    close_svg(fp);
    ++created_files;
    return true;
}

static bool write_letter(const char *path, const Glyph *glyph) {
    FILE *fp = open_svg(path);
    if (!fp) {
        return false;
    }
    write_bg(fp);

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (glyph->rows[r][c] == '#') {
                int x = GLYPH_ORIGIN + c * CELL;
                int y = GLYPH_ORIGIN + r * CELL;
                fprintf(fp, "<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" fill=\"%s\"/>\n", x, y, CELL, CELL, COLOR);
            }
        }
    }

    close_svg(fp);
    ++created_files;
    return true;
}

static bool write_one_ball(const char *path) {
    FILE *fp = open_svg(path);
    if (!fp) {
        return false;
    }
    write_bg(fp);
    fprintf(fp, "<circle cx=\"256\" cy=\"256\" r=\"60\" fill=\"%s\"/>\n", COLOR);
    close_svg(fp);
    ++created_files;
    return true;
}

static bool write_seven_balls(const char *path) {
    FILE *fp = open_svg(path);
    if (!fp) {
        return false;
    }

    write_bg(fp);
    fprintf(fp, "<circle cx=\"256\" cy=\"256\" r=\"60\" fill=\"%s\"/>\n", COLOR);

    for (int deg = 0; deg < 360; deg += 60) {
        double rad = (double)deg * PI / 180.0;
        long x = lround(256.0 + 150.0 * cos(rad));
        long y = lround(256.0 + 150.0 * sin(rad));
        fprintf(fp, "<circle cx=\"%ld\" cy=\"%ld\" r=\"60\" fill=\"%s\"/>\n", x, y, COLOR);
    }

    close_svg(fp);
    ++created_files;
    return true;
}

static bool write_polygon_shape(const char *path, int sides, int start_angle_deg) {
    FILE *fp = open_svg(path);
    if (!fp) {
        return false;
    }

    write_bg(fp);
    fputs("<polygon points=\"", fp);
    for (int i = 0; i < sides; ++i) {
        int deg = start_angle_deg + i * (360 / sides);
        double rad = (double)deg * PI / 180.0;
        long x = lround(256.0 + 180.0 * cos(rad));
        long y = lround(256.0 + 180.0 * sin(rad));
        fprintf(fp, "%ld,%ld", x, y);
        if (i < sides - 1) {
            fputc(' ', fp);
        }
    }
    fprintf(fp, "\" fill=\"%s\"/>\n", COLOR);

    close_svg(fp);
    ++created_files;
    return true;
}

static bool write_square_shape(const char *path) {
    FILE *fp = open_svg(path);
    if (!fp) {
        return false;
    }

    write_bg(fp);
    fprintf(fp, "<rect x=\"76\" y=\"76\" width=\"360\" height=\"360\" fill=\"%s\"/>\n", COLOR);

    close_svg(fp);
    ++created_files;
    return true;
}

int main(void) {
    const char *base = "out/03_colors/orange";
    char path[256];

    if (!ensure_dir("out") || !ensure_dir("out/03_colors") || !ensure_dir(base)) {
        return 1;
    }

    snprintf(path, sizeof(path), "%s/03_colors_orange_solid_screen.svg", base);
    if (!write_solid_screen(path)) {
        return 1;
    }

    for (size_t i = 0; i < sizeof(UPPERCASE_GLYPHS) / sizeof(UPPERCASE_GLYPHS[0]); ++i) {
        snprintf(path, sizeof(path), "%s/03_colors_orange_ABC_%s.svg", base, UPPERCASE_GLYPHS[i].name);
        if (!write_letter(path, &UPPERCASE_GLYPHS[i])) {
            return 1;
        }
    }

    for (size_t i = 0; i < sizeof(LOWERCASE_GLYPHS) / sizeof(LOWERCASE_GLYPHS[0]); ++i) {
        snprintf(path, sizeof(path), "%s/03_colors_orange_abc_%s.svg", base, LOWERCASE_GLYPHS[i].name);
        if (!write_letter(path, &LOWERCASE_GLYPHS[i])) {
            return 1;
        }
    }

    snprintf(path, sizeof(path), "%s/03_colors_orange_ball_one.svg", base);
    if (!write_one_ball(path)) {
        return 1;
    }

    snprintf(path, sizeof(path), "%s/03_colors_orange_balls_seven.svg", base);
    if (!write_seven_balls(path)) {
        return 1;
    }

    snprintf(path, sizeof(path), "%s/03_colors_orange_shapes_triangle.svg", base);
    if (!write_polygon_shape(path, 3, -90)) {
        return 1;
    }

    snprintf(path, sizeof(path), "%s/03_colors_orange_shapes_square.svg", base);
    if (!write_square_shape(path)) {
        return 1;
    }

    snprintf(path, sizeof(path), "%s/03_colors_orange_shapes_pentagon.svg", base);
    if (!write_polygon_shape(path, 5, -90)) {
        return 1;
    }

    snprintf(path, sizeof(path), "%s/03_colors_orange_shapes_hexagon.svg", base);
    if (!write_polygon_shape(path, 6, -90)) {
        return 1;
    }

    printf("FILE COUNT: %d\n", created_files);
    if (created_files != EXPECTED_FILE_COUNT) {
        fprintf(stderr, "ERROR: expected %d files, got %d\n", EXPECTED_FILE_COUNT, created_files);
        return 1;
    }

    return 0;
}
