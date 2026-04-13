#ifndef EXCEL_H_
#define EXCEL_H_

#include <stdio.h>

#include <allocator.h>
#include <sb.h>

#include "sheet.h"
#include "parser.h"

typedef struct {
    Allocator       *allocator;
    StringBuilder   *sb;
    Parser          *parser;
    struct {
        Sheet       **items;
        size_t      len;
        size_t      size;
    } sheets;
} Excel;

typedef struct {
    size_t rows;
    size_t cols;
} ExcelStats;

Excel   *excel_new_from_allocator(Allocator *allocator);
Sheet   *excel_add_sheet_from_raw(Excel *excel, char *raw_content);
void    excel_free(Excel *excel);

#endif // EXCEL_H_