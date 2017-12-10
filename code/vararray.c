#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("How many scores to input? ");
    char buffer[10];
    fgets(buffer, 10, stdin);
    int num_scores = atoi(buffer);
    int scores[num_scores];
    for (int i = 0; i < num_scores; i++) {
        printf("Enter score %d: ", i+1);
        fgets(buffer, 10, stdin);
        scores[i] = atoi(buffer);
    }

    // ... do something with the scores

    return EXIT_SUCCESS;
}
