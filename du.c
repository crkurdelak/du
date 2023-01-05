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
        printf(".\n");
    }
    else {
        // one or more file/directory names were provided,
        // so start where getopt() left off and iterate through them
        for (int i = optind; i < argc; i++) {
            // TODO process each file/directory
            printf("%s\n", argv[i]);
        }
    }
}
