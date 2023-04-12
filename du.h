/**
 * @file du.h
 * @brief A minimal clone of the POSIX `du` utility.
 *
 * @author Brian R. Snider (bsnider@georgefox.edu)
 */

#ifndef DU_H
#define DU_H

// TODO declare and document helper methods here
/**
 * Finds and prints the space usage of the specified directory and any subdirectories
 * @param dir_stream the directory stream
 * @param path the filepath of the directory
 * @param opt_all true if all the files in the directory are to have their space usage printed
 * @param opt_bytes true if the space usage is to be printed in bytes instead of 1024 byte units
 */
void process_dir(DIR *dir_stream, char *path, bool opt_all, bool opt_bytes);

#endif
