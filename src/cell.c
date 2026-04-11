#include "cell.h"

Cell cell_from_value(StringView value) {
    return (Cell) {
        .kind = CELL_KIND_VALUE,
        .as.value = value
    };
}

Cell cell_from_formula(StringView formula) {
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
