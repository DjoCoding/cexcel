#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ARRAY_INITIAL_SIZE 100

#define STRING_VIEW_IMPLEMENTATION
#define ALLOCATOR_IMPLEMENTATION
#define STRING_BUILDER_IMPLEMENTATION

#include <allocator.h>
#include <sb.h>
#include <array.h>

#include "excel.h"

char *fcontent(Allocator *allocator, char *filepath) {
    FILE *file = fopen(filepath, "r");
    if(file == NULL) {
        perror("fopen failed");
        exit(EXIT_FAILURE);
    }

    if(fseek(file, 0, SEEK_END) < 0) {
        fclose(file);
        perror("fseek failed");
        exit(EXIT_FAILURE);
    }

    long int size = ftell(file);
    if(size < 0) {
        fclose(file);
        perror("ftell failed");
        exit(EXIT_FAILURE);
    }
    size_t len = (size_t)size;

    if(fseek(file, 0, SEEK_SET) < 0) {
        fclose(file);
        perror("fseek failed");
        exit(EXIT_FAILURE);
    }

    char *content = allocator_alloc(allocator, sizeof(char) * (len + 1));
    content[len] = 0;
    
    size_t n = fread(content, sizeof(char) * len, 1, file);
    if(n < 1) {
        fclose(file);
        allocator_free(allocator, content);
        perror("fread failed");
        exit(EXIT_FAILURE);
    }

    fclose(file);
    return content;
}


typedef struct {
    int     count;
    char    **values;
} Args;

Args args_init(int argc, char **argv) {
    return (Args) {
        .count = argc,
        .values = argv
    };
}

char *args_consume(Args *args) {
    assert(args->count != 0);
    char *arg = args->values[0];
    args->values += 1;
    args->count -= 1;
    return arg;
}

char *args_peek(Args args) {
    assert(args.count != 0);
    return args.values[0];
}

bool args_end(Args args) {
    return args.count == 0;
}

void usage(FILE *f) {
    (void)f;
}

int main(int argc, char **argv) {
    Args args = args_init(argc, argv);
    args_consume(&args);
    
    if(args_end(args)) {
        usage(stderr);
        fprintf(stderr, "ERROR: invalid arguments count.\n");
        exit(1);
    }

    Allocator *allocator = allocator_new();

    char *filepath = args_consume(&args);
    char *content = fcontent(allocator, filepath);

    Excel *excel = excel_new_from_allocator(allocator);
    excel_add_sheet_from_raw(excel, content);
    excel_free(excel);
    
    return 0;
}