#include <assert.h>
#include <ctype.h>

#include <array.h>
#include "const.h"

#include "lexer.h"

// INLINE FUNCTIONS
inline static void __int__lexer_prepare(Lexer *lexer, StringView sv);
inline static void __int__lexer_cooldown(Lexer *lexer);
inline static bool __int__lexer_finished(Lexer *lexer);


Lexer *lexer_new_from_allocator(Allocator *allocator) {
    Lexer *lexer = allocator_alloc(allocator, sizeof(*lexer));
    lexer->allocator    = allocator;
    lexer->sb           = sb_new_from_allocator(lexer->allocator);
    lexer->has_content  = false;
    lexer->content      = SV_NULL;
    lexer->error        = NULL;
    lexer->cursor       = 0;
    return lexer;
}

inline static void __int__lexer_prepare(Lexer *lexer, StringView sv) {
    lexer->content      = sv;
    lexer->has_content  = true;
    lexer->cursor       = 0;
    lexer->error        = NULL;
}

inline static void __int__lexer_cooldown(Lexer *lexer) {
    lexer->content      = SV_NULL;
    lexer->has_content  = false;
    lexer->cursor       = 0;
}

inline static bool __int__lexer_finished(Lexer *lexer) {
    assert(lexer->has_content);
    return lexer->cursor >= lexer->content.len;    
}

char __int__lexer_peek(Lexer *lexer) {
    assert(lexer->has_content);
    assert(!__int__lexer_finished(lexer));
    return lexer->content.data[lexer->cursor];
}

void __int__lexer_advance(Lexer *lexer, size_t len) {
    assert(lexer->has_content);
    assert(!__int__lexer_finished(lexer));
    assert(len + lexer->cursor <= lexer->content.len);
    lexer->cursor += len;
}

void __int__lexer_advance_one(Lexer *lexer) {
    __int__lexer_advance(lexer, 1);
}

StringView __int__lexer_peek_part(Lexer *lexer, size_t len) {
    assert(lexer->has_content);
    size_t from = lexer->cursor;
    size_t to   = lexer->cursor + len;
    if(to > lexer->content.len) to = lexer->content.len;
    return sv_substring(lexer->content, from, to);
}

StringView __int__lexer_lex_string_literal(Lexer *lexer) {
    assert(!__int__lexer_finished(lexer));
    assert(__int__lexer_peek(lexer) == '\"');
    __int__lexer_advance_one(lexer);

    char    *data = lexer->content.data;
    size_t  len   = 0;

    while(!__int__lexer_finished(lexer)) {
        if(__int__lexer_peek(lexer) == '\"') break;
        __int__lexer_advance_one(lexer);
        len += 1;
    }

    if(__int__lexer_finished(lexer)) {
        if(len == 0) {
            sb_append_cstr(lexer->sb, "expected string literal, but found end of input");
            lexer->error = sb_collect(lexer->sb);
            return SV_NULL;
        }

        sb_append_cstr(lexer->sb, "not closed string literal");
        sb_append_cstr(lexer->sb, " ");
        sb_append_str(lexer->sb, data, len);
        lexer->error = sb_collect(lexer->sb);
        return SV_NULL;
    }

    if(__int__lexer_peek(lexer) == '\"') {
        __int__lexer_advance_one(lexer);
        return sv_init(data, len);
    }
    
    assert(false && "unreachable");
}

f64 __int__lexer_lex_number(Lexer *lexer, StringView *as_sv) {
    assert(!__int__lexer_finished(lexer));
    assert(isdigit(__int__lexer_peek(lexer)));

    if(as_sv != NULL) as_sv->data    = lexer->content.data;
    f64    value   = 0;
    size_t len     = 0;

    while(!__int__lexer_finished(lexer)) {
        char c = __int__lexer_peek(lexer);
        if(!isdigit(c)) break;
        
        value *= 10;
        value += c - '0';
        
        len += 1;
        __int__lexer_advance_one(lexer);
    }

    assert(len >= 1);
    if(as_sv != NULL)  as_sv->len = len;

    return value;
}

StringView __int__lexer_lex_identifier(Lexer *lexer) {
    assert(!__int__lexer_finished(lexer));

    char c = __int__lexer_peek(lexer);
    assert(isalpha(c) || c == '_');

    char   *data = lexer->content.data;
    size_t  len   = 0;

    while(!__int__lexer_finished(lexer)) {
        char c = __int__lexer_peek(lexer);
        if(!isalpha(c) && c != '_') break;
        
        len += 1;
        __int__lexer_advance_one(lexer);
    }

    assert(len >= 1);
    return sv_init(data, len);
}

