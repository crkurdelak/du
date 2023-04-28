/**
 * @file du.h
 * @brief A minimal clone of the POSIX `du` utility.
 *
 * @author Ceci Kurdelak (ckurdelak20@georgefox.edu)
 */

#ifndef DU_H
#define DU_H

/**
 * @struct du_t
 * @brief A structure to hold all the information needed to run process_dir
 */
typedef struct {
    char* dir_name;
    unsigned long cumul_total;
    bool opt_all;
    bool opt_bytes;
    bool opt_summ;
    bool opt_cumul;
} du_t;

/**
 * Finds and prints the space usage of the specified directory and any subdirectories
 * @param dir_name the filepath of the directory
 * @param opt_all true if all the files in the directory are to have their space usage printed
 * @param opt_bytes true if the space usage is to be printed in bytes instead of 1024 byte units
 * @param opt_summ true if only the space usage of the given args is to be printed
 * @param opt_cumul true if the cumulative space usage of all directories is to be printed
 * @return the total space taken by the specified directory in bytes
 */
unsigned long
process_dir(du_t dir_info);

#endif
