#include <assert.h>
#include <ctype.h>

#include <array.h>
#include "const.h"

#include "parser.h"

// INTERNALS
Formula *__int__parser_parse_formula(Parser *parser);

// INLINE FUNCTIONS
inline static void __int__parser_prepare(Parser *parser, TokenList tokens);
inline static void __int__parser_cooldown(Parser *parser);
inline static bool __int__parser_finished(Parser *parser);


// HELPER FUNCTIONS
bool sv_is_integer(StringView sv) {
    for(size_t i = 0; i < sv.len; ++i) {
        char c = sv.data[i];
        if(!isdigit(c)) false; 
    }
    return true;
} 

bool sv_is_column(StringView sv) {
    for(size_t i = 0; i < sv.len; ++i) {
        char c = sv.data[i];
        if(!isalpha(c)) false; 
    }
    return true;
}

Parser *parser_new_from_allocator(Allocator *allocator) {
    Parser *parser = allocator_alloc(allocator, sizeof(*parser));
    parser->allocator   = allocator;
    parser->sb          = sb_new_from_allocator(parser->allocator);
    parser->has_tokens  = false;
    parser->cursor      = 0;
    parser->error       = NULL;
    return parser;
}

inline static void __int__parser_prepare(Parser *parser, TokenList tokens) {
    parser->tokens      = tokens;
    parser->has_tokens  = true;
    parser->cursor       = 0;
    parser->error        = NULL;
}

inline static void __int__parser_cooldown(Parser *parser) {
    parser->has_tokens  = false;
    parser->cursor       = 0;
}

inline static bool __int__parser_finished(Parser *parser) {
    assert(parser->has_tokens);
    return parser->cursor >= parser->tokens.len;    
}

Token __int__parser_peek_ahead(Parser *parser, size_t ahead) {
    assert(parser->has_tokens);
    assert(parser->cursor + ahead < parser->tokens.len);
    return parser->tokens.items[parser->cursor + ahead];
}

Token __int__parser_peek(Parser *parser) {
    return __int__parser_peek_ahead(parser, 0);
}

void __int__parser_advance_one(Parser *parser) {
    assert(parser->has_tokens);
    assert(!__int__parser_finished(parser));
    parser->cursor += 1;
}

Literal parser_parse_literal(Parser *parser, TokenList tokens) {
    sb_reset(parser->sb);

    if(tokens.len != 1) {
        sb_append_cstr(parser->sb, "expected a single literal but found more");
        parser->error = sb_collect(parser->sb);
        return INVALID_LITERAL;
    }
    
    Token token = tokens.items[0];
    assert(token.kind != TOKEN_KIND_EQUAL); // it should have been parsed by the parser_parse_formula

    if(token.kind == TOKEN_KIND_STRING_LITERAL) return STRING_LITERAL(token.as.string);
    if(token.kind == TOKEN_KIND_NUMBER_LITERAL) return NUMBER_LITERAL(token.as.number.value);
    if(token.kind == TOKEN_KIND_STRING_LITERAL) return BOOLEAN_LITERAL(token.as.boolean);

    sb_append_cstr(parser->sb, "expected a literal but found other");
    parser->error = sb_collect(parser->sb);

    return INVALID_LITERAL;
}

