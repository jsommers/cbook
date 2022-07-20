#include <stdio.h>

#define SECOND 1
#define FIFTH 4

int main() {
    unsigned char flags = 0b00000000;
    flags = (1<<SECOND);          // light up the 2nd bit
    flags = flags | (1 << FIFTH); // and the 5th
    printf("0x%x\n", flags);
    return 0;
}
