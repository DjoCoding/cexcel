#include <assert.h>
#include "sheet.h"

Sheet *sheet_new(Allocator *allocator, size_t rows, size_t cols, char *raw) {
    Sheet *sheet    = allocator_alloc(allocator, sizeof(*sheet));
    sheet->rows     = rows;
    sheet->cols     = cols;
    sheet->raw      = raw;
    sheet->cells    = allocator_zalloc(allocator, sizeof(*sheet->cells) * sheet->rows * sheet->cols);
    return sheet;
}

static inline size_t sheet_index(Sheet *s, size_t row, size_t col) {
    assert(s->rows > row);
    assert(s->cols > col);
    return row * s->cols + col;
}

void sheet_assign_cell(Sheet *s, size_t row, size_t col, Cell cell) {
    size_t idx = sheet_index(s, row, col);
    s->cells[idx] = cell;
}

Cell *sheet_get_cell(Sheet *s, size_t row, size_t col) {
    size_t idx = sheet_index(s, row, col);
    return &s->cells[idx];
}


void sheet_free(Sheet *s, Allocator *allocator) {
    for(size_t col = 0; col < s->cols; ++col) {
        for(size_t row = 0; row < s->rows; ++row) {
            Cell *cell = sheet_get_cell(s, row, col);
            if(cell->kind == CELL_KIND_FORMULA) {
                formula_free(allocator, cell->as.formula);
            }
        }
    }
    allocator_free(allocator, s->cells);
    allocator_free(allocator, s);
}
