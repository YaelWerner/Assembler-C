#include "../Headers/hashtable.h"

#include "../Headers/first_pass.h"

/* djb2 */
unsigned long hash(const unsigned char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % HASH_TABLE_SIZE;
}

void insert(hash_table source, const node to_insert) {
    unsigned long index = hash((const unsigned char *)get_key(to_insert));
    node curr = *(source + index);
    if (curr == NULL)
        *(source + index) = to_insert;

    else {
        while (get_next(curr) != NULL) {
            curr = get_next(curr);
        }
        set_next(curr, to_insert);
    }
}

void *search(const hash_table source,const char *key) {
    node found;
    unsigned long index = hash((const unsigned char *)key);
    found = search_node(*(source + index), key);
    return found != NULL ? get_value(found) : NULL;
}

void update_data_values(const hash_table table, const int IC) {
    int i;
    node curr;
    struct symbol_value *val = NULL;
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        curr = table[i];
        while (curr != NULL) {
            val = (struct symbol_value *)get_value(curr);
            if (val->type[0]) {
                val->count += IC + 100;
            } else if (val->type[1]) {
                val->count += 100;
            }
            curr = get_next(curr);
        }
    }
}

void free_table(hash_table src, int flag) {
    int i;
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        if (src[i] != NULL) {
            if (flag) {
                free_list(src[i], free_nested_list); /* 1: free key */
            } else {
                free_list(src[i], NULL); /* key is always added with strdup, and we need to change it as long its not null */
            }
        }
    }
}

