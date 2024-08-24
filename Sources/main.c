#include "../Headers/main.h"

int main(int argc, char *argv[]) {
    while(--argc > 0) {
        hash_table macros = {0}, symbols = {0};
        int error = 0;
        printf(PINK_COLOR "working on %s\n" RESET, argv[argc]);
        printf(BLUE_COLOR "start pre-preccesing, fetching macros\n" RESET);

        /* macro_search(FILE *as_file) - search for macro definition line-by-line, if found - adding them to the hashTable - add_to_hash(char *key, char *value) */
        macro_search(argv[argc], macros, &error);
        if (error) {
            print_error(25, 0); /* error in line 1 means in 1 / more lines */
            continue;
        }

        printf(BLUE_COLOR "starting first pass\n" RESET);
        if (!first_pass(argv[argc], symbols, macros)) { /* if an error occur in first_pass, should move to next file */
            print_error(17, -1);
            continue;
        }
        

        printf(BLUE_COLOR "starting second pass\n" RESET);
        if (!second_pass(argv[argc], symbols)) {
            print_error(18, -1);
            continue;
        }


        free_table(macros, 1);
        free_table(symbols, 0);
    }

    printf(BLUE_COLOR "Done\n" RESET);
    return 0;
}
