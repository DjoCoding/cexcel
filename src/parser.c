#include <assert.h>

#include "parser.h"

Parser *parser_new() {
    Allocator *allocator = allocator_new();
    Parser *parser = allocator_alloc(allocator, sizeof(*parser));
    parser->allocator = allocator;
    parser->current = 0;
    parser->allocator = allocator;
    return parser;
}

void __int__parser_reset(Parser *parser) {
    parser->current = 0;
    allocator_reset(parser->allocator);
}

Formula *parser_parse_formula(Parser *parser, StringView sv_formula) {
    __int__parser_reset(parser);
    (void)sv_formula;
    assert(false && "not implemented yet");
}


Literal parser_parse_literal(Parser *parser, StringView sv_literal) {
    __int__parser_reset(parser);
    (void)sv_literal;
    assert(false && "not implemented yet");
}

void parser_free(Parser *parser) {
    allocator_kill(parser->allocator);
}
