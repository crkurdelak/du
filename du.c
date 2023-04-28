/**
 * @file du.c
 * @brief A minimal clone of the POSIX `du` utility.
 *
 * @author Ceci Kurdelak (ckurdelak20@georgefox.edu)
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
    bool opt_summ = false;
    bool opt_cumul = false;
    bool opts_valid = true;
    opterr = 0;
    int opt;

    unsigned long dir_space = 0;

    while ((opt = getopt(argc, argv, "absc")) != -1) {
        switch (opt) {
            case 'a':
                opt_all = true;
                break;
            case 'b':
                opt_bytes = true;
                break;
            case 's':
                opt_summ = true;
                break;
            case 'c':
                opt_cumul = true;
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
    else {
        // put together struct to call process_dir
        du_t dir_info;
        dir_info.cumul_total = 0;           // initialize as 0
        dir_info.opt_all = opt_all;
        dir_info.opt_bytes = opt_bytes;
        dir_info.opt_summ = opt_summ;
        dir_info.opt_cumul = opt_cumul;

        // determine if there are any additional arguments remaining
        if (optind == argc) {
            // no file/directory names provided,
            // so use current working directory
            char *working_dirname = ".";
            // call recursive fn
            dir_info.dir_name = working_dirname;
            dir_space = process_dir(&dir_info);
            printf(".\n");
        } else {
            // one or more file/directory names were provided,
            // so start where getopt() left off and iterate through them
            for (int i = optind; i < argc; i++) {
                dir_info.dir_name = argv[i];
                // call recursive fn
                if (opt_summ) {
                    // TODO if opt_summ create thread for each arg, call process_dir on it, (struct w all
                    //  args, incl cumulative total)
                }
                else {
                    dir_space = process_dir(&dir_info);
                }
                if (opt_summ) {
                    if (opt_bytes) {
                        printf("%lu         %s\n", dir_space, dir_info.dir_name);
                    }
                        // else print total space taken by directory in units ("blocks") of 1024 bytes
                    else {
                        printf("%lu         %s\n", dir_space / 1024, dir_info.dir_name);
                    }
                }
            }
        }
        if (opt_cumul) {
            if (opt_bytes) {
                printf("%lu\n", dir_info.cumul_total);
            }
            else {
                // print in "blocks"
                printf("%lu\n", dir_info.cumul_total / 1024);
            }
        }
    }
}

unsigned long
process_dir(du_t *dir_info) {
    // extract information from dir_info
    char* dir_name = dir_info->dir_name;
    bool opt_all = dir_info->opt_all;
    bool opt_bytes = dir_info->opt_bytes;
    bool opt_summ = dir_info->opt_summ;
    bool opt_cumul = dir_info->opt_cumul;

    unsigned long dir_space = 0;
    unsigned long file_space = 0;
    struct stat stat_buf;
    DIR* dir_stream = opendir(dir_name);
    if (dir_stream) {
        // get first entry in directory
        struct dirent *current_entry = readdir(dir_stream);
        while (current_entry) {
            // if file is a directory, descend into directory and process its files (call process_dir())
            if (current_entry->d_type == DT_DIR) {
                if (strcmp(current_entry->d_name, ".") != 0 &&
                    strcmp(current_entry->d_name, "..") !=
                    0) {
                    //char* next_dir = *dir_name + "/" + current_entry->d_name;
                    char current_path[100];
                    strcpy(current_path, dir_name);
                    strcat(current_path, "/");
                    strcat(current_path, current_entry->d_name);

                    // put together new struct for recursive call
                    du_t new_dir_info;
                    new_dir_info.dir_name = current_path;
                    new_dir_info.cumul_total = dir_info->cumul_total;
                    new_dir_info.opt_all = opt_all;
                    new_dir_info.opt_bytes = opt_bytes;
                    new_dir_info.opt_summ = opt_summ;
                    new_dir_info.opt_cumul = opt_cumul;

                    dir_space += process_dir(&new_dir_info);
                }
            } else {
                // find file system space used by file

                // build path
                char current_path[100];
                strcpy(current_path, dir_name);
                strcat(current_path, "/");
                strcat(current_path, current_entry->d_name);
                stat(current_path, &stat_buf);
                file_space = stat_buf.st_size;

                dir_space += file_space;

                // if opt_all, print space used by file
                if (opt_all && !opt_summ) {
                    if (opt_bytes) {
                        printf("%lu         %s\n", file_space, current_path);
                    }
                    else {
                        // print space used by file
                        printf("%lu         %s\n", file_space / 1024, current_path);
                    }
                }
            }
            current_entry = readdir(dir_stream);
        }

        // if opt_bytes, print total space taken by directory in bytes
        if (opt_bytes && !opt_summ) {
            printf("%lu         %s\n", dir_space, dir_name);
        }
        // else print total space taken by directory in units ("blocks") of 1024 bytes
        else {
            // if it's a summary, don't print space of individual directories
            if (!opt_summ) {
                printf("%lu         %s\n", dir_space / 1024, dir_name);
            }
        }
    }
    dir_info->cumul_total += dir_space;
    return dir_space;
}
