#include "cell.h"

Cell cell_from_literal(Literal literal) {
    return (Cell) {
        .kind = CELL_KIND_LITERAL,
        .as.literal = literal
    };
}

Cell cell_from_formula(Formula *formula) {
    return (Cell) {
        .kind = CELL_KIND_FORMULA,
        .as.formula = formula
    };
} 

Cell cell_as_empty() {
    return (Cell) {
        .kind = CELL_KIND_EMPTY,
    };
}
