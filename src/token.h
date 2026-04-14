#ifndef TOKEN_H_
#define TOKEN_H_

#include <sv.h>

#include "types.h"

typedef enum {
    TOKEN_KIND_INVALID = 0,
    TOKEN_KIND_STRING_LITERAL,
    TOKEN_KIND_NUMBER_LITERAL,
    TOKEN_KIND_BOOL_LITERAL,
    TOKEN_KIND_IDENTIFIER,
    TOKEN_KIND_OPEN_PAREN,
    TOKEN_KIND_CLOSE_PAREN,
    TOKEN_KIND_EQUAL,
    TOKEN_KIND_PLUS,
    TOKEN_KIND_MINUS,
    TOKEN_KIND_TIMES,
    TOKEN_KIND_DIV,
    TOKEN_KIND_MOD,
    TOKEN_KIND_SEMICOLON,
    TOKEN_KIND_COLON,
} TokenKind;

typedef struct {
    TokenKind kind;
    union {
        StringView  string;
        struct {
            f64         value;
            StringView  as_sv;
        } number;
        bool        boolean;
        StringView  identifier;
    } as;
} Token;

typedef struct {
    Token   *items;
    size_t  len;
    size_t  size;
} TokenList;

#define INVALID_TOKEN                   ((Token){.kind=TOKEN_KIND_INVALID})
#define TOKEN_EQUAL                     ((Token){.kind=TOKEN_KIND_EQUAL})
#define TOKEN_OPEN_PAREN                ((Token){.kind=TOKEN_KIND_OPEN_PAREN})
#define TOKEN_CLOSE_PAREN               ((Token){.kind=TOKEN_KIND_CLOSE_PAREN})
#define TOKEN_PLUS                      ((Token){.kind=TOKEN_KIND_PLUS})
#define TOKEN_MINUS                     ((Token){.kind=TOKEN_KIND_MINUS})
#define TOKEN_TIMES                     ((Token){.kind=TOKEN_KIND_TIMES})
#define TOKEN_DIV                       ((Token){.kind=TOKEN_KIND_DIV})
#define TOKEN_MOD                       ((Token){.kind=TOKEN_KIND_MOD})
#define TOKEN_SEMICOLON                 ((Token){.kind=TOKEN_KIND_SEMICOLON})
#define TOKEN_STRING_LITERAL(s)         ((Token){.kind=TOKEN_KIND_STRING_LITERAL,.as.string=s})
#define TOKEN_BOOL_LITERAL(b)           ((Token){.kind=TOKEN_KIND_STRING_LITERAL,.as.boolean=b})
#define TOKEN_NUMBER_LITERAL(v, s)      ((Token){.kind=TOKEN_KIND_NUMBER_LITERAL,.as.number={.value=v,.as_sv=s}})
#define TOKEN_IDENTIFIER_LITERAL(v)     ((Token){.kind=TOKEN_KIND_IDENTIFIER,.as.identifier=v})
#define TOKEN_COLON                     ((Token){.kind=TOKEN_KIND_COLON})

#endif // TOKEN_H_
