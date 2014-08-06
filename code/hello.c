#include <stdio.h>
#include <string.h>

int main() {
    printf("Enter your name: ");
    char name[32];
    fgets(name, 32, stdin);
    name[strlen(name)-1] = '\0'; 
    printf("Hello, %s!\n", name);
    return 0;
}
