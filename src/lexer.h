#ifndef LEXER_H_
#define LEXER_H_

#include <allocator.h>
#include <sb.h>

#include "token.h"

typedef struct {
    Allocator       *allocator;
    StringBuilder   *sb;
    
    StringView      content;
    size_t          cursor;
    bool            has_content;
    char            *error;
} Lexer;

Lexer       *lexer_new_from_allocator(Allocator *allocator);
TokenList    lexer_lex(Lexer *lexer, StringView sv);
bool         lexer_has_error(Lexer *lexer);
char        *lexer_get_error(Lexer *lexer);
void         lexer_free(Lexer *lexer);

#endif // LEXER_H_