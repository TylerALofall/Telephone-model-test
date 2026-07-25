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
#define GLYPH_ROWS 8
#define GLYPH_COLS 8
#define MAX_PATH_LEN 512
#define PI 3.14159265358979323846
#define SOLID_TILE_COUNT 1
#define BALL_TILE_COUNT 1
#define BALLS_TILE_COUNT 1
#define SHAPE_TILE_COUNT 4

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

static int expected_file_count(void) {
    int uppercase_count = (int)(sizeof(UPPERCASE_GLYPHS) / sizeof(UPPERCASE_GLYPHS[0]));
    int lowercase_count = (int)(sizeof(LOWERCASE_GLYPHS) / sizeof(LOWERCASE_GLYPHS[0]));
    return SOLID_TILE_COUNT + uppercase_count + lowercase_count + BALL_TILE_COUNT + BALLS_TILE_COUNT + SHAPE_TILE_COUNT;
}

static bool ensure_dir(const char *path) {
    if (mkdir(path, 0755) == 0) {
        return true;
    }
    if (errno == EEXIST) {
        return true;
    }
    perror(path);
    return false;
}

static bool build_path(char *dest, size_t dest_size, const char *format, const char *output_dir, const char *filename) {
    int written = snprintf(dest, dest_size, format, output_dir, filename);
    if (written < 0 || (size_t)written >= dest_size) {
        fprintf(stderr, "ERROR: path too long for buffer\n");
        return false;
    }
    return true;
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

    for (int r = 0; r < GLYPH_ROWS; ++r) {
        for (int c = 0; c < GLYPH_COLS; ++c) {
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
        double rad = ((double)deg * PI) / 180.0;
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
        double rad = ((double)deg * PI) / 180.0;
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
    const char *output_dir = "out/03_colors/orange";
    char path[MAX_PATH_LEN];

    if (!ensure_dir("out") || !ensure_dir("out/03_colors") || !ensure_dir(output_dir)) {
        return 1;
    }

    if (!build_path(path, sizeof(path), "%s/%s", output_dir, "03_colors_orange_solid_screen.svg")) {
        return 1;
    }
    if (!write_solid_screen(path)) {
        return 1;
    }

    for (size_t i = 0; i < sizeof(UPPERCASE_GLYPHS) / sizeof(UPPERCASE_GLYPHS[0]); ++i) {
        if (!build_path(path, sizeof(path), "%s/03_colors_orange_ABC_%s.svg", output_dir, UPPERCASE_GLYPHS[i].name)) {
            return 1;
        }
        if (!write_letter(path, &UPPERCASE_GLYPHS[i])) {
            return 1;
        }
    }

    for (size_t i = 0; i < sizeof(LOWERCASE_GLYPHS) / sizeof(LOWERCASE_GLYPHS[0]); ++i) {
        if (!build_path(path, sizeof(path), "%s/03_colors_orange_abc_%s.svg", output_dir, LOWERCASE_GLYPHS[i].name)) {
            return 1;
        }
        if (!write_letter(path, &LOWERCASE_GLYPHS[i])) {
            return 1;
        }
    }

    if (!build_path(path, sizeof(path), "%s/%s", output_dir, "03_colors_orange_ball_one.svg")) {
        return 1;
    }
    if (!write_one_ball(path)) {
        return 1;
    }

    if (!build_path(path, sizeof(path), "%s/%s", output_dir, "03_colors_orange_balls_seven.svg")) {
        return 1;
    }
    if (!write_seven_balls(path)) {
        return 1;
    }

    if (!build_path(path, sizeof(path), "%s/%s", output_dir, "03_colors_orange_shapes_triangle.svg")) {
        return 1;
    }
    if (!write_polygon_shape(path, 3, -90)) {
        return 1;
    }

    if (!build_path(path, sizeof(path), "%s/%s", output_dir, "03_colors_orange_shapes_square.svg")) {
        return 1;
    }
    if (!write_square_shape(path)) {
        return 1;
    }

    if (!build_path(path, sizeof(path), "%s/%s", output_dir, "03_colors_orange_shapes_pentagon.svg")) {
        return 1;
    }
    if (!write_polygon_shape(path, 5, -90)) {
        return 1;
    }

    if (!build_path(path, sizeof(path), "%s/%s", output_dir, "03_colors_orange_shapes_hexagon.svg")) {
        return 1;
    }
    if (!write_polygon_shape(path, 6, -90)) {
        return 1;
    }

    printf("FILE COUNT: %d\n", created_files);
    int expected = expected_file_count();
    if (created_files != expected) {
        fprintf(stderr, "ERROR: expected %d files, got %d\n", expected, created_files);
        return 1;
    }

    return 0;
}
