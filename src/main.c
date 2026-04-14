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

char *fcontent(char *filepath) {
    FILE *file = fopen(filepath, "r");
    if(file == NULL) {
        perror("fopen failed");
        return NULL;
    }

    if(fseek(file, 0, SEEK_END) < 0) {
        fclose(file);
        perror("fseek failed");
        return NULL;
    }

    long int size = ftell(file);
    if(size < 0) {
        fclose(file);
        perror("ftell failed");
        return NULL;
    }
    size_t len = (size_t)size;

    if(fseek(file, 0, SEEK_SET) < 0) {
        fclose(file);
        perror("fseek failed");
        return NULL;
    }

    char *content = malloc(sizeof(char) * (len + 1));
    if(content == NULL) {
        fclose(file);
        perror("malloc failed");
        return NULL;
    }
    content[len] = 0;
    
    size_t n = fread(content, sizeof(char) * len, 1, file);
    if(n < 1) {
        fclose(file);
        free(content);
        perror("fread failed");
        return NULL;
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

#include "lexer.h"

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
    
    char *content = fcontent(filepath);
    if(content == NULL) {
        return 1;
    }

    Excel *excel = excel_new();
    excel_add_sheet_from_raw(excel, content);
    excel_free(excel);

    allocator_kill(allocator);
    return 0;
}