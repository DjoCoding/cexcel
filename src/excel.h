#ifndef EXCEL_H_
#define EXCEL_H_

#include <stdio.h>

#include <allocator.h>
#include <sb.h>

#include "sheet.h"
#include "parser.h"
#include "lexer.h"

typedef struct {
    Allocator       *allocator;
    StringBuilder   *sb;
    Parser          *parser;
    Lexer           *lexer;
    struct {
        Sheet       **items;
        size_t      len;
        size_t      size;
    } sheets;
    char            *error;
} Excel;

typedef struct {
    size_t rows;
    size_t cols;
} ExcelStats;

Excel   *excel_new();
Sheet   *excel_add_sheet_from_raw(Excel *excel, char *raw_content);
bool     excel_has_error(Excel *excel);
char    *excel_get_error(Excel *excel);
void     excel_free(Excel *excel);

#endif // EXCEL_H_