#ifndef SHEET_H_
#define SHEET_H_

#include "cell.h"

typedef struct {
    Cell    **cells;       // matrix of cells
    size_t  rows;
    size_t  cols;
    char    *raw;
} Sheet;

#endif // SHEET_H_
