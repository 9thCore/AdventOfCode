#include <stdio.h>
#include <stdlib.h>

#define ARBITRARY_INPUT_MAX_RANGES 200
#define ARBITRARY_INPUT_MAX_INGREDIENTS 1000

typedef unsigned long long ingredient_t;
#define INGREDIENT_FORMAT "%llu"

// changed to one input with everything, from array of inputs
typedef struct {
    int range_count;
    struct {
        ingredient_t left, right;
    } ranges[ARBITRARY_INPUT_MAX_RANGES];

    int ingredient_count;
    ingredient_t ingredients[ARBITRARY_INPUT_MAX_INGREDIENTS];
} program_input;

void part1(program_input);
void part2(program_input);

int main(int argc, char **argv)
{
    program_input input = {.range_count = 0, .ingredient_count = 0};

    ingredient_t left, right;

    while (scanf(INGREDIENT_FORMAT "-" INGREDIENT_FORMAT, &left, &right) != 1) {
        input.ranges[input.range_count].left = left;
        input.ranges[input.range_count].right = right;
        input.range_count++;
    }
    
    input.ingredients[input.ingredient_count++] = left; // We read the first ingredient here, accidentally
    while (scanf(INGREDIENT_FORMAT, input.ingredients + input.ingredient_count) != EOF) {
        input.ingredient_count++;
    }

    part1(input);
    part2(input);

    return EXIT_SUCCESS;
}

void part1(program_input input) {
    int answer = 0;

    for (int i = 0; i < input.ingredient_count; i++) {
        ingredient_t ingredient = input.ingredients[i];
        for (int j = 0; j < input.range_count; j++) {
            if (input.ranges[j].left <= ingredient
                && ingredient <= input.ranges[j].right) {
                answer++;
                break;
            }
        }
    }

    printf("The answer to part 1 is: %d\n", answer);
}

void part2(program_input input) {
    int answer = 0;

    printf("The answer to part 2 is: %d\n", answer);
}