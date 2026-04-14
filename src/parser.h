#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>

#include <allocator.h>
#include <sb.h>

#include "token.h"
#include "formula.h"

typedef struct {
    Allocator       *allocator;
    StringBuilder   *sb;
    TokenList       tokens;
    size_t          cursor;
    bool            has_tokens;
    char            *error;
} Parser;

Parser  *parser_new_from_allocator(Allocator *allocator);
Literal  parser_parse_literal(Parser *parser, TokenList tokens);
Formula *parser_parse_formula(Parser *parser, TokenList tokens);
bool     parser_has_error(Parser *parser);
char    *parser_get_error(Parser *parser);
void     parser_free(Parser *parser);

#endif // PARSER_H_