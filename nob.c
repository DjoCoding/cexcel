#define NOB_IMPLEMENTATION
#include "nob.h"

void cmd_append_flags(Cmd *cmd) {
    nob_cmd_append(cmd, "-Wall", "-Wextra", "-Werror", "-ggdb2");
}

void cmd_append_includes(Cmd *cmd) {
    nob_cmd_append(cmd, "-I./clib");
}

bool add_c_files_as_src(Nob_Walk_Entry entry) {
    Cmd *cmd = (Cmd *)entry.data;
    
    if (entry.type == FILE_REGULAR) {
        String_View path = sv_from_cstr(entry.path);
        if (sv_end_with(path, ".c")) {
            nob_log(NOB_INFO, "%.*s %s", (int)entry.level*2, " ", entry.path);
            char *copy = strdup(entry.path);
            nob_cmd_append(cmd, copy);
            return true;
        }
    }

    if (entry.type == FILE_DIRECTORY) {
        nob_log(NOB_INFO, "dir: %s", entry.path);
        return true;
    }

    return true;
}


int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    if(!nob_mkdir_if_not_exists("build")) {
        nob_log(NOB_ERROR, "failed to mkdir build dir");
        exit(1);
    };

    Cmd cmd = {0};
    nob_cmd_append(&cmd, "gcc");

    cmd_append_flags(&cmd);
    cmd_append_includes(&cmd);

    const char *src_dir = "./src";
    if(!nob_walk_dir(src_dir, add_c_files_as_src, .data=&cmd)) {
        nob_log(NOB_ERROR, "failed to add all c source files");
        exit(1);
    }

    cmd_append(&cmd, "-o", "build/excel");

    if (!nob_cmd_run_sync(cmd)) {
        nob_log(NOB_ERROR, "build failed");
        return 1;
    }

    cmd_free(cmd);
    return 0;
}