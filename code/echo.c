#include <stdio.h>
#include <stdlib.h>

int main() {
    int character;
    character = getchar();
    while (character != EOF) {
        putchar(character);
        character = getchar();
    }
    return EXIT_SUCCESS;
}
