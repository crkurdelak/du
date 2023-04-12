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
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
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
        char* working_dirname = NULL;
        getcwd(working_dirname, 100);
        // open the directory
        DIR* dir_stream = opendir(working_dirname);
        // call recursive fn
        process_dir(dir_stream, working_dirname, opt_all, opt_bytes);
        printf(".\n");
    }
    else {
        // one or more file/directory names were provided,
        // so start where getopt() left off and iterate through them
        for (int i = optind; i < argc; i++) {
            printf("%s\n", argv[i]);
            // open the directory
            DIR* dir_stream = opendir(argv[i]);
            // call recursive fn
            process_dir(dir_stream, argv[i], opt_all, opt_bytes);
        }
    }
}

void process_dir(DIR *dir_stream, char *path, bool opt_all, bool opt_bytes) {
    unsigned long dir_space = 0;
    unsigned long file_space = 0;
    struct stat* stat_buf = NULL;
    // get first entry in directory
    struct dirent* current_entry = readdir(dir_stream);
    while (current_entry) {
        // if file is a directory, descend into directory and process its files (call process_dir())
        if (current_entry->d_type == DT_DIR) {
            if (strcmp(current_entry->d_name, ".") != 0 && strcmp(current_entry->d_name, "..") !=
            0) {
                DIR *next_dir = opendir(current_entry->d_name);
                process_dir(next_dir, path, opt_all, opt_bytes);
            }
        }
        else {
            // find file system space used by file

            // append directory name to path
            strcat(path, current_entry->d_name);
            stat(path, stat_buf); // TODO why is this returning null
            file_space = stat_buf->st_size;

            dir_space += file_space;

            // if opt_all, print space used by file
            if (opt_all) {
                // print space used by file
                printf("%lu         %s\n", file_space / 1024, path);
            }
        }
        current_entry = readdir(dir_stream);
    }

    // if opt_bytes, print total space taken by directory in bytes
    if (opt_bytes) {
        printf("%lu \n", dir_space);
    }
    // else print total space taken by directory in units ("blocks") of 1024 bytes
    else {
        printf("%lu \n", dir_space / 1024);
    }
}
