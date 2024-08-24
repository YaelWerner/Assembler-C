#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "linkedlist.h"

#define HASH_TABLE_SIZE 100


typedef node hash_table[HASH_TABLE_SIZE];
/**
 * Generates a hash value for the given string, which is used as the index in the hash table.
 * @param str A pointer to the string to hash.
 * @return An unsigned long representing the hash value of the string.
 */
unsigned long hash(const unsigned char *str);

/**
 * Inserts a node into the hash table based on the key stored in the node.
 * @param source The hash table where the node will be inserted.
 * @param to_insert A pointer to the node to insert into the hash table.
 * @return This function does not return a value. It modifies the hash table in place.
 */
void insert(hash_table source, const node to_insert);

/**
 * Searches for a value in the hash table by its key.
 * @param source The hash table to search.
 * @param key A pointer to the key string to search for.
 * @return A pointer to the value associated with the key, or NULL if the key is not found.
 */
void *search(const hash_table source, const char *key);

/**
 * Frees all memory allocated for the hash table, including all nodes and values.
 * @param src The hash table to free.
 * @param flag An integer flag that determines how values are freed (e.g., whether nested structures need to be freed).
 * @return This function does not return a value. It frees all memory associated with the hash table.
 */
void free_table(hash_table src, int flag);

/**
 * Updates the data values in the hash table by adding the given IC value to each relevant entry.
 * @param table The hash table to update.
 * @param IC The value to add to the data entries.
 * @return This function does not return a value. It modifies the hash table in place.
 */
void update_data_values(const hash_table table, const int IC);

#endif
