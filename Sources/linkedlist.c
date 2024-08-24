#include "../Headers/linkedlist.h"

struct node {
    char *key;
    void *value;
    struct node *next;
};

void insert_node(char *key, void *value, node *root) {
    node new = create_node(key, value), curr; /* what CONST means? */
    if (*root == NULL)
        *root = new;
    else {
        curr = *root;
        while(curr->next != NULL)
            curr = curr->next;

        curr->next = new;
    }
}

/* returns a new node with key and value allocated by the user */
node create_node(char *key, void *value) { /* what CONST means? */
    node new_node = safe_malloc(sizeof(struct node));
    new_node->key = strdup(key);

    new_node->value = value; /* Needs to be changed, cannot use strdup() when value is void* parameter */

    new_node->next = NULL;
    return new_node;
}

node search_node(node root, const char *key) {
    while (root != NULL) {
        if (!strcmp(root->key, key)) {
            return root;
        }
        root = root->next;
    }
    return NULL;
}

void fprint_linked_list(FILE *output, node list) {
    while (list != NULL) {
        fprintf(output, "%s", get_key(list));
        list = list->next;
    }
}

/* gets: node head and frees all subsequens node */
void free_list(node head, void (*free_value)(void *)) {
    while (head != NULL) {
        node temp = head;
        if (head->key) {
            free(head->key);
        }

        if (head->value != NULL) {
            if (free_value) {
                free_value(head->value);
            } else {
                free(head->value);
            }
        }

        head = head->next;
        free(temp);
    }
}



void free_nested_list(void *value) {
    node nested_head = (node)value;  /* Use appropriate type casting */
    if (nested_head != NULL) {
        free_list(nested_head, NULL);
    }
}


node get_next(node curr) {
    return curr->next;
}

const char *get_key(const node curr) {
    return curr->key;
}

void set_next(node curr, node next) {
    curr->next = next;
}

void *get_value(node curr) {
    return curr != NULL ? curr->value: NULL;
}

void set_value(node curr, void *value) {
    curr->value = value;
}