Formula *__int__parser_parse_cell_location_as_formula(Parser *parser) {
    assert(parser->has_tokens);
    assert(!__int__parser_finished(parser));

    Token col_token = __int__parser_peek(parser);
    assert(col_token.kind == TOKEN_KIND_IDENTIFIER);
    if(!sv_is_column(col_token.as.identifier)) {
        sb_append_cstr(parser->sb, "expected column name of cell location but else was found");
        sb_append_cstr(parser->sb, " ");
        sb_append_cstr(parser->sb, "|");
        sb_append(parser->sb, col_token.as.identifier.data, col_token.as.identifier.len);
        sb_append_cstr(parser->sb, "|");
        parser->error = sb_collect(parser->sb);
        return NULL;
    }

    __int__parser_advance_one(parser);

    assert(__int__parser_peek(parser).kind == TOKEN_KIND_COLON);
    __int__parser_advance_one(parser);

    if(__int__parser_finished(parser)) {
        sb_append_cstr(parser->sb, "expected row of cell location but end of input was found");
        parser->error = sb_collect(parser->sb);
        return NULL;
    }

    Token token = __int__parser_peek(parser);
    if(token.kind != TOKEN_KIND_NUMBER_LITERAL) {
        sb_append_cstr(parser->sb, "expected row of cell location but found else");
        parser->error = sb_collect(parser->sb);
        return NULL;
    }

    if(!sv_is_integer(token.as.number.as_sv)) {
        sb_append_cstr(parser->sb, "expected row of cell location to be a positive integer but found else");
        parser->error = sb_collect(parser->sb);
        return NULL;
    }
    size_t row = (size_t)token.as.number.value;

    __int__parser_advance_one(parser);

    return formula_new_as_cell_location(
        parser->allocator, 
        col_token.as.string, 
        row
    );
}

Formula *__int__parser_parse_function_call_as_formula(Parser *parser) {
    assert(parser->has_tokens);
    assert(!__int__parser_finished(parser));

    Token id_token = __int__parser_peek(parser);
    assert(id_token.kind == TOKEN_KIND_IDENTIFIER);
    __int__parser_advance_one(parser);

    assert(__int__parser_peek(parser).kind == TOKEN_KIND_OPEN_PAREN);
    __int__parser_advance_one(parser);


    FormulaList args = {0};
    while(!__int__parser_finished(parser)) {
        if(__int__parser_peek(parser).kind == TOKEN_KIND_CLOSE_PAREN) break;

        Formula *arg = __int__parser_parse_formula(parser);
        if(arg == NULL) {
            formula_list_free(parser->allocator, args);
            return NULL;
        }

        arrappend_allocator(args, arg, allocator_realloc, parser->allocator);

        if(__int__parser_finished(parser)) break;

        if(__int__parser_peek(parser).kind == TOKEN_KIND_SEMICOLON) {
            __int__parser_advance_one(parser);
            continue;
        }
    }

    if(__int__parser_finished(parser)) {
        sb_append_cstr(parser->sb, "expected ) at end of function call but end of input was found");
        parser->error = sb_collect(parser->sb);
        return NULL;
    }

    assert(__int__parser_peek(parser).kind == TOKEN_KIND_CLOSE_PAREN);
    __int__parser_advance_one(parser);

    return formula_new_as_function_call(parser->allocator, id_token.as.identifier, args);
}


Formula *__int__parser_parse_term_as_formula(Parser *parser) {
    assert(parser->has_tokens);
    assert(!__int__parser_finished(parser));

    Token token = __int__parser_peek(parser);

    if(token.kind == TOKEN_KIND_STRING_LITERAL) {
        __int__parser_advance_one(parser);
        return formula_new_as_string(parser->allocator, token.as.string);
    }

    if(token.kind == TOKEN_KIND_NUMBER_LITERAL) {
        __int__parser_advance_one(parser);
        return formula_new_as_number(parser->allocator, token.as.number.value);
    }

    if(token.kind == TOKEN_KIND_BOOL_LITERAL) {
        __int__parser_advance_one(parser);
        return formula_new_as_bool(parser->allocator, token.as.boolean);
    }

    if(token.kind == TOKEN_KIND_IDENTIFIER) {
        if(__int__parser_finished(parser)) {
            sb_append_cstr(parser->sb, "expected the function call but found end of input");
            parser->error = sb_collect(parser->sb);
            return NULL;
        }


        Token next_token = __int__parser_peek_ahead(parser, 1);
        if(next_token.kind == TOKEN_KIND_OPEN_PAREN) {
            return __int__parser_parse_function_call_as_formula(parser);
        }

        if(next_token.kind == TOKEN_KIND_COLON) {
            return __int__parser_parse_cell_location_as_formula(parser);
        }

        sb_append_cstr(parser->sb, "expected ( for function call but found else");
        parser->error = sb_collect(parser->sb);
        return NULL;
    }

    sb_append_cstr(parser->sb, "expected a term but found else");
    parser->error = sb_collect(parser->sb);
    return NULL;
}

