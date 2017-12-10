#include <stdio.h>
#include <stdlib.h>

int main() {
    float f = 3.14f;
    double d = 3.14;
    long double ld = 3.14l;
    printf("A float is %lu bytes\n", sizeof(f));
    printf("A double is %lu bytes\n", sizeof(d));
    printf("An long double is %lu bytes\n", sizeof(ld));
    return EXIT_SUCCESS;
}
