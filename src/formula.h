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
    EXPR_KIND_BINARY
} ExpressionKind;

typedef enum {
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


struct Expression {
    ExpressionKind kind;
    union {
        BinaryExpression binary;
        UnaryExpression  unary;
        FunctionCall     func_call;
        Literal          literal;
    } as; 
};

typedef Expression      Formula;
typedef ExpressionList  FormulaList;

Formula *formula_new_as_function_call(Allocator *allocator, StringView name, FormulaList args);
Formula *formula_new_as_unary(Allocator *allocator, Operator op, Formula *value);
Formula *formula_new_as_binary(Allocator *allocator, Formula *left, Formula *right, Operator op);
Formula *formula_new_as_bool(Allocator *allocator, bool value);
Formula *formula_new_as_float(Allocator *allocator, f64 value);
Formula *formula_new_as_int(Allocator *allocator, i64 value);
Formula *formula_new_as_string(Allocator *allocator, StringView string);


#endif // FORMULA_H_
