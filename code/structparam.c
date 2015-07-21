#include <stdio.h>
#include <stdlib.h>

struct course_grade {
    char course_name[32];
    char letter_grade;
};

struct student {
    char name[32];
    short class_year;
    int num_courses_completed;
    struct course_grade courses_completed[48];
};

typedef struct student student_t;
typedef struct course_grade grade_t;

double compute_gpa(grade_t course_list[], int num_courses) {
    double sum = 0.0;
    for (int i = 0; i < num_courses; i++) {
        if (course_list[i].letter_grade == 'A') {
            sum += 4.0;
        } else if (course_list[i].letter_grade == 'B') {
            sum += 3.0;
        } else if (course_list[i].letter_grade == 'C') {
            sum += 2.0;
        } else if (course_list[i].letter_grade == 'D') {
            sum += 1.0;
        }
    }
    return sum / num_courses;
}

void print_student(student_t s, double gpa) {
    printf("%s, Class of %d, GPA: %2.2f\n", s.name, s.class_year, gpa);
}

int main() {
    student_t students[2] = { 
        { "A. Student", 2019, 3, { {"Flowerpot construction", 'A'},
                                   {"Underwater basketweaving", 'C'},
                                   {"Dog grooming", 'B'}  },
        },
        { "B. Smart", 2018, 4, { {"Flowerpot construction", 'B'},
                                 {"Underwater basketweaving", 'C'},
                                 {"Cat dentistry", 'C'},
                                 {"Dog grooming", 'C'},  }
        }
    };
    int num_students = sizeof(students) / sizeof(student_t);

    for (int i = 0; i < num_students; i++) {
        double gpa = compute_gpa(students[i].courses_completed, students[i].num_courses_completed);
        print_student(students[i], gpa);
    }
    return 0;
}
