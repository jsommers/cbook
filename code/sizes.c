#include <stdio.h>

int main() {
    char c = 'a';
    short s = 0xbeef;
    int i = 100000;
    long l = 100000000L;
    long long ll = 60000000000LL;
    printf("A char is %lu bytes\n", sizeof(c));
    printf("A short is %lu bytes\n", sizeof(s));
    printf("An int is %lu bytes\n", sizeof(i));
    printf("A long is %lu bytes\n", sizeof(l));
    printf("A long long is %lu bytes\n", sizeof(ll));
    return 0;
}
