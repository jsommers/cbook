#include <stdio.h>

int main() {
    printf("Enter your name: ");
    char name[16];
    fgets(name, 16, stdin);
    printf("Hello, %s!\n", name);
    return 0;
}
