#include "../Headers/first_pass.h"

int DC = 0;
int IC = 0;
short data_image[4096] = {0};
short code_image[4096] = {0};


struct instruction opcode_array[] = {
    {"mov", 2, {1, 1, 1, 1}, {0, 1, 1, 1}},
    {"cmp", 2, {1, 1, 1, 1}, {1, 1, 1, 1}},
    {"add", 2, {1, 1, 1, 1}, {0, 1, 1, 1}},
    {"sub", 2, {1, 1, 1, 1}, {0, 1, 1, 1}},
    {"lea", 2, {0, 1, 0, 0}, {0, 1, 1, 1}},
    {"clr", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
    {"not", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
    {"inc", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
    {"dec", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
    {"jmp", 1, {0, 0, 0, 0}, {0, 1, 1, 0}},
    {"bne", 1, {0, 0, 0, 0}, {0, 1, 1, 0}},
    {"red", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
    {"prn", 1, {0, 0, 0, 0}, {1, 1, 1, 1}},
    {"jsr", 1, {0, 0, 0, 0}, {0, 1, 1, 0}},
    {"rts", 0, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {"stop", 0, {0, 0, 0, 0}, {0, 0, 0, 0}}
};


int first_pass(const char *am_file_name, hash_table symbols, hash_table macros) {
    char line[MAX_LINE] = {0}, word_buffer[MAX_LINE] = {0}, symbol_name[MAX_SYMBOL_LENGTH] = {0}, opcode_temp[MAX_LINE] = {0};
    char *sub_holder;
    int error_flag = 0, error = 0, symbol_flag = 0, offset = 0, total_offset = 0, line_index = 0;
    FILE *am_file = fopen_with_ending(am_file_name, ".am", "r");


    while (fgets(line, MAX_LINE, am_file)) {
        line_index++;

        /* new initials for each line */
        symbol_name[0] = '\0';
        word_buffer[0] = '\0';
        opcode_temp[0] = '\0';
        symbol_flag = 0;
        offset = 0;
        total_offset = 0;
        error = 0;

        /* gets first word of line into first_word, and first_word length into offset */
        if (!sscanf(line, "%s%n", word_buffer, &offset))
            continue;


        total_offset += offset;


        /* if label detected, and no error occur, symbol flag updating to True, symbol-name updating, and the "first_word" is now the second */
        if (is_label(word_buffer, &error, macros, symbols)) {
            if (error) {
                print_error(error, line_index);
                error_flag = 1;
                continue;
            }

            symbol_flag = 1;
            strcpy(symbol_name, (sub_holder = substring(word_buffer, 0, offset-1)));
            free(sub_holder);
            sscanf(line + total_offset, "%s%n", word_buffer, &offset);
            total_offset += offset;
        }


        if (!strcmp(word_buffer, ".data") || !strcmp(word_buffer, ".string")) {
            if (symbol_flag) {
                if (!add_symbol(symbols, symbol_name, DC, 0)) {
                    print_error(18, line_index);
                }
            }

            /* distinguish between .data and .string, any errors will add to error variable */
            if (!strcmp(word_buffer, ".data")) {
                sscanf(line + total_offset, "%[^\n]", word_buffer); /* now word_buffer contain the arguments of .data */
                handle_data(word_buffer, &error);
            } else {
                sscanf(line + total_offset, "%[^\n]", word_buffer);
                handle_string(word_buffer, &error);
            }

            if (error) {
                error_flag = 1;
                print_error(error, line_index);
            }
            continue;
        }

        if (!strcmp(word_buffer, ".extern") || !strcmp(word_buffer, ".entry")) {
            if (symbol_flag) {
                print_error(10, line_index);
            }
            /* distinguish between .data and .string, any errors will add to error variable */
            if (!strcmp(word_buffer, ".extern")) {
                sscanf(line + total_offset, "%[^\n]", word_buffer);
                handle_extern(symbols, word_buffer, &error);
            } else {
                sscanf(line + total_offset, "%[^\n]", word_buffer);
                handle_entry(line + offset, &error);
            }

            if (error) {
                error_flag = 1;
                print_error(error, line_index);
            }
            continue;
        }

        if (is_opcode(word_buffer)) {
            if (symbol_flag) {
                if (!add_symbol(symbols, symbol_name, IC, 1)) {
                    print_error(18, line_index);
                }
            }
            strcpy(opcode_temp, word_buffer);

            word_buffer[0] = '\0'; /* reset word buffer */
            sscanf(line + total_offset, "%[^\n]", word_buffer);
            handle_opcode(opcode_temp, word_buffer, &error);
            if (error) {
                error_flag = 1;
                print_error(error, line_index);
            }
            continue;
        }

        /* if got here no save word represent the first word of the line, need to exit */
        error_flag = 1;
        print_error(5, line_index);
    }


    update_data_values(symbols, IC);
    IC--; /* IC (/ DC) finish the first pass with the next empty memory place, we want the last used memory place */
    DC--;

    fclose(am_file);

    return !error_flag; /* error flag is 1 if in any point of the first pass an error occur, otherwise 0 */
}

/* HANDALING SYMBOL IN LINE */
int is_label(char *name, int *error, hash_table macros, hash_table symbols) {
    int len = strlen(name);
    char *sub_holder;

    /* does is even a symbol definition? */
    if (name[len-1] != ':') {
        return 0; /* not a symbol definition */
    }

    /* if here, detected an ':', now need to check if the symbol name is OK */
    if (!check_symbol_name((sub_holder = substring(name, 0, len-1)))) {
        free(sub_holder);
        *error = 1; /* 1: error in label name */
        return 1; /* symbol, but error detected in symbol-name */
    }

    /* if got here, there is symbol definition and the symbol name is OK, need to check in macros & symbols table if already defined */
    if (search(macros, name) || search(symbols, name)) {
        *error = 2; /* 2: existing macro / symbol with the same name */
        return 1; /* symbol, but already exist in macro / symbol table */
    }

    free(sub_holder);
    /* if got here, there is a symbol, with correct name, and no exiting macro / symbol with the same name */
    return 1;
}

int check_symbol_name(char *name) {
    int len = strlen(name);

    /* checl the requirments of label name */
    if (len > 31 || !isalpha(name[0]) || !alpha_and_numeric_only_string(name)){
        return 0;
    }

    /* label name is fine and meets the requirments of label name */
    return 1;
}

int add_symbol(hash_table symbols, char *key, int count, int flag) {
    node symbol;
    struct symbol_value *value = safe_malloc(sizeof(struct symbol_value));

    if (search(symbols, key)) {
        free(value);
        return 0; /* existing symbol slreasy in symbols */
    }

    value->type[0] = 0;
    value->type[1] = 0;
    value->type[2] = 0;
    value->type[3] = 0;
    value->type[flag] = 1; /* turning on the right flag */
    value->count = count;

    symbol = create_node(key, value);
    insert(symbols, symbol);

    return 1; /* success */
}


/* HANDALING .DATA LINE */
void handle_data(char *arg, int *error) {
    char copy[MAX_LINE] = {0}, *token = NULL;
    short bin_num = 0;
    int number;
    if (!check_comma(arg)) {
        *error = 3; /* 3: error in .data line arguments */
        return;
    }

    /* arguments of .data are valid, need to code them into memory */
    strcpy(copy, arg);
    token = strtok(copy, COMMA_DELIM);

    while (token != NULL) {
        if (IC + DC > 4096) {
            print_error(24, -1);
            exit(EXIT_FAILURE);
        }
        /* Process token here */
        if (!check_data_num(token)) {
            *error = 9;
            break;
        }

        number = strtol(token, NULL, 10);
        int_to_binary(number, &bin_num);
        data_image[DC] = bin_num;
        token = strtok(NULL, COMMA_DELIM);
        DC++;
    }
}

int check_data_num(char *num) {
    /* Define a buffer to hold the copied string, with space for null terminator */
    char copy[83], *start = NULL, *end = NULL;

    /* Copy the original string to the buffer */
    strcpy(copy, num);
    start = copy;

    /* Skip leading whitespace */
    while (isspace((unsigned char)*start)) {
        start++;
    }

    /* If after skipping whitespaces the string is empty */
    if (*start == '\0') {
        return 0;  /* Invalid number */
    }

    /* Find the end of the string */
    end = start + strlen(start) - 1;

    /* Skip trailing whitespace */
    while (end > start && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }

    /* Check if the string starts with a '+' or '-' sign */
    if (*start == '+' || *start == '-') {
        start++;  /* Move to the next character */
    }

    /* Check if the string is now empty after a sign */
    if (*start == '\0') {
        return 0;  /* Invalid number (just a sign with no digits) */
    }

    /* Traverse the rest of the string and ensure all characters are digits */
    while (*start != '\0') {
        if (!isdigit((unsigned char)*start)) {
            return 0;  /* Invalid character found */
        }
        start++;
    }

    return 1;  /* The string is a valid number */
}


/* HANDALING .STRING LINE */
void handle_string(char *str, int *error) {
    char copy[MAX_LINE] = {0};
    int start = 0, end = 0;
    if (!check_string(str, &start, &end)) {
        *error = 4; /* 4: error in .string string */
        return;
    }

    /* if got here string is valid, need to code each char into memory */
    strcpy(copy, str);

    for (;start <= end; start++) {
        if (IC + DC > 4096) {
            print_error(24, -1);
            exit(EXIT_FAILURE);
        }
        data_image[DC] = copy[start];
        DC++;
    }

    /* Indication of string end */
    data_image[DC] = '\0';
    DC++;
}

/* s_index: start index, e_index: end index */

int check_string(char *str, int *s_index, int *e_index) {
    /* Define a buffer to hold the copied string, with space for null terminator AND Pointers to traverse the copied string */
    char copy[83], *p, *start, *end;

    /* Copy the original string to the buffer */
    strcpy(copy, str);

    /* Initialize pointers */
    start = copy;
    end = copy + strlen(copy) - 1;

    /* Skip leading whitespace */
    while (isspace((unsigned char)*start)) {
        start++;
    }

    /* If the string is too short to have quotes or is empty */
    if (*start == '\0' || strlen(start) < 2) {
        return 0;  /* Invalid string */
    }

    /* Skip trailing whitespace */
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }

    /* Check if the string starts and ends with double quotes */
    if (*start != '\"' || *end != '\"') {
        return 0;  /* Invalid string */
    }

    /* Store the start and end indices (excluding the quotes) */
    *s_index = (start - copy) + 1;  /* Index of the first character inside the quotes */
    *e_index = (end - copy) - 1;    /* Index of the last character inside the quotes */

    /* Traverse the characters between the quotes */
    for (p = start + 1; p < end; p++) {
        if (!isprint((unsigned char)*p)) {
            return 0;  /* Invalid character found */
        }
    }

    return 1;  /* The string is valid */
}

/* HANDALING .EXTERN LINE */
void handle_extern(hash_table symbols, const char *arg, int *error) {
    char copy[MAX_LINE], *token;
    int arg_count = 0;

    if (!check_comma(arg)) {
        *error = 16;
        return;
    }

    sscanf(arg, "%s", copy);
    token = strtok(copy, COMMA_DELIM);
    while (isspace(*token)) token++;
    while (token != NULL) {
        arg_count++;
        if (arg_count != 1) {
            *error = 13;
            return;
        }
        if (!check_symbol_name(token)) {
            *error = 1;
            return;
        }
        if (!add_symbol(symbols, token, 0, 2)) {
            *error = 18;
            return;
        }
        token = strtok(NULL, COMMA_DELIM);
    }
}


/* HANDALING .ENTRY LINE */
void handle_entry(char *arg, int *error) {
    char copy[MAX_LINE], *token;
    int arg_count = 0;

    if (!check_comma(arg)) {
        *error = 16;
        return;
    }

    strcpy(copy, arg);
    token = strtok(copy, COMMA_DELIM);
    while (isspace(*token)) token++;
    while (token != NULL) {
        arg_count++;
        if (arg_count != 1) {
            *error = 13;
            return;
        }
        if (!check_symbol_name(token)) {
            *error = 1;
            return;
        }
        token = strtok(NULL, COMMA_DELIM);
    }
}


/* HANDALING OPCODE LINE */
int is_opcode(char *word) {
    return linear_search(opcode_array, OPCODE_ARRAY_SIZE, word) != -1 ? 1 : 0;
}

void handle_opcode(char *opcode, const char *arg, int *error) {
    short inst_word = 0;
    int index = linear_search(opcode_array, OPCODE_ARRAY_SIZE, opcode), token_count = 0, curr_addressing_method = 0, ic_holder = IC, source_or_dest = 0;
    char copy[MAX_LINE], *token = NULL, reg_flag = 0;

    if (!check_comma(arg)) {
        *error = 12;
        return;
    }

    strcpy(copy, arg);
    if (index != -1) {
        inst_word = index << 11;
    } else {
        *error = 11;
        return;
    }


    token = strtok(copy, COMMA_DELIM);
    while (token != NULL) {
        /* initialization */
        curr_addressing_method = check_operand(token);

        if (empty_line(token) && (!strcmp(opcode, "stop") || !strcmp(opcode, "rts"))) {
            break;
        }

        IC++; /* word of operand is about to be written, increasing IC from the first operand for saving ic_holder for the inst_word */
        token_count++;

        if (curr_addressing_method == -1) {
            *error = 14;
            return;
        }

        if (IC + DC > 4096) {
            print_error(24, -1);
            exit(EXIT_FAILURE);
        }


        if (token_count == 1) {
            if (!is_all_zeros(opcode_array[index].legal_src, 4)) { /* this token should refer as a source operand token */
                if (!check_opcode_operand_type(source_or_dest, index, curr_addressing_method, error)) return;
                set_bit_to_one(&inst_word, curr_addressing_method + 7);
            } else { /* there is only one token and it is the dest token */
                source_or_dest = 1;
                if (!check_opcode_operand_type(source_or_dest, index, curr_addressing_method, error)) return;
                set_bit_to_one(&inst_word, curr_addressing_method + 3);

            }
        } else { /* this is the second operand, for sure its the dest */
            source_or_dest = 1;
            if (!check_opcode_operand_type(source_or_dest, index, curr_addressing_method, error)) return;
            set_bit_to_one(&inst_word, curr_addressing_method + 3);

        }

        if (token_count > get_arg_count(index)) {
            *error = 13;
            return;
        }

        /* HANDALING EACH OPERAND BY ITS ADDRESSING METHOD */
        /* insert into word in bits 10-7 source operand addressing, in 6-3 target operand addressing, in 2-0 the 'A', 'R', 'E' */
        if (curr_addressing_method == 0) { /* curr_addressing_method == 0 means its a number */
            code_image[IC] = get_number_from_operand(token) << 3;
            set_bit_to_one(&code_image[IC], 2);
        }
        else if (curr_addressing_method == 1) {
            code_image[IC] = 0; /* probably a label that defined later in code */
        }
        else { /* curr_addressing_method == 2 or 3 means its a register */
            if (source_or_dest == 0) { /* meaning its a source register */
                code_image[IC] = get_number_from_operand(token) << 6;
                reg_flag = 1; /* maybe the target operand is a register, this would help us to check later (in the next token) */
            } else {
                if (reg_flag) {
                    IC--;
                    code_image[IC] |= get_number_from_operand(token) << 3;
                } else {
                    code_image[IC] = get_number_from_operand(token) << 3;
                }
            }
            set_bit_to_one(&code_image[IC], 2);
        }

        token = strtok(NULL, COMMA_DELIM);
    }
    if (token_count != get_arg_count(index)) {
        *error = 13;
        return;
    }
    set_bit_to_one(&inst_word, 2); /* 'ARE' of instruction line is alwats A */
    code_image[ic_holder] = inst_word;
    IC++; /* if IC not increased here, next line will overwrite the last operand word of this line */
}


void set_bit_to_one(short *src, int index) {
    *src = *src | 1 << index;
}

int check_operand(char *token) {
    char copy[MAX_LINE];
    int i = 0;
    strcpy(copy, token);

    while (isspace(copy[i])) i++; /* increase token also outside of check_operand for skipping leading spaces */

    if (copy[i] == '#') {
        return check_data_num(token + i + 1) ? 0 : -1;
    }
    if (copy[i] == '*') {
        return check_register_name(token + i + 1) ? 2 : -1;
    }
    if (copy[i] == 'r') {
        return check_register_name(token + i) ? 3 : -1;
    }
    return check_symbol_name(token + i) ? 1 : -1;
}

int check_register_name(char *name) {
    char copy[MAX_LINE];
    int i = 0;
    strcpy(copy, name);

    if (copy[i] != 'r') return 0;
    i++;
    if (copy[i] != '0' && copy[i] != '1' && copy[i] != '2' && copy[i] != '3' && copy[i] != '4' && copy[i] != '5' && copy[i] != '6' && copy[i] != '7') return 0;
    i++;
    while (copy[i] != '\0') {
        if (!isspace(copy[i])) return 0;
        i++;
    }

    return 1;
}

int get_number_from_operand(char *token) {
    char *copy = token;

    while (*copy != '#' && *copy != 'r') copy++;
    return atoi(copy+1);
}

int check_opcode_operand_type(int dest_flag, int index, int addressing_method, int *error) {
    if ((!dest_flag && opcode_array[index].legal_src[addressing_method] == 0) || (dest_flag && opcode_array[index].legal_dst[addressing_method] == 0)) { /* the operand type does not match the opcode allowed types */
        *error = 15;
        return 0;
    }

    return 1;
}


/* MULTIPLE-USE FUNCTIONS */
bool check_comma(const char *arg) {
    bool expect_comma = false;
    while (*arg != '\0') {
        if (isspace(*arg)) {
            arg++;
        } else if (*arg == ',') {
            if (empty_line((char *)arg+1) | !expect_comma) return false;
            expect_comma = false;
            arg++;
        } else {
            if (expect_comma) return false;
            while (*arg != ',' && !isspace(*arg) && *arg != '\0') {
                arg++;
            }
            expect_comma = true;
        }
    }
    return true;
}

int get_arg_count(int opcode_index) {
    return opcode_array[opcode_index].arg_count;
}

