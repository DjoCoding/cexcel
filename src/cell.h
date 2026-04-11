#ifndef CELL_H_
#define CELL_H_

#include <sv.h>

typedef enum {
    CELL_KIND_FORMULA,
    CELL_KIND_VALUE,
    CELL_KIND_EMPTY
} CellKind;

typedef struct {
    CellKind kind;
    union {
        StringView value;
        StringView formula;
    } as;
} Cell;

#endif