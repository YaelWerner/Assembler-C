#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include <stdio.h>
#include "first_pass.h"

/**
 * Executes the second pass over the assembly file to finalize symbol addresses and process entries/external labels.
 * @param am_file_name The name of the assembly file.
 * @param symbols A hash table containing symbols from the first pass.
 * @return An integer indicating success (0) or failure (non-zero).
 */
int second_pass(char *am_file_name, hash_table symbols);

/**
 * Activates an entry symbol in the symbol table, marking it as an entry point.
 * @param symbols A hash table containing symbols.
 * @param arg The argument representing the entry symbol name.
 * @param error Pointer to an integer where an error code will be stored if any issues are found.
 * @return This function does not return a value. It modifies the symbol table and updates the error code.
 */
void activate_entry(hash_table symbols, char *arg, int *error);

/**
 * Completes the processing of an opcode by resolving its operands and updating the code image.
 * @param arg The arguments of the opcode.
 * @param error Pointer to an integer where an error code will be stored if any issues are found.
 * @param symbols A hash table containing symbols used by the opcode.
 * @param ext_flag Pointer to an integer indicating if the opcode involves an external symbol.
 * @param ext_name The name of the external symbol, if applicable.
 * @return This function does not return a value. It modifies the code image and updates the error code and external flag.
 */
void complete_opcode(const char *arg, int *error, hash_table symbols, int *ext_flag, const char *ext_name);

/**
 * Converts a short integer to a 5-digit octal string representation.
 * @param num The short integer to convert.
 * @return A pointer to a string containing the 5-digit octal representation of the number. The returned string should be freed by the caller.
 */
char *print_only_5_octal(short num);

#endif
