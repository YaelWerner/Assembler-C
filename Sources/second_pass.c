#include "../Headers/second_pass.h"

/* -- THIS IS SECOND PASS -- */
int second_pass(char *am_file_name, hash_table symbols) {
    FILE *am_file = fopen_with_ending(am_file_name, ".am", "r"), *ob_file = fopen_with_ending(am_file_name, ".ob", "w"), *ent_file;
    char line[MAX_LINE] = {0}, word_buffer[MAX_LINE] = {0}, *octal_str, *sub_holder;
    int error = 0, offset, total_offset, line_index = -1, i, ext_flag = 0, ent_flag = 0;
    struct symbol_value *temp = NULL;
    IC = -1;

    while (fgets(line, MAX_LINE, am_file)) {
        /* Initials */
        word_buffer[0] = '\0';
        offset = 0;
        total_offset = 0;
        error = 0;
        line_index++;
        temp = NULL;

        if (!sscanf(line + offset, "%s%n", word_buffer, &offset))
            continue;

        total_offset += offset;


        if (search(symbols, (sub_holder = substring(word_buffer, 0, offset-1)))) { /* first word is label */
            sscanf(line + total_offset, "%s%n", word_buffer, &offset);
            total_offset += offset;
        }
        free(sub_holder);

        /* if got here: word_buffer should hold the first word of the line (after label if have) */
        if (!strcmp(word_buffer, ".data") || !strcmp(word_buffer, ".string")) {
            continue;
        }

        if (!strcmp(word_buffer, ".extern")) {
            continue;
        }

        if (!strcmp(word_buffer, ".entry")) {
            sscanf(line + total_offset, "%s%n", word_buffer, &offset);
            activate_entry(symbols, word_buffer, &error);
            total_offset += offset;

            if (ent_flag) ent_file = fopen_with_ending(am_file_name, ".ent", "a");
            else ent_file = fopen_with_ending(am_file_name, ".ent", "w");
            sscanf(line + total_offset, "%s", word_buffer); /* word_buffer now holds perfectly the external symbol name */
            temp = search(symbols, word_buffer);
            fprintf(ent_file, "%s %d\n", word_buffer, temp->count);
            fclose(ent_file);
            ent_flag = 1;
        }

        else { /* opcode word */
            complete_opcode(line + total_offset, &error, symbols, &ext_flag, am_file_name);
        }

        if (error) {
            print_error(error, line_index);
        }
    }



    fprintf(ob_file, "%d %d\n", IC + 1, DC + 1);
    for (i = 0; i <= IC; i++) {
        octal_str = print_only_5_octal(code_image[i]);
        fprintf(ob_file, "%04d %s\n", i+100, octal_str);
        free(octal_str);
    }

    for (i = IC+1; i <= DC+IC+1; i++) {
        octal_str = print_only_5_octal(data_image[i-IC-1]);
        fprintf(ob_file, "%04d %s\n", i+100, octal_str);
        free(octal_str);
    }

    fclose(am_file);
    fclose(ob_file);

    return 1;
}

void activate_entry(hash_table symbols, char *arg, int *error) {
    struct symbol_value *value;
    value = search(symbols, arg);
    if (value == NULL) {
        *error = 19;
        return;
    }
    if (value->type[2] == 1) { /* extern flag already on this label */
        *error = 20;
        return;
    }
    value->type[3] = 1;
}

void complete_opcode(const char *arg, int *error, hash_table symbols, int *ext_flag, const char *ext_name) {
    FILE *ext_file;
    char copy[MAX_LINE], *token = NULL, curr_arg[MAX_LINE] = {0};
    int addressing_method, reg_flag = 0;
    struct symbol_value *value;
    strcpy(copy, arg);

    token = strtok(copy, COMMA_DELIM);
    IC++; /* skipping the opcode word index */
    while (token != NULL) {
        addressing_method = check_operand(token);

        sscanf(token, "%s", curr_arg);

        if (empty_line(curr_arg)) { /* stop or rts line*/
            break;
        }

        IC++; /* for each token */

        if (reg_flag && addressing_method >= 2) {
            IC--;
        }

        if (addressing_method == 1) { /* token is label argument that need to be coded in code_image[IC] */
            value = search(symbols, curr_arg);
            if (value == NULL) {
                *error = 19;
                return;
            }
            code_image[IC] = value->count << 3; /* saving space for ARE */
            if (value->type[2] == 1) {
                set_bit_to_one(&code_image[IC], 0); /* turning on external flag 'E' */

                if (*ext_flag) ext_file = fopen_with_ending(ext_name, ".ext", "a");
                else ext_file = fopen_with_ending(ext_name, ".ext", "w");
                fprintf(ext_file, "%s %04d\n", curr_arg, IC+100);
                fclose(ext_file);
                *ext_flag = 1;


            } else {
                set_bit_to_one(&code_image[IC], 1); /* turning on relocatable flag 'R' */
            }

        }

        if (addressing_method >= 2) {
            reg_flag = 1;
        }

        token = strtok(NULL, COMMA_DELIM);
    }
}

char *print_only_5_octal(short num) {
    /* Define the buffer size to accommodate 5 octal digits and a null terminator */
    int buffer_size = 6;
    char *buffer = safe_malloc(buffer_size * sizeof(char));
    int i;
    unsigned short unum;

    /* Null-terminate the string */
    buffer[buffer_size - 1] = '\0';

    /* Convert negative number to unsigned for correct octal representation */
    unum = (unsigned short)num;

    /* Start filling buffer from the second-to-last position */
    i = buffer_size - 2;

    /* Convert the number to octal and store it in the buffer */
    do {
        buffer[i--] = "01234567"[unum % 8];
        unum /= 8;
    } while (unum > 0 && i >= 0); /* Ensure no more than 5 digits */

    /* Ensure leading zeros if fewer than 5 digits are present */
    while (i >= 0) {
        buffer[i--] = '0';
    }

    /* Return pointer to the start of the octal string */
    return &buffer[i + 1];
}
