#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"


typedef struct node *node;
/**
 * Inserts a new node with the given key and value into the linked list.
 * @param key The key of the node.
 * @param value A pointer to the value to be stored in the node.
 * @param root A pointer to the root of the linked list.
 * @return This function does not return a value. It modifies the linked list in place.
 */
void insert_node(char *key, void *value, node *root);

/**
 * Creates a new node with the given key and value.
 * @param key The key of the node.
 * @param value A pointer to the value to be stored in the node.
 * @return A pointer to the newly created node.
 */
node create_node(char *key, void *value);

/**
 * Searches for a node in the linked list with the specified key.
 * @param root The root node of the linked list.
 * @param key The key to search for.
 * @return A pointer to the node with the specified key, or NULL if not found.
 */
node search_node(node root, const char *key);

/**
 * Prints the linked list to the specified file output stream.
 * @param output The file output stream.
 * @param list The root node of the linked list.
 * @return This function does not return a value. It writes the list to the specified output stream.
 */
void fprint_linked_list(FILE *output, node list);

/**
 * Adds a value to the specified node in the linked list.
 * @param source The node where the value will be added.
 * @param value A pointer to the value to add.
 * @return A pointer to the newly added value in the node.
 */
void *add_value(node source, void *value);

/**
 * Creates a deep copy of the given linked list.
 * @param source The root node of the linked list to copy.
 * @return A pointer to the root node of the copied linked list.
 */
node copy_list(node source);

/**
 * Retrieves the next node in the linked list.
 * @param curr The current node.
 * @return A pointer to the next node, or NULL if there is no next node.
 */
node get_next(node curr);

/**
 * Retrieves the key of the current node.
 * @param curr The current node.
 * @return A pointer to the key string.
 */
const char *get_key(node curr);

/**
 * Sets the next node in the linked list.
 * @param curr The current node.
 * @param next A pointer to the node that will be the next node.
 * @return This function does not return a value. It modifies the linked list in place.
 */
void set_next(node curr, node next);

/**
 * Retrieves the value stored in the current node.
 * @param curr The current node.
 * @return A pointer to the value stored in the node.
 */
void *get_value(node curr);

/**
 * Frees the memory allocated for the linked list, including its nodes and values.
 * @param head The root node of the linked list.
 * @param free_value A function pointer to a function that frees the values stored in the nodes.
 * @return This function does not return a value. It frees all memory associated with the linked list.
 */
void free_list(node head, void (*free_value)(void *));

/**
 * Frees a nested list stored as a value in a node.
 * @param value A pointer to the nested list to free.
 * @return This function does not return a value. It frees all memory associated with the nested list.
 */
void free_nested_list(void *value);

/**
 * Sets the value of the current node.
 * @param curr The current node.
 * @param value A pointer to the value to set.
 * @return This function does not return a value. It modifies the node in place.
 */
void set_value(node curr, void *value);

#endif
