#ifndef FORMULA_H_
#define FORMULA_H_

#include <stdbool.h>

#include <sv.h>
#include <allocator.h>

#include "literal.h"

typedef struct Expression Expression;

typedef enum {
    EXPR_KIND_LITERAL = 0,
    EXPR_KIND_UNARY,
    EXPR_KIND_FUNC_CALL,
    EXPR_KIND_BINARY,
    EXPR_KIND_CELL_LOCATION,
} ExpressionKind;

typedef enum {
    OP_INVALID = 0,
    OP_PLUS,
    OP_MINUS,
    OP_TIMES,
    OP_DIV,
    OP_MOD,
} Operator;

typedef struct {
    Operator   op;
    Expression *left, *right;
} BinaryExpression;

typedef struct {
    Operator   op;        // must be one of the two:     OP_PLUS, OP_MINUS;
    Expression *value;
} UnaryExpression;

typedef struct {
    Expression  **items;
    size_t      len;
    size_t      size;
} ExpressionList;

typedef struct {
    StringView      name;
    ExpressionList  args;
} FunctionCall;

typedef struct {
    StringView      col;
    size_t          row;
} CellLocation;


struct Expression {
    ExpressionKind kind;
    union {
        BinaryExpression binary;
        UnaryExpression  unary;
        FunctionCall     func_call;
        CellLocation     cell_location;
        Literal          literal;
    } as; 
};

typedef Expression      Formula;
typedef ExpressionList  FormulaList;

Formula *formula_new_as_cell_location(Allocator *allocator, StringView col, size_t row);
Formula *formula_new_as_function_call(Allocator *allocator, StringView name, FormulaList args);
Formula *formula_new_as_unary(Allocator *allocator, Operator op, Formula *value);
Formula *formula_new_as_binary(Allocator *allocator, Formula *left, Formula *right, Operator op);
Formula *formula_new_as_bool(Allocator *allocator, bool value);
Formula *formula_new_as_number(Allocator *allocator, f64 value);
Formula *formula_new_as_string(Allocator *allocator, StringView string);
void     formula_free(Allocator *allocator, Formula *formula);
void     formula_list_free(Allocator *allocator, FormulaList formulas);


#endif // FORMULA_H_
