#include "excel.h"

#include <array.h>

// INTERNALS
char *__int__excel_preprocess(Excel *excel, char *content);
ExcelStats __int__excel_stats(Excel *excel, char *preprocessed);


Excel *excel_new_from_allocator(Allocator *allocator) {
    StringBuilder  *sb = sb_new_from_allocator(allocator);
    
    Excel *excel = allocator_alloc(allocator, sizeof(*excel));
    *excel = (Excel){0};

    excel->allocator = allocator;
    excel->sb = sb;

    return excel;
}

char *__int__excel_preprocess(Excel *excel, char *content) {
    StringView sv_content = sv_from_cstr(content);
    while(sv_content.len != 0) {
        StringView line = sv_trim(sv_split(&sv_content, '\n'));
        if(line.len == 0) continue;

        sb_append(excel->sb, line.data, line.len);
        if(sv_content.len != 0) sb_append_cstr(excel->sb, "\n");
    }
    return sb_collect(excel->sb);
}

ExcelStats __int__excel_stats(Excel *excel, char *preprocessed) {
    (void)excel;
    (void)preprocessed;
    return (ExcelStats) {0};
}

Sheet *excel_add_sheet_from_raw(Excel *excel, char *raw_content) {
    char *preprocessed = __int__excel_preprocess(excel, raw_content);
    ExcelStats stats   = __int__excel_stats(excel, preprocessed);
    
    Sheet *sheet    = allocator_alloc(excel->allocator, sizeof(*sheet));
    sheet->rows     = stats.rows_count;
    sheet->cols     = stats.cols_count;
    sheet->raw      = raw_content;
    sheet->cells    = allocator_alloc(excel->allocator, sizeof(**sheet->cells) * sheet->rows * sheet->cols);

    // FIXME: parse the preprocessed content here and set the cells

    arrappend(excel->sheets, sheet);
    return sheet;
}

void excel_free(Excel *excel) {
    return allocator_kill(excel->allocator);
}
