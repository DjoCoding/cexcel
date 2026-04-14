#include <assert.h>

#include <array.h>

#include "formula.h"


Formula *__int__formula_new_from_allocator(Allocator *allocator) {
    Formula *formula = allocator_alloc(allocator, sizeof(*formula));
    return formula;
}

Formula *formula_new_as_cell_location(Allocator *allocator, StringView col, size_t row) {
    Formula *formula = __int__formula_new_from_allocator(allocator);
    formula->kind = EXPR_KIND_CELL_LOCATION;
    formula->as.cell_location.col = col;
    formula->as.cell_location.row = row;
    return formula;
}

Formula *formula_new_as_string(Allocator *allocator, StringView string) {
    Formula *formula = __int__formula_new_from_allocator(allocator);
    formula->kind = EXPR_KIND_LITERAL;
    formula->as.literal = STRING_LITERAL(string);
    return formula;
}

Formula *formula_new_as_number(Allocator *allocator, f64 value) {
    Formula *formula = __int__formula_new_from_allocator(allocator);
    formula->kind = EXPR_KIND_LITERAL;
    formula->as.literal = NUMBER_LITERAL(value);
    return formula;
}

Formula *formula_new_as_bool(Allocator *allocator, bool value) {
    Formula *formula = __int__formula_new_from_allocator(allocator);
    formula->kind = EXPR_KIND_LITERAL;
    formula->as.literal = BOOLEAN_LITERAL(value);
    return formula;
}

Formula *formula_new_as_binary(Allocator *allocator, Formula *left, Formula *right, Operator op) {
    Formula *formula = __int__formula_new_from_allocator(allocator);
    formula->kind = EXPR_KIND_BINARY;
    formula->as.binary.left     = left;
    formula->as.binary.op       = op;
    formula->as.binary.right    = right;
    return formula;
}

Formula *formula_new_as_unary(Allocator *allocator, Operator op, Formula *value) {
    Formula *formula = __int__formula_new_from_allocator(allocator);
    formula->kind = EXPR_KIND_UNARY;
    formula->as.unary.op        = op;
    formula->as.unary.value     = value;
    return formula;
}

Formula *formula_new_as_function_call(Allocator *allocator, StringView name, FormulaList args) {
    Formula *formula = __int__formula_new_from_allocator(allocator);
    formula->kind = EXPR_KIND_FUNC_CALL;
    formula->as.func_call.name = name;
    formula->as.func_call.args = args;
    return formula;
}

void formula_free(Allocator *allocator, Formula *formula) {
    if(formula == NULL) return;

    if(formula->kind == EXPR_KIND_BINARY) {
        formula_free(allocator, formula->as.binary.left);
        formula_free(allocator, formula->as.binary.right);
        formula_free(allocator, formula);
        return;
    }

    if(formula->kind == EXPR_KIND_UNARY) {
        formula_free(allocator, formula->as.unary.value);
        formula_free(allocator, formula);
        return;
    }

    if(formula->kind == EXPR_KIND_FUNC_CALL) {
        formula_list_free(allocator, formula->as.func_call.args);
        formula_free(allocator, formula);
        return;
    }

    return formula_free(allocator, formula);
}

void formula_list_free(Allocator *allocator, FormulaList formulas) {
    for(size_t i = 0; i < formulas.len; ++i) {
        formula_free(allocator, formulas.items[i]);
    }
    arrfree_allocator(formulas, allocator_free, allocator);
}
