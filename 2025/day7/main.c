#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARBITRARY_LINE_COUNT 150
#define ARBITRARY_LINE_LENGTH 150

typedef unsigned long long number_t;
#define NUMBER_FORMAT "%llu"

typedef struct {
    char manifold[ARBITRARY_LINE_COUNT][ARBITRARY_LINE_LENGTH];
    number_t width, height;
    number_t startx, starty;
} program_input;

void part1(program_input);
void part2(program_input);

int main(int argc, char **argv)
{
    program_input input = {.height = 0};

    while (scanf("%s", input.manifold[input.height]) != EOF) {
        char *p = strchr(input.manifold[input.height], 'S');
        if (p != NULL) {
            input.startx = p - input.manifold[input.height];
            input.starty = input.height;
        }

        input.height++;
    }

    input.width = strlen(input.manifold[0]);

    part1(input);
    part2(input);

    return EXIT_SUCCESS;
}

void part1(program_input input) {
    number_t answer = 0;

    int *tachyons = calloc(input.width, sizeof(int));

    tachyons[input.startx] = 1;
    for (int i = input.starty; i < input.height; i ++) {
        for (int j = 1; j < input.width - 1; j++) {
            if (tachyons[j] && input.manifold[i][j] == '^') {
                answer++;
                tachyons[j] = 0;
                tachyons[j - 1] = 1;
                tachyons[j + 1] = 1;
            }
        }
    }

    free(tachyons);

    printf("The answer to part 1 is: " NUMBER_FORMAT "\n", answer);
}

void part2(program_input input) {
    number_t answer = 0;

    number_t timeline_count[ARBITRARY_LINE_COUNT][ARBITRARY_LINE_LENGTH] = {0};

    int last_line_index = input.height - 2;
    char *last_line = input.manifold[last_line_index];
    number_t *last_timeline_line = timeline_count[last_line_index];
    for (int j = 0; j < input.width; j++) {
        if (last_line[j] == '^') {
            last_timeline_line[j] = 2;
        }
    }

    for (int i = input.height - 4; i >= 2; i -= 2) {
        for (int j = 1; j < input.width - 1; j++) {
            if (input.manifold[i][j] == '^') {
                number_t left = timeline_count[i + 2][j - 1];
                number_t right = timeline_count[i + 2][j + 1];
                timeline_count[i][j] = (left ? left : 1) + (right ? right : 1);
            } else {
                timeline_count[i][j] = timeline_count[i + 2][j];
            }
        }
    }

    for (int i = input.starty; i < input.height; i++) {
        if (timeline_count[i][input.startx] != 0) {
            answer = timeline_count[i][input.startx];
            break;
        }
    }

    printf("The answer to part 2 is: " NUMBER_FORMAT "\n", answer);
}