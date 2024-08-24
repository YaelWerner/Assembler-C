#ifndef ERROR_HANDLE_H
#define ERROR_HANDLE_H

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "first_pass.h"

extern const char *error_messages[];

/**
 * Checks if the given line is empty or consists only of whitespace characters.
 * @param line A pointer to the line to check.
 * @return An integer indicating if the line is empty (1) or not (0).
 */
int empty_line(char *line);

/**
 * Prints an error message corresponding to the given error number and line number.
 * @param error_num The error number to print.
 * @param line The line number where the error occurred.
 * @return This function does not return a value. It outputs the error message to the standard output or log.
 */
void print_error(int error_num, int line);

/**
 * Checks if the line contains a valid macro definition and updates the macros hash table.
 * @param line_after_macr The line after the macro definition keyword.
 * @param line_count The current line number in the file.
 * @param macros The hash table containing macros.
 * @return An integer indicating if the macro is valid (0) or if an error occurred (non-zero).
 */
int check_macr(char *line_after_macr, int line_count, hash_table macros);

/**
 * Checks if the given line exceeds the maximum allowed length.
 * @param line A pointer to the line to check.
 * @return An integer indicating if the line is valid (0) or if it exceeds the length limit (non-zero).
 */
int length_check(char *line);

#endif
