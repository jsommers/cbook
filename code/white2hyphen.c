#include <stdio.h>
#include <string.h>

int main() {
    char buffer[64];
    printf ("Gimme a string: ");
    fgets(buffer, 64, stdin);

    for (int i = strlen(buffer)-1; i >= 0; i--) {
        if (buffer[i] == ' ') {
            buffer[i] = '_';
        }
    }
    printf ("I don't like spaces, so here: %s\n", buffer);    
    return 0;
}
