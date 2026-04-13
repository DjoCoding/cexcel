#include "formula.h"


Formula *__int__formula_new_from_allocator(Allocator *allocator) {
    Formula *formula = allocator_alloc(allocator, sizeof(*formula));
    return formula;
}

Formula *formula_new_as_string(Allocator *allocator, StringView string) {
    Formula *formula = __int__formula_new_from_allocator(allocator);
    formula->kind = EXPR_KIND_LITERAL;
    formula->as.literal = literal_init_from_string(string);
    return formula;
}

Formula *formula_new_as_int(Allocator *allocator, i64 value) {
    Formula *formula = __int__formula_new_from_allocator(allocator);
    formula->kind = EXPR_KIND_LITERAL;
    formula->as.literal = literal_init_from_int(value);
    return formula;
}

Formula *formula_new_as_float(Allocator *allocator, f64 value) {
    Formula *formula = __int__formula_new_from_allocator(allocator);
    formula->kind = EXPR_KIND_LITERAL;
    formula->as.literal = literal_init_from_float(value);
    return formula;
}

Formula *formula_new_as_bool(Allocator *allocator, bool value) {
    Formula *formula = __int__formula_new_from_allocator(allocator);
    formula->kind = EXPR_KIND_LITERAL;
    formula->as.literal = literal_init_from_bool(value);
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