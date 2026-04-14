#include <assert.h>
#include <ctype.h>

#include "const.h"
#include "parser.h"

Parser *parser_new_from_allocator(Allocator *allocator) {
    Parser *parser = allocator_alloc(allocator, sizeof(*parser));
    parser->allocator   = allocator;
    parser->sb          = sb_new_from_allocator(parser->allocator);
    parser->error       = NULL;
    return parser;
}

Literal parser_parse_literal(Parser *parser, TokenList tokens) {
    (void)parser;
    (void)tokens;
    assert(false && "not implemented yet");
}

Formula *parser_parse_formula(Parser *parser, TokenList tokens) {
    (void)parser;
    (void)tokens;
    assert(false && "not implemented yet");
}

bool parser_has_error(Parser *parser) {
    return parser->error != NULL;
}

char *parser_get_error(Parser *parser) {
    assert(parser_has_error(parser));
    return parser->error;
}

void parser_free(Parser *parser) {
    sb_free(parser->sb);
    allocator_free(parser->allocator, parser);
}

