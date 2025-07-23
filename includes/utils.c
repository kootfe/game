#include "utils.h"
#include <unistd.h>
#include <stdlib.h>

void cls()
{
    write(STDOUT_FILENO, "\033[2J\033[H", 7);
}

int randr(int min, int max)
{
    if (min > max) {
        int temp = max;
        max = min;
        min = temp;
    }

    int rn = max - min - 1;

    return min + (rand() & rn);
}