Formula *__int__parser_parse_multiplicative_formula(Parser *parser) {
    assert(parser->has_tokens);
    assert(!__int__parser_finished(parser));

    Formula *left = __int__parser_parse_term_as_formula(parser);
    if(parser_has_error(parser)) {
        assert(left == NULL);
        return NULL;
    }
    
    while(!__int__parser_finished(parser)) {
        Token token = __int__parser_peek(parser);

        bool is_expected = false;
        Operator op      = OP_INVALID; 
        switch(token.kind) {
            case TOKEN_KIND_TIMES:   is_expected = true;  op = OP_TIMES;     break;
            case TOKEN_KIND_DIV:     is_expected = true;  op = OP_DIV;       break;
            case TOKEN_KIND_MOD:     is_expected = true;  op = OP_MOD;       break;
            default:                 is_expected = false;                    break;
        }

        if(!is_expected) return left;

        assert(op != OP_INVALID);

        __int__parser_advance_one(parser);

        if(__int__parser_finished(parser)) {
            sb_append_cstr(parser->sb, "expected the right hand side of the operation but found end of input");
            parser->error = sb_collect(parser->sb);
            formula_free(parser->allocator, left);
            return NULL;
        }

        Formula *right = __int__parser_parse_term_as_formula(parser);
        if(parser_has_error(parser)) {
            assert(right == NULL);
            formula_free(parser->allocator, left);
            return NULL;
        }

        left = formula_new_as_binary(parser->allocator, left, right, op);
    }

    return left;
}

Formula *__int__parser_parse_additive_formula(Parser *parser) {
    assert(parser->has_tokens);
    assert(!__int__parser_finished(parser));

    Formula *left = __int__parser_parse_multiplicative_formula(parser);
    if(parser_has_error(parser)) {
        assert(left == NULL);
        return NULL;
    }
    
    while(!__int__parser_finished(parser)) {
        Token token = __int__parser_peek(parser);

        bool is_expected = false;
        Operator op      = OP_INVALID; 
        switch(token.kind) {
            case TOKEN_KIND_PLUS:   is_expected = true;  op = OP_PLUS;     break;
            case TOKEN_KIND_MINUS:  is_expected = true;  op = OP_MINUS;    break;
            default:                is_expected = false;                   break;
        }

        if(!is_expected) return left;

        assert(op != OP_INVALID);

        __int__parser_advance_one(parser);

        if(__int__parser_finished(parser)) {
            sb_append_cstr(parser->sb, "expected the right hand side of the operation but found end of input");
            parser->error = sb_collect(parser->sb);
            formula_free(parser->allocator, left);
            return NULL;
        }

        Formula *right = __int__parser_parse_multiplicative_formula(parser);
        if(parser_has_error(parser)) {
            assert(right == NULL);
            formula_free(parser->allocator, left);
            return NULL;
        }

        left = formula_new_as_binary(parser->allocator, left, right, op);
    }

    return left;
}

Formula *__int__parser_parse_formula(Parser *parser) {
    assert(parser->has_tokens);
    assert(!__int__parser_finished(parser));
    return __int__parser_parse_additive_formula(parser);
}

Formula *parser_parse_formula(Parser *parser, TokenList tokens) {
    assert(tokens.len >= 1);
    assert(tokens.items[0].kind == TOKEN_KIND_EQUAL);

    sb_reset(parser->sb);
    
    __int__parser_prepare(parser, tokens);
    __int__parser_advance_one(parser);

    if(__int__parser_finished(parser)) {
        sb_append_cstr(parser->sb, "expected a formula but found end of input");
        parser->error = sb_collect(parser->sb);
        return NULL;
    }

    Formula *formula = __int__parser_parse_formula(parser);
    __int__parser_cooldown(parser);

    return formula;
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

