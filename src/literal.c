#include "literal.h"

Literal literal_init_from_string(StringView string) {
    return (Literal) {
        .kind = LIT_KIND_STRING,
        .as.string = string
    };
}

Literal literal_init_from_int(i64 value) {
    return (Literal) {
        .kind = LIT_KIND_INT,
        .as.int_ = value
    };
}

Literal literal_init_from_float(f64 value) {
    return (Literal) {
        .kind = LIT_KIND_FLOAT,
        .as.float_ = value
    };
}

Literal literal_init_from_bool(bool value) {
    return (Literal) {
        .kind = LIT_KIND_BOOL,
        .as.bool_ = value
    };
}