Token __int__lexer_lex_one(Lexer *lexer) {
    assert(!__int__lexer_finished(lexer));

    if(sv_equal(
        __int__lexer_peek_part(lexer, 1),
        sv_from_cstr(EXCEL_FORMULA_PREFIX)
    )) {
        __int__lexer_advance_one(lexer);
        return TOKEN_EQUAL;
    }

    if(__int__lexer_peek(lexer) == '(') {
        __int__lexer_advance_one(lexer);
        return TOKEN_OPEN_PAREN;
    }

    if(__int__lexer_peek(lexer) == ')') {
        __int__lexer_advance_one(lexer);
        return TOKEN_CLOSE_PAREN;
    }

    if(__int__lexer_peek(lexer) == '+') {
        __int__lexer_advance_one(lexer);
        return TOKEN_PLUS;
    }

    if(__int__lexer_peek(lexer) == '-') {
        __int__lexer_advance_one(lexer);
        return TOKEN_MINUS;
    }


    if(__int__lexer_peek(lexer) == '*') {
        __int__lexer_advance_one(lexer);
        return TOKEN_TIMES;
    }

    if(__int__lexer_peek(lexer) == '/') {
        __int__lexer_advance_one(lexer);
        return TOKEN_DIV;
    }

    if(__int__lexer_peek(lexer) == '%') {
        __int__lexer_advance_one(lexer);
        return TOKEN_MOD;
    }

    if(__int__lexer_peek(lexer) == ';') {
        __int__lexer_advance_one(lexer);
        return TOKEN_SEMICOLON;
    }

    if(__int__lexer_peek(lexer) == ':') {
        __int__lexer_advance_one(lexer);
        return TOKEN_COLON;
    }

    if(__int__lexer_peek(lexer) == '\"') {
        StringView string = __int__lexer_lex_string_literal(lexer);
        if(lexer_has_error(lexer)) return INVALID_TOKEN;
        return TOKEN_STRING_LITERAL(string);
    }

    if(sv_equal(
        __int__lexer_peek_part(lexer, sizeof("true")),
        sv_from_cstr("true")
    )) {
        __int__lexer_advance(lexer, sizeof("true"));
        return TOKEN_BOOL_LITERAL(true);
    }

    if(sv_equal(
        __int__lexer_peek_part(lexer, sizeof("false")),
        sv_from_cstr("false")
    )) {
        __int__lexer_advance(lexer, sizeof("false"));
        return TOKEN_BOOL_LITERAL(false);
    }

    if(isdigit(__int__lexer_peek(lexer))) {
        StringView as_sv = SV_NULL;
        f64 value = __int__lexer_lex_number(lexer, &as_sv);
        if(!lexer_has_error(lexer)) return TOKEN_NUMBER_LITERAL(value, as_sv);
        return INVALID_TOKEN;
    }

    char c = __int__lexer_peek(lexer);
    if(isalpha(c) || c == '_') {
        StringView identifier = __int__lexer_lex_identifier(lexer);
        if(!lexer_has_error(lexer)) return TOKEN_IDENTIFIER_LITERAL(identifier);
        return INVALID_TOKEN;
    }

    StringView token_sv = __int__lexer_peek_part(lexer, 1);

    sb_append_cstr(lexer->sb, "invalid token");
    sb_append_cstr(lexer->sb, " ");
    sb_append_str(lexer->sb, token_sv.data, token_sv.len);
    lexer->error = sb_collect(lexer->sb);

    return INVALID_TOKEN; 
}

TokenList lexer_lex(Lexer *lexer, StringView sv) {
    TokenList tokenlist = {0};
    __int__lexer_prepare(lexer, sv);

    sb_reset(lexer->sb);
    while(!__int__lexer_finished(lexer)) {
        if(isspace(__int__lexer_peek(lexer))) {
            __int__lexer_advance_one(lexer);
            continue;
        }

        Token token = __int__lexer_lex_one(lexer);
        if(lexer_has_error(lexer)) {
            if(tokenlist.len != 0) allocator_free(lexer->allocator, tokenlist.items);
            return (TokenList){0};
        }

        arrappend_allocator(tokenlist, token, allocator_realloc, lexer->allocator);
        sb_reset(lexer->sb);
    }

    __int__lexer_cooldown(lexer);
    return tokenlist;
}

bool lexer_has_error(Lexer *lexer) {
    return lexer->error != NULL;
}

char *lexer_get_error(Lexer *lexer) {
    assert(lexer_has_error(lexer));
    return lexer->error;
}

void lexer_free(Lexer *lexer) {
    sb_free(lexer->sb);
    allocator_free(lexer->allocator, lexer);    
}
