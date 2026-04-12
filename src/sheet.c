#include <assert.h>
#include "sheet.h"

Sheet *sheet_new(Allocator *allocator, size_t rows, size_t cols, char *raw) {
    Sheet *sheet    = allocator_alloc(allocator, sizeof(*sheet));
    sheet->rows     = rows;
    sheet->cols     = cols;
    sheet->raw      = raw;
    sheet->cells    = allocator_alloc(allocator, sizeof(*sheet->cells) * sheet->rows * sheet->cols);
    return sheet;
}

static inline size_t sheet_index(Sheet *s, size_t row, size_t col) {
    return row * s->cols + col;
}

void sheet_assign_cell(Sheet *s, size_t row, size_t col, Cell cell) {
    assert(s->rows > row);
    assert(s->cols > col);
    
    size_t idx = sheet_index(s, row, col);
    s->cells[idx] = cell;
}


void sheet_free(Sheet *s, Allocator *allocator) {
    allocator_free(allocator, s->cells);
    allocator_free(allocator, s);
}
