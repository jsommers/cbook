#include <stdio.h>
#include <stdbool.h>

struct fraction {
    int numerator;
    int denominator;
};

int main() {
    struct fraction f1;
    struct fraction f2 = { 1, 2 };
    struct fraction f3 = { .denominator=5, .numerator=3 };
    struct fraction f4 = { .denominator=5 };
    f1 = f2; //  copies the whole struct
    printf("f1: %d/%d\n", f1.numerator, f1.denominator);
    printf("f2: %d/%d\n", f2.numerator, f2.denominator);
    printf("f3: %d/%d\n", f3.numerator, f3.denominator);
    printf("f4: %d/%d\n", f4.numerator, f4.denominator);

    struct fraction f6 = { 1, 2};
    printf("Size of f6: %lu\n", sizeof(f6));
    printf("Size of struct fraction: %lu\n", sizeof(struct fraction));

    return 0;
}
