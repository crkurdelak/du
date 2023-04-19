/**
 * @file du.h
 * @brief A minimal clone of the POSIX `du` utility.
 *
 * @author Ceci Kurdelak (ckurdelak20@georgefox.edu)
 */

#ifndef DU_H
#define DU_H

/**
 * Finds and prints the space usage of the specified directory and any subdirectories
 * @param dir_name the filepath of the directory
 * @param opt_all true if all the files in the directory are to have their space usage printed
 * @param opt_bytes true if the space usage is to be printed in bytes instead of 1024 byte units
 * @return the total space taken by the specified directory in bytes
 */
unsigned long process_dir(char *dir_name, bool opt_all, bool opt_bytes);

#endif
