#ifndef SHEET_H_
#define SHEET_H_

#include <allocator.h>

#include "cell.h"

typedef struct {
    Cell    *cells;       // matrix of cells
    size_t  rows;
    size_t  cols;
    char    *raw;
} Sheet;

Sheet *sheet_new(Allocator *allocator, size_t rows, size_t cols, char *raw);
void   sheet_assign_cell(Sheet *sheet, size_t row, size_t col, Cell cell);
void   sheet_free(Sheet *sheet, Allocator *allocator);

#endif // SHEET_H_
