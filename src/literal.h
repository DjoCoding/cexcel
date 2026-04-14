#ifndef LITERAL_H_
#define LITERAL_H_

#include <stdbool.h>

#include <sv.h>
#include <allocator.h>

#include "types.h"

typedef enum {
    LIT_KIND_INVALID,
    LIT_KIND_FLOAT,
    LIT_KIND_STRING,
    LIT_KIND_INT,
    LIT_KIND_BOOL
} LiteralKind;

typedef struct {
    LiteralKind kind;
    union {
        f64         float_;
        StringView  string;
        i64         int_;
        bool        bool_;
    } as;
} Literal;

#define INVALID_LITERAL         ((Literal) {.kind = LIT_KIND_INVALID})

Literal literal_init_from_string(StringView string);
Literal literal_init_from_int(i64 value);
Literal literal_init_from_float(f64 value);
Literal literal_init_from_bool(bool value);

#endif // LITERAL_H_