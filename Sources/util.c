#include "../Headers/util.h"




FILE *fopen_with_ending(const char *source_file_name, const char *ending, const char *mode) {
    char *file_name = create_file_name_with_ending(source_file_name, ending);
    FILE *fp = NULL;
    fp = fopen(file_name, mode);
    if(fp == NULL) {
        perror("Error");
    }
    free(file_name);
    return fp;
}

char *create_file_name_with_ending(const char *source_file_name, const char *ending) {
    size_t len1 = strlen(source_file_name), len2 = strlen(ending);
    char *result = safe_malloc(len1 + len2 + 1);

    strcpy(result, source_file_name);
    strcat(result, ending);

    return result;
}

/* Implementation of strdup */
char *strdup(const char *s) {
    /* Find the length of the string */
    size_t len = strlen(s);

    /* Allocate memory for the duplicate string (+1 for the null terminator) */
    char *dup = (char *)malloc((len + 1) * sizeof(char));

    /* Check if memory allocation was successful */
    if (dup == NULL) {
        return NULL; /* Allocation failed */
    }

    /* Copy the string to the allocated memory */
    strcpy(dup, s);

    /* Return the duplicated string */
    return dup;
}

void *safe_malloc(size_t size) {
    void *mem = malloc(size);
    if (mem == NULL) {
        perror("\033[1;33mError\033[0m");
        exit(EXIT_FAILURE);
    }
    return mem;
}

/* Function to extract a substring */
char *substring(const char* source, int start, int end) {
    int length;
    char *substr;

    /* Check for valid indices */
    if (start < 0 || end > strlen(source) || start > end) {
        printf("\033[1;31mInvalid start or end indices of substring.\033[0m\n");
        return NULL;
    }

    /* Calculate the length of the substring */
    length = end - start;

    /* Allocate memory for the substring */
    substr = (char*)safe_malloc((length + 1) * sizeof(char));


    /* Copy the substring */
    strncpy(substr, source + start, length);

    /* Add the null terminator */
    substr[length] = '\0';

    return substr;
}


int alpha_and_numeric_only_string(char *word) {
    while (*word) {
        if (!isalpha(*word) && !isdigit(*word) && !isspace(*word))
            return 0;
        word++;
    }
    return 1;
}


void int_to_binary(int value, short *binary) {
    int i;
    *binary = 0;
    for (i = 0; i < sizeof(short) * 8; i++) {
        if (value & (1 << i)) {
            *binary |= (1 << i);
        }
    }
}

void print_binary(short value) {
    int bit_size = sizeof(short) * 8, i;

    for (i = bit_size - 1; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
    printf(" = %d\n", value);
}

int linear_search(struct instruction arr[], int size, const char *target) {
    int i;
    for (i = 0; i < size; i++) {
        if (strcmp(arr[i].name, target) == 0) {
            return i;
        }
    }
    return -1;
}

int is_all_zeros(int arr[], int size) {
    int i;
    for (i = 0; i < size; i++) {
        if (arr[i] != 0) {
            return 0;
        }
    }
    return 1;
}

void print_array(const int arr[], int size) {
    int i;

    printf("[");
    for (i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("]");
}