#include <stdio.h>
#include <stdlib.h>

#define ARBITRARY_INPUT_MAX_RANGES 200
#define ARBITRARY_INPUT_MAX_INGREDIENTS 1000

typedef unsigned long long ingredient_t;
#define INGREDIENT_FORMAT "%llu"

typedef struct {
    ingredient_t left, right;
} range_t;

// changed to one input with everything, from array of inputs
typedef struct {
    int range_count;
    range_t ranges[ARBITRARY_INPUT_MAX_RANGES];

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

int inside_range(ingredient_t ingredient, range_t *range) {
    return range->left <= ingredient && ingredient <= range->right;
}

void part1(program_input input) {
    int answer = 0;

    for (int i = 0; i < input.ingredient_count; i++) {
        ingredient_t ingredient = input.ingredients[i];
        for (int j = 0; j < input.range_count; j++) {
            if (inside_range(ingredient, input.ranges + j)) {
                answer++;
                break;
            }
        }
    }

    printf("The answer to part 1 is: %d\n", answer);
}

int invalid_range(range_t *range) {
    return range->left == 0 && range->right == 0;
}

void invalidate_range(range_t *range) {
    range->left = 0;
    range->right = 0;
}

void part2(program_input input) {
    ingredient_t answer = 0;

    range_t *joined_ranges = malloc(sizeof(range_t) * input.range_count);
    int joined_range_count = 0;

    for (int i = 0; i < input.range_count; i++) {
        range_t *origin = input.ranges + i;

        if (invalid_range(origin)) {
            continue;
        }

        int changed;

        do {
            changed = 0;

            for (int j = i + 1; j < input.range_count; j++) {
                range_t *additive = input.ranges + j;
                if (invalid_range(additive)) {
                    continue;
                }

                int left_inside = inside_range(additive->left, origin);
                int right_inside = inside_range(additive->right, origin);

                if (left_inside && right_inside) {
                    invalidate_range(additive);
                } else if (left_inside) { // Implicintly, it can't have the right side inside of the origin range
                    origin->right = additive->right;
                    invalidate_range(additive);
                    changed = 1;
                } else if (right_inside) { // Implicintly, it can't have the left side inside of the origin range
                    origin->left = additive->left;
                    invalidate_range(additive);
                    changed = 1;
                } else {
                    if (inside_range(origin->left, additive)
                    && inside_range(origin->right, additive)) {
                        *origin = *additive;
                        invalidate_range(additive);
                        changed = 1;
                    }
                }
            }
        } while (changed);

        joined_ranges[joined_range_count++] = *origin;
        invalidate_range(origin);
    }

    for (int i = 0; i < joined_range_count; i++) {
        answer += joined_ranges[i].right - joined_ranges[i].left + 1;
    }

    free(joined_ranges);

    printf("The answer to part 2 is: " INGREDIENT_FORMAT "\n", answer);
}