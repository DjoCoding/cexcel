#ifndef CELL_H_
#define CELL_H_

#include <sv.h>

#include "formula.h"

typedef enum {
    CELL_KIND_FORMULA,
    CELL_KIND_LITERAL,
    CELL_KIND_EMPTY
} CellKind;

typedef struct {
    CellKind kind;
    union {
        Literal     literal;
        Formula     *formula;
    } as;
} Cell;

Cell cell_from_literal(Literal literal);
Cell cell_from_formula(Formula *formula);
Cell cell_as_empty();

#endif