#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ARBITRARY_INPUT_MAX_LENGTH 250
#define ARBITRARY_BANK_MAX_LENGTH 150

typedef struct {
    char bank[ARBITRARY_BANK_MAX_LENGTH];
} program_input;

void part1(program_input *, size_t);
void part2(program_input *, size_t);

int main(int argc, char **argv)
{
    program_input input[ARBITRARY_INPUT_MAX_LENGTH];
    size_t input_length = 0;

    while (scanf("%s\n", input[input_length].bank) != EOF) {
        input_length++;
    }

    part1(input, input_length);
    part2(input, input_length);

    return EXIT_SUCCESS;
}

void part1(program_input *input, size_t input_length) {
    int sum = 0;

    for (size_t i = 0; i < input_length; i++) {
        char *bank = input[i].bank;
        char first = bank[0], second = bank[1];

        for (int j = 1; bank[j] != '\0'; j++) {
            if (bank[j] > first && bank[j + 1] != '\0') {
                first = bank[j];
                second = bank[j + 1];
            } else if (bank[j] > second) {
                second = bank[j];
            }
        }

        sum += (first - '0') * 10 + second - '0';
    }

    printf("The answer for part 1 is: %d\n", sum);
}

void part2(program_input *input, size_t input_length) {
    
}