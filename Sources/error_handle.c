#include "../Headers/error_handle.h"

const char *error_messages[] = {
    "No error",    /* 0 - No error */
    "symbol name does not match requirements",    /* -- 1 -- */
    "existing macro / symbol with the same name",   /* -- 2 -- */
    ".data line arguments",    /* --3 -- */
    ".string given string",    /* -- 4 -- */
    "not a saved word",    /* -- 5 -- */
    "another word in 'endmacr' line",   /* -- 6 -- */
    "macro call line has more chars after than the macro-call",    /* -- 7 -- */
    "macro definition line is not non-whitespace characters after macro name",    /* -- 8 -- */
    "one of the numbers of .data line written incorrectly",    /* -- 9 -- */
    "a label detected inside .extern or .entry line",    /* -- 10 -- */
    "cannot find the opcode u searching for",    /* -- 11 -- */
    "one of the numbers of opcode line written incorrectly",    /* -- 12 -- */
    "too many / few arguments for this opcode",    /* -- 13 -- */
    "one or more operands are not declared currectly",    /* -- 14 -- */
    "the operand type does not match the opcode allowed types", /* -- 15 -- */
    "commas for this line are set incorrectly", /* -- 16 -- */
    "one or more errors occur in first-pass, moving to next file", /* -- 17 -- */
    "existing symbol already in symbols", /* -- 18 -- */
    "symbol was not found in symbol table", /* -- 19 -- */
    "label with both external and entry flags", /* -- 20 -- */
    "detected a too-long line", /* -- 21 -- */
    "failed to open .as / .asm file", /* -- 22 -- */
    "already macro with the same name || macro name is one of the 16 opcodes", /* -- 23 -- */
    "memory is over", /* -- 24 -- */
    "could not pre procced the as_file, moving to next file", /* -- 25 -- */
    "macro name is 'macr' or 'endmacr'", /* -- 26 -- */
    "macro name is instruction word", /* -- 27 -- */
};

int empty_line(char *line){
    char copy[MAX_LINE];
    int i = 0;
    strcpy(copy, line);
    while (copy[i] != '\0') {
        if (!isspace(copy[i])) return 0;
        i++;
    }
    return 1;
}

void print_error(int error_number, int line_number) {
    if (error_number < 0 || error_number >= sizeof(error_messages) / sizeof(error_messages[0])){
        printf("Unknown error at line %d\n", line_number);
    } else {
        printf("\033[1;31mError at line %d: %s\033[0m\n", line_number, error_messages[error_number]);
    }
}


int check_macr(char *line_after_macr, int line_count, hash_table macros) {
    char macro_name[MAX_LINE];
    int offset = 0;
    sscanf(line_after_macr, "%s%n", macro_name, &offset);

    if (!empty_line(line_after_macr + offset)) {
        print_error(8, line_count); /* macro definition line is not non-whitespace characters after macro name */
        return 0;
    }

    if (search(macros, macro_name) != NULL || is_opcode(macro_name)) {
        print_error(23, line_count);
        return 0;  /*already macro with the same name || macro name is one of the 16 opcodes*/
    }

    if (!strcmp(macro_name, ".entry") || !strcmp(macro_name, ".entry") || !strcmp(macro_name, ".entry") || !strcmp(macro_name, ".entry")) {
        print_error(27, line_count);
        return 0;
    }

    if (!strcmp(macro_name, "macr") || !strcmp(macro_name, "endmacr")) {
        print_error(26, line_count);
        return 0;
    }
    return 1;
}

int length_check(char *line) {
    size_t len = strlen(line);
    if (len == MAX_LINE - 1 && line[len - 1] != '\n') {
        return 0;
    }
    return 1;
}