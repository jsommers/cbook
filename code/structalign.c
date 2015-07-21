#include <stdio.h>

struct student {
    char name[32];
    short class_year;
    char age;
};

int main() {
    struct student s = { "H. Sommers", 2026, 5 };
    printf("An example student: %s, %d, %d\n", s.name, s.class_year, s.age);
    printf("Size of a student struct: %lu\n", sizeof(struct student));
    return 0;
}
