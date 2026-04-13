#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>

#include <allocator.h>

#include "formula.h"

typedef struct {
    Allocator   *allocator;
    size_t      current;
} Parser;

Parser  *parser_new();
Formula *parser_parse_formula(Parser *parser, StringView formula_sv);
Literal  parser_parse_literal(Parser *parser, StringView literal_sv);
void     parser_free(Parser *parser);

#endif // PARSER_H_