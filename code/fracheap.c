#include <stdio.h>
#include <stdlib.h> // for malloc and free

struct fraction {
    int numerator;
    int denominator;
};

void get_fractions(struct fraction *fracblock, int numfrac) {
    char buffer[32];
    for (int i = 0; i < numfrac; i++) {
        printf("Enter numerator for fraction %d: ", i+1);
        fgets(buffer, 32, stdin);
        int numerator = atoi(buffer);
        printf("Enter denominator for fraction %d: ", i+1);
        fgets(buffer, 32, stdin);
        int denominator = atoi(buffer);

        // use array syntax to fill in numer/denom for the ith fraction
        fracblock[i].numerator = numerator;
        fracblock[i].denominator = denominator;
    }
}

void invert_fractions(struct fraction *fracblock, int numfrac) {
    for (int i = 0; i < numfrac; i++) {
        int tmp = fracblock->numerator;
        fracblock->numerator = fracblock->denominator;
        fracblock->denominator = tmp;

        fracblock += 1; // pointer arithmetic:
                        // advance the pointer by 1 struct fraction
    }
}

void print_fractions(struct fraction *fracblock, int numfrac) {
    for (int i = 0; i < numfrac; i++) {
        // use pointer-arithmetic syntax to get numerator/denominator
        // for each fraction

        printf("%d: %d/%d\n", i+1, (fracblock+i)->numerator, 
                                   (fracblock+i)->denominator);
    }
}

int main() {
    char buffer[32];
    printf("How many fractions to make? ");
    fgets(buffer, 32, stdin);
    int numfrac = atoi(buffer);

    // allocate a block of numfrac fractions from the heap
    struct fraction *fractions = malloc(sizeof(struct fraction) * numfrac);

    // call function to "fill-in" each fraction
    get_fractions(fractions, numfrac);
    invert_fractions(fractions, numfrac);
    print_fractions(fractions, numfrac);

    free(fractions); // return block of fraction memory to the heap

    return 0;
}
