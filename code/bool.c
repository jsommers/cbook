#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int main() {
    bool done = false;
    char response[128];
    int sum = 0;
    int n = 0;
    printf ("Let me compute an average for you.  When you're done, enter -999\n");
    while (!done) {
        printf("Enter the %dth number: ", n+1);
        fflush(stdout);
        fgets(response, 128, stdin);
        int value = atoi(response);
        done = (value == -999);
        if (!done) {
            sum += value;
            n += 1;
        }
    }
    int average = 0;
    if (n > 0) {
        average = sum / n;
    }
    printf("The average is: %d\n", average);

    return 0;
}
