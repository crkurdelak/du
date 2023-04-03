/**
 * @file du.c
 * @brief A minimal clone of the POSIX `du` utility.
 *
 * @author Brian R. Snider (bsnider@georgefox.edu)
 */

#include <libgen.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "du.h"

int main(int argc, char* argv[]) {
    // parse command-line arguments
    bool opt_all = false;
    bool opt_bytes = false;
    bool opts_valid = true;
    opterr = 0;
    int opt;
    while ((opt = getopt(argc, argv, "ab")) != -1) {
        switch (opt) {
            case 'a':
                opt_all = true;
                break;
            case 'b':
                opt_bytes = true;
                break;
            case '?':
                fprintf(stderr, "%s: invalid option -- '%c'\n", basename(argv[0]), optopt);
                opts_valid = false;
                break;
        }
    }

    if (!opts_valid) {
        exit(EXIT_FAILURE);
    }

    // determine if there are any additional arguments remaining
    if (optind == argc) {
        // no file/directory names provided,
        // so use current working directory
        // TODO process current working directory
        // call recursive fn
        printf(".\n");
    }
    else {
        // one or more file/directory names were provided,
        // so start where getopt() left off and iterate through them
        for (int i = optind; i < argc; i++) {
            // TODO process each file/directory
            printf("%s\n", argv[i]);
            // call recursive fn
            // :3
        }
    }
}

void process_dir(char *dirname, bool opt_all, bool opt_bytes) {
    // for each file in directory
    // find file system space used by file
    // if opt_all, print space used by file
    // if file is a directory, descend into directory and process its files (call process_dir())

    // if opt_bytes, print total space taken by directory in bytes
    // else print total space taken by directory in units ("blocks") of 1024 bytes
}
