#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARBITRARY_EQUATION_MAX_COUNT 1000
#define ARBITRARY_EQUATION_MAX_LENGTH 10
#define ARBITRARY_INPUT_LINE_LENGTH 3900
#define END_OF_EQUATION_MARKER 0

typedef unsigned long long number_t;
#define NUMBER_FORMAT "%llu"
#define NUMBER_CAST(string) atoll(string)

#define INPUT_TRANSFORMED_FILENAME "input-transformed.txt"

typedef struct {
    number_t numbers[ARBITRARY_EQUATION_MAX_LENGTH];
    int length;
    char operation;
} equation_t;

typedef struct {
    equation_t equations[ARBITRARY_EQUATION_MAX_COUNT];
    int equation_count;
} program_input;

void part2(program_input);

int main(int argc, char **argv)
{
    program_input input = {};

    char raw_input[ARBITRARY_EQUATION_MAX_LENGTH][ARBITRARY_INPUT_LINE_LENGTH] = {0};
    int line_index = 0;

    while (fgets(raw_input[line_index], ARBITRARY_INPUT_LINE_LENGTH, stdin) != NULL) {
        char *p = strchr(raw_input[line_index], '\n');
        if (p != NULL) {
            *p = '\0';
        }

        line_index++;
    }

    FILE *transformed_input = fopen(INPUT_TRANSFORMED_FILENAME, "w");
    if (transformed_input == NULL) {
        fprintf(stderr, "can't create transformed input");
        exit(EXIT_FAILURE);
    }

    for (int i = ARBITRARY_INPUT_LINE_LENGTH - 1; i >= 0; i--) {
        int filled_line = 0;
        char operation = '\0';

        for (int j = 0; j < line_index; j++) {
            char c = raw_input[j][i];
            if (c == '+' || c == '*') {
                operation = c;
            } else if (c >= '0' && c <= '9') {
                fprintf(transformed_input, "%c", c);
                filled_line = 1;
            }
        }
        
        if (filled_line) {
            fprintf(transformed_input, "\n");
            if (operation != '\0') {
                fprintf(transformed_input, "%d %c\n", END_OF_EQUATION_MARKER, operation);
                input.equation_count++;
            }
        }
    }

    fclose(transformed_input);

    transformed_input = fopen(INPUT_TRANSFORMED_FILENAME, "r");

    for (int equation_index = 0; equation_index < input.equation_count; equation_index++) {
        equation_t *equation = input.equations + equation_index;

        int number_index = 0;
        number_t number;

        while (fscanf(transformed_input, NUMBER_FORMAT " ", &number)
            && number != END_OF_EQUATION_MARKER) {
            equation->numbers[number_index] = number;
            number_index++;
        }

        fscanf(transformed_input, "%c", &equation->operation);
        equation->length = number_index;
    }

    fclose(transformed_input);

    part2(input);

    return EXIT_SUCCESS;
}

number_t compute(equation_t equation) {
    int add_or_multiply = equation.operation == '+';

    number_t result = (add_or_multiply ? 0 : 1);
    for (int i = 0; i < equation.length; i++) {
        if (add_or_multiply) {
            result += equation.numbers[i];
        } else {
            result *= equation.numbers[i];
        }
    }

    return result;
}

void part2(program_input input) {
    number_t answer = 0;

    for (int i = 0; i < input.equation_count; i++) {
        answer += compute(input.equations[i]);
    }

    printf("The answer to part 2 is: " NUMBER_FORMAT "\n", answer);
}