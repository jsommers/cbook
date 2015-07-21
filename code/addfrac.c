#include <stdio.h>
#include <stdlib.h>

typedef struct fraction {
    int numerator;
    int denominator;
} fraction_t;

// compute the greatest common divisor, recursive style
int gcd(int a, int b) {
    if (b == 0) {
        return a;
    } else {
        return gcd(b, a % b);
    }
}

// compute the least common multiple 
int lcm(int a, int b) {
    return abs(a * b) / gcd(a, b);
}

// add a couple fractions together
fraction_t add_fractions(fraction_t f1, fraction_t f2) {
    int denom = lcm(f1.denominator, f2.denominator);
    int numer = (denom / f1.denominator) * f1.numerator +
                (denom / f2.denominator) * f2.numerator;
    fraction_t result = { numer, denom };
    return result;
}

int main() {
    fraction_t f1 = { 1, 2};
    fraction_t f2 = { 3, 5};
    fraction_t f3 = add_fractions(f1, f2);
    printf("%d/%d + %d/%d = %d/%d\n", f1.numerator, f1.denominator,
                                      f2.numerator, f2.denominator,
                                      f3.numerator, f3.denominator);
    return 0;
}
