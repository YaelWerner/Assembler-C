#ifndef UTIL_H
#define UTIL_H

#define MAX_LINE 82
#define ACTION_CODE_COUNT 16

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct instruction {
    char *name;
    int arg_count;
    int legal_src[4];
    int legal_dst[4];
};

/**
 * Opens a file with the specified file name and ending, in the given mode.
 * @param source_file_name The base name of the file.
 * @param ending The file extension or ending to append.
 * @param mode The mode in which to open the file (e.g., "r", "w").
 * @return A pointer to the opened file, or NULL if the file could not be opened.
 */
FILE *fopen_with_ending(const char *source_file_name, const char *ending, const char *mode);

/**
 * Creates a new file name by appending the specified ending to the source file name.
 * @param source_file_name The base name of the file.
 * @param ending The file extension or ending to append.
 * @return A pointer to the newly created file name string. The caller is responsible for freeing the memory.
 */
char *create_file_name_with_ending(const char *source_file_name, const char *ending);

/**
 * Duplicates a string by allocating memory and copying the contents.
 * @param s The string to duplicate.
 * @return A pointer to the newly allocated string, or NULL if allocation fails.
 */
char *strdup(const char *s);

/**
 * Allocates memory safely, exiting the program if the allocation fails.
 * @param size The size of the memory to allocate.
 * @return A pointer to the allocated memory.
 */
void *safe_malloc(size_t size);

/**
 * Extracts a substring from the source string, starting at the given index and ending at the specified index.
 * @param source The source string.
 * @param start The starting index of the substring.
 * @param end The ending index of the substring.
 * @return A pointer to the newly created substring. The caller is responsible for freeing the memory.
 */
char *substring(const char* source, int start, int end);

/**
 * Checks if the given string contains only alphabetic and numeric characters.
 * @param word The string to check.
 * @return An integer indicating if the string is valid (1) or not (0).
 */
int alpha_and_numeric_only_string(char *word);

/**
 * Converts an integer value to its binary representation and stores it in a short array.
 * @param value The integer value to convert.
 * @param binary A pointer to the short array to store the binary representation.
 * @return This function does not return a value. It modifies the binary array in place.
 */
void int_to_binary(int value, short *binary);

/**
 * Prints the binary representation of a short integer value.
 * @param value The short integer value to print.
 * @return This function does not return a value. It prints the binary value to the standard output.
 */
void print_binary(short value);

/**
 * Performs a linear search on an array of instruction structs to find a target string.
 * @param arr The array of instruction structs to search.
 * @param size The size of the array.
 * @param target The target string to search for.
 * @return The index of the target in the array, or -1 if not found.
 */
int linear_search(struct instruction arr[], int size, const char *target);

/**
 * Checks if all elements in the given integer array are zeros.
 * @param arr The integer array to check.
 * @param size The size of the array.
 * @return An integer indicating if the array contains only zeros (1) or not (0).
 */
int is_all_zeros(int arr[], int size);

/**
 * Prints the contents of an integer array.
 * @param arr The integer array to print.
 * @param size The size of the array.
 * @return This function does not return a value. It prints the array contents to the standard output.
 */
void print_array(const int arr[], int size);

#endif
