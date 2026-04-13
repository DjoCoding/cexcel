#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>

#include <allocator.h>

typedef struct {
    Allocator   *allocator;
    size_t      current;
} Parser;

Parser *parser_new_from_allocator(Allocator *allocator);

#endif // PARSER_H_