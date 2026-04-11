#ifndef EXCEL_H_
#define EXCEL_H_

#include <stdio.h>

#include <allocator.h>
#include <sb.h>

#include "sheet.h"

typedef struct {
    Allocator       *allocator;
    StringBuilder   *sb;
    struct {
        Sheet       **items;
        size_t      len;
        size_t      size;
    } sheets;
} Excel;

typedef struct {
    size_t rows_count;
    size_t cols_count;
} ExcelStats;

void excel_free(Excel *excel);
Sheet *excel_add_sheet_from_raw(Excel *excel, char *raw_content);
Excel *excel_new_from_allocator(Allocator *allocator);

#endif // EXCEL_H_