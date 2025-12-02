#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ARBITRARY_INPUT_MAX_LENGTH 100

typedef unsigned long long num_t;

typedef struct {
    num_t left, right;
} program_input;

void part1(program_input *, size_t);
void part2(program_input *, size_t);

int main(int argc, char **argv)
{
    program_input input[ARBITRARY_INPUT_MAX_LENGTH];
    size_t input_length;

    while (scanf("%llu-%llu,", &input[input_length].left, &input[input_length].right) != EOF) {
        input_length++;
    }

    part1(input, input_length);
    part2(input, input_length);

    return EXIT_SUCCESS;
}

void find_properties(num_t number, num_t *digit_count, num_t *next_power) {
    *digit_count = 0;
    *next_power = 1;

    while (number) {
        (*digit_count)++;
        (*next_power) *= 10;
        number /= 10;
    }
}

void iterate_trivial_case(num_t *trivial_case) {
    *trivial_case = *trivial_case * 10 + 1;
}

void part1(program_input *input, size_t input_length) {
    num_t sum = 0;
    num_t digit_count, next_power, copy_find_pattern;

    for (size_t i = 0; i < input_length; i++) {
        find_properties(input[i].left, &digit_count, &next_power);
        copy_find_pattern = sqrt(next_power) + 1;

        for (num_t j = input[i].left; j <= input[i].right; j++) {
            if (j == next_power) {
                next_power *= 10;
                copy_find_pattern = sqrt(next_power) + 1;
                digit_count++;
            }

            if (digit_count % 2 == 1) {
                j = next_power - 1;
                continue;
            }
            
            if (j % copy_find_pattern == 0) {
                sum += j;
            }
        }
    }

    printf("The answer for part 1 is: %llu\n", sum);
}

void part2(program_input *input, size_t input_length) {
    num_t sum = 0;
    num_t digit_count, next_power;
    num_t trivial_case;

    for (size_t i = 0; i < input_length; i++) {
        find_properties(input[i].left, &digit_count, &next_power);

        trivial_case = 0;
        for (num_t j = 0; j < digit_count; j++) {
            iterate_trivial_case(&trivial_case);
        }

        for (num_t j = input[i].left; j <= input[i].right; j++) {
            if (j < 10) {
                continue;
            }

            if (j == next_power) {
                next_power *= 10;
                iterate_trivial_case(&trivial_case);
                digit_count++;
            }
            
            if (j % trivial_case == 0) {
                sum += j;
                continue;
            }

            for (num_t k = 2; k <= digit_count / 2; k++) {
                if (digit_count % k != 0) {
                    continue;
                }

                num_t pattern = 1;
                num_t raise = pow(10, k);

                for (num_t step = 1; step < digit_count / k; step++) {
                    pattern = pattern * raise + 1;
                }

                if (j % pattern == 0) {
                    sum += j;
                    break;
                }
            }
        }
    }

    printf("The answer for part 2 is: %llu", sum);
}