#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    char buffer[64];
    printf ("Gimme a string: ");
    fgets(buffer, 64, stdin);
    for (int i = 0; i < strlen(buffer); i++) {
        if (isupper(buffer[i])) {
            buffer[i] = tolower(buffer[i]);
        }
    }
    printf ("Here's your string, lower-cased: %s\n", buffer);
    return 0;
}
