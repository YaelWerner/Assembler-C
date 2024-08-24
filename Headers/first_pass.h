#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>
#include "hashtable.h"
#include "error_handle.h"

#define MAX_SYMBOL_LENGTH 31
#define OPCODE_ARRAY_SIZE 16
#define COMMA_DELIM ","

extern int DC, IC;
extern short data_image[4096];
extern short code_image[4096];

typedef enum {
 false, true
} bool;

struct symbol_value {
 int type[4]; /* 0: data, 1: opcode, 2: external, 3: entry */
 int count; /* DC for data, IC for opcode */
};

extern struct instruction opcode_array[];

/**
 * Executes the first pass over the assembly file to collect labels, symbols, and macro information.
 * @param am_file_name The name of the assembly file.
 * @param symbols A hash table to store symbols found during the first pass.
 * @param macros A hash table to store macros found during the first pass.
 * @return An integer indicating success (0) or failure (non-zero).
 */
int first_pass(const char *am_file_name, hash_table symbols, hash_table macros);

/**
 * Checks if a given name is a valid label and handles it if so.
 * @param name The name to check.
 * @param error Pointer to an integer where an error code will be stored if any issues are found.
 * @param macros A hash table containing macro definitions.
 * @param symbols A hash table containing symbol definitions.
 * @return An integer indicating success (0) or failure (non-zero).
 */
int is_label(char *name, int *error, hash_table macros, hash_table symbols);

/**
 * Validates if the symbol name follows the correct conventions.
 * @param name The symbol name to check.
 * @return An integer indicating success (0) or failure (non-zero).
 */
int check_symbol_name(char *name);

/**
 * Adds a new symbol to the symbol hash table.
 * @param symbols The hash table where the symbol will be added.
 * @param key The symbol name.
 * @param count The current data counter (DC) or instruction counter (IC).
 * @param flag The type of symbol (e.g., data, opcode).
 * @return An integer indicating success (0) or failure (non-zero).
 */
int add_symbol(hash_table symbols, char *key, int count, int flag);

/**
 * Handles the processing of a data directive and updates the data image.
 * @param arg The argument of the data directive.
 * @param error Pointer to an integer where an error code will be stored if any issues are found.
 * @return This function does not return a value. It modifies the data image and updates the error code.
 */
void handle_data(char *arg, int *error);

/**
 * Checks if a given string contains a valid comma.
 * @param arg The string to check.
 * @return A boolean value indicating true if the comma is valid, false otherwise.
 */
bool check_comma(const char *arg);

/**
 * Handles the processing of a string directive and updates the data image.
 * @param str The string argument.
 * @param error Pointer to an integer where an error code will be stored if any issues are found.
 * @return This function does not return a value. It modifies the data image and updates the error code.
 */
void handle_string(char *str, int *error);

/**
 * Validates the format of a string and identifies its start and end indices.
 * @param str The string to check.
 * @param s_index Pointer to an integer where the start index of the string will be stored.
 * @param e_index Pointer to an integer where the end index of the string will be stored.
 * @return An integer indicating success (0) or failure (non-zero).
 */
int check_string(char *str, int *s_index, int *e_index);

/**
 * Handles the processing of an extern directive and updates the symbol table.
 * @param symbols The symbol hash table to update.
 * @param arg The argument of the extern directive.
 * @param error Pointer to an integer where an error code will be stored if any issues are found.
 * @return This function does not return a value. It modifies the symbol table and updates the error code.
 */
void handle_extern(hash_table symbols, const char *arg, int *error);

/**
 * Handles the processing of an entry directive.
 * @param arg The argument of the entry directive.
 * @param error Pointer to an integer where an error code will be stored if any issues are found.
 * @return This function does not return a value. It updates the error code based on the processing results.
 */
void handle_entry(char *arg, int *error);

/**
 * Checks if a given word is a valid opcode.
 * @param word The word to check.
 * @return An integer representing the index of the opcode if valid, or -1 if invalid.
 */
int is_opcode(char *word);

/**
 * Handles the processing of an opcode and its arguments.
 * @param opcode The opcode to handle.
 * @param arg The arguments of the opcode.
 * @param error Pointer to an integer where an error code will be stored if any issues are found.
 * @return This function does not return a value. It updates the code image and error code.
 */
void handle_opcode(char *opcode, const char *arg, int *error);

/**
 * Validates if a given string is a valid data number.
 * @param str The string to check.
 * @return An integer indicating success (0) or failure (non-zero).
 */
int check_data_num(char *str);

/**
 * Retrieves the argument count for a given opcode based on its index.
 * @param opcode_index The index of the opcode in the opcode array.
 * @return The number of arguments required by the opcode.
 */
int get_arg_count(int opcode_index);

/**
 * Sets a specific bit to 1 in a short integer at the given index.
 * @param src Pointer to the short integer.
 * @param index The index of the bit to set.
 * @return This function does not return a value. It modifies the short integer in place.
 */
void set_bit_to_one(short *src, int index);

/**
 * Checks if a given token is a valid operand.
 * @param token The token to check.
 * @return An integer indicating success (0) or failure (non-zero).
 */
int check_operand(char *token);

/**
 * Checks if a given name is a valid register name.
 * @param name The name to check.
 * @return An integer indicating success (0) or failure (non-zero).
 */
int check_register_name(char *name);

/**
 * Extracts a number from a given operand token.
 * @param token The operand token containing the number.
 * @return The extracted number as an integer.
 */
int get_number_from_operand(char *token);

/**
 * Validates the operand type for a given opcode and addressing method.
 * @param dest_flag The flag indicating the destination type.
 * @param index The index of the opcode in the opcode array.
 * @param addressing_method The addressing method used by the operand.
 * @param error Pointer to an integer where an error code will be stored if any issues are found.
 * @return An integer indicating success (0) or failure (non-zero).
 */
int check_opcode_operand_type(int dest_flag, int index, int addressing_method, int *error);

#endif
