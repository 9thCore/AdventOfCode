#include <stdio.h>
#include <stdlib.h>

#define ARBITRARY_INPUT_MAX_LENGTH 4200
#define FULL_ROTATION 100
#define RELEVANT_ROTATION 0

typedef struct {
    int current_rotation;
} program_state;

typedef struct {
    int change;
} program_input;

void part1(program_state, program_input *, int);
void part2(program_state, program_input *, int);

int main(int argc, char **argv)
{
    program_state initial_state = {FULL_ROTATION / 2};
    program_input input[ARBITRARY_INPUT_MAX_LENGTH];
    int input_length;

    char direction, discard;
    int change;

    while (scanf("%c%d", &direction, &change) != EOF) {
        input[input_length++].change = change * (direction == 'L' ? -1 : 1);

        // Discard trailing newline
        scanf("%c", &discard);
    }

    part1(initial_state, input, input_length);
    part2(initial_state, input, input_length);

    return EXIT_SUCCESS;
}

void part1(program_state state, program_input *input, int input_length) {
    int password = 0;

    for (int i = 0; i < input_length; i++) {
        state.current_rotation = (state.current_rotation + input[i].change) % FULL_ROTATION;
        password += !state.current_rotation;
    }

    printf("The answer to part 1 is: %d\n", password);
}

void part2(program_state state, program_input *input, int input_length) {
    int password = 0;

    for (int i = 0; i < input_length; i++) {
        int desired = state.current_rotation + input[i].change;
        int sign = state.current_rotation < desired ? 1 : -1;
        while (state.current_rotation != desired) {

            if (state.current_rotation % FULL_ROTATION == 0) {
                password++;
            }

            state.current_rotation = state.current_rotation + sign;
        }
    }

    printf("The answer to part 2 is: %d\n", password);
}