#ifndef PRE_PROC_H
#define PRE_PROC_H

#include <stdio.h>
#include <string.h>
#include "hashtable.h"
#include "util.h"
#include "error_handle.h"

/**
 * Searches for macro definitions in the specified file and stores them in a hash table.
 * @param file_name A pointer to a string containing the name of the file to search for macros.
 * @param macros A hash table where macro definitions will be stored.
 * @param main_error A pointer to an integer that stores any error codes encountered during the search.
 * @return This function does not return a value. It modifies the macros hash table and updates main_error if an error occurs.
 */
void macro_search(char *file_name, hash_table macros, int *main_error);

/**
 * Extracts the name of the macro from a line that follows a `macr` keyword.
 * @param line_after_macr A pointer to a string containing the line of text after the `macr` keyword.
 * @return A pointer to a string containing the extracted macro name. The returned string should be freed by the caller.
 */
char *get_macro_name(char *line_after_macr);

/**
 * Processes a macro definition or usage within a given line.
 * @param curr_line A pointer to a node representing the current line being processed.
 * @param line A pointer to a string containing the line of text that may contain a macro.
 * @return This function does not return a value. It modifies the contents of curr_line based on the macro handling.
 */
void handle_macro(node *curr_line, char *line);

/**
 * Removes leading whitespace characters from a given line of text.
 * @param line A pointer to a string representing the line of text to be processed.
 * @return This function does not return a value. It modifies the line in place, removing any leading whitespace.
 */
void remove_leading_whitespace(char *line);

#endif
