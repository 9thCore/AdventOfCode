#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define ARBITRARY_INPUT_MAX_LENGTH 250
#define ARBITRARY_LINE_MAX_LENGTH 150

typedef struct {
    int line_length;
    char line[ARBITRARY_LINE_MAX_LENGTH];
} program_input;

void part1(program_input *, size_t);
void part2(program_input *, size_t);

int main(int argc, char **argv)
{
    program_input input[ARBITRARY_INPUT_MAX_LENGTH];
    size_t input_length = 0;

    while (scanf("%s\n", input[input_length].line) != EOF) {
        input[input_length].line_length = strlen(input[input_length].line);
        input_length++;
    }

    part1(input, input_length);
    part2(input, input_length);

    return EXIT_SUCCESS;
}

int has_roll_of_paper(program_input *input, size_t input_length, int i, int j) {
    if (i < 0 || j < 0 || i >= input_length || j >= input[0].line_length) {
        return 0;
    }

    return input[i].line[j] == '@';
}

int adjacent_rolls_of_paper(program_input *input, size_t input_length, int i, int j) {
    return has_roll_of_paper(input, input_length, i-1, j-1)
            + has_roll_of_paper(input, input_length, i, j-1)
            + has_roll_of_paper(input, input_length, i+1, j-1)
            + has_roll_of_paper(input, input_length, i-1, j)
            + has_roll_of_paper(input, input_length, i+1, j)
            + has_roll_of_paper(input, input_length, i-1, j+1)
            + has_roll_of_paper(input, input_length, i, j+1)
            + has_roll_of_paper(input, input_length, i+1, j+1);
}

void part1(program_input *input, size_t input_length) {
    int count = 0;

    for (int i = 0; i < input_length; i++) {
        for (int j = 0; input[i].line[j]; j++) {
            if (has_roll_of_paper(input, input_length, i, j)
            && adjacent_rolls_of_paper(input, input_length, i, j) < 4) {
                count++;
            }
        }
    }

    printf("The answer for part 1 is: %d\n", count);
}

void copy_program_input(program_input **dest, program_input *src, size_t input_length) {
    *dest = malloc(sizeof(program_input) * input_length);
    for (int i = 0; i < input_length; i++) {
        strcpy((*dest)[i].line, src[i].line);
        (*dest)[i].line_length = src[i].line_length;
    }
}

void part2(program_input *input, size_t input_length) {
    program_input *active_buffer, *inactive_buffer;
    copy_program_input(&active_buffer, input, input_length);
    copy_program_input(&inactive_buffer, input, input_length);

    int count = 0;
    int changed;

    do {
        changed = 0;

        for (int i = 0; i < input_length; i++) {
            for (int j = 0; input[i].line[j]; j++) {
                if (has_roll_of_paper(active_buffer, input_length, i, j)
                && adjacent_rolls_of_paper(active_buffer, input_length, i, j) < 4) {
                    inactive_buffer[i].line[j] = '.';
                    changed = 1;
                    count++;
                }
            }
        }

        for (int i = 0; i < input_length; i++) {
            strcpy(active_buffer[i].line, inactive_buffer[i].line);
        }

        program_input *temp = active_buffer;
        active_buffer = inactive_buffer;
        inactive_buffer = temp;
    } while (changed);

    printf("The answer for part 2 is: %d\n", count);
    
    free(active_buffer);
    free(inactive_buffer);
}