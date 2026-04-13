#include <assert.h>

#include <array.h>

#include "cell.h"
#include "const.h"
#include "excel.h"

#define __int__excel_max(a, b) ((a) > (b) ? (a) : (b))

// INTERNALS
char *__int__excel_preprocess(Excel *excel, char *content);
ExcelStats __int__excel_stats(Excel *excel, char *preprocessed);


Excel *excel_new_from_allocator(Allocator *allocator) {
    StringBuilder  *sb = sb_new_from_allocator(allocator);
    
    Excel *excel = allocator_alloc(allocator, sizeof(*excel));
    *excel = (Excel){0};

    excel->allocator = allocator;
    excel->sb = sb;
    excel->parser = parser_new();

    return excel;
}

char *__int__excel_preprocess(Excel *excel, char *content) {
    sb_reset(excel->sb);
    
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
    ExcelStats stats = {0};

    StringView sv = sv_from_cstr(preprocessed);
    while(sv.len != 0) {
        stats.rows += 1;

        StringView line = sv_trim(sv_split(&sv, '\n'));
        size_t     line_cols = 0;
        while(line.len != 0) {
            line_cols += 1;
            sv_split(&line, EXCEL_FILE_SEP);
        }

        stats.cols = __int__excel_max(line_cols, stats.cols);
    }

    return stats;
}

Sheet *excel_add_sheet_from_raw(Excel *excel, char *raw_content) {
    char *preprocessed = __int__excel_preprocess(excel, raw_content);
    ExcelStats stats   = __int__excel_stats(excel, preprocessed);
    
    Sheet *sheet = sheet_new(
        excel->allocator, 
        stats.rows, 
        stats.cols, 
        raw_content
    );

    StringView preprocessed_sv = sv_from_cstr(preprocessed);
    for(size_t row = 0; row < stats.rows; ++row) {
        StringView line = sv_split(&preprocessed_sv, '\n');
        assert(line.len != 0);

        for(size_t col = 0; col < stats.cols; ++col) {
            if(line.len == 0) {
                sheet_assign_cell(sheet, row, col, cell_as_empty());
                continue;
            }
            
            StringView cell_sv = sv_trim(sv_split(&line, EXCEL_FILE_SEP));
            if(cell_sv.len == 0) {
                sheet_assign_cell(sheet, row, col, cell_as_empty());
                continue;
            }


            if(sv_starts_with(
                cell_sv, 
                sv_from_cstr(EXCEL_FORMULA_PREFIX)
            )) {
                // FIXME: Parse formula here and set the cell
                assert(false && "not implemented");
            }

            
            // FIXME: Parse value here and set the cell
            assert(false && "not implemented");
        }
    }

    arrappend_allocator(excel->sheets, sheet, allocator_realloc, excel->allocator);
    return sheet;
}

void excel_free(Excel *excel) {
    parser_free(excel->parser);
    return allocator_kill(excel->allocator);
}
