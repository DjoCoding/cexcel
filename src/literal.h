#ifndef LITERAL_H_
#define LITERAL_H_

#include <stdbool.h>

#include <sv.h>
#include <allocator.h>

#include "types.h"

typedef enum {
    LIT_KIND_INVALID,
    LIT_KIND_STRING,
    LIT_KIND_NUMBER,
    LIT_KIND_BOOL
} LiteralKind;

typedef struct {
    LiteralKind kind;
    union {
        StringView  string;
        f64         number;
        bool        boolean;
    } as;
} Literal;

#define INVALID_LITERAL         ((Literal) {.kind=LIT_KIND_INVALID})
#define STRING_LITERAL(s)       ((Literal) {.kind=LIT_KIND_STRING,.as.string=s})
#define NUMBER_LITERAL(v)       ((Literal) {.kind=LIT_KIND_NUMBER,.as.number=v})
#define BOOLEAN_LITERAL(b)      ((Literal) {.kind=LIT_KIND_BOOL,.as.boolean=b})

#endif // LITERAL_H_