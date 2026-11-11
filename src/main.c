#include "student.h"
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Consume oversized lines so their remainder cannot become the next answer. */
static int read_line(const char *prompt, char *buffer, size_t size)
{
    fputs(prompt, stdout);
    fflush(stdout);
    if (!fgets(buffer, (int)size, stdin)) return 0;
    char *newline = strchr(buffer, '\n');
    if (newline) { *newline = '\0'; return 1; }
    int ch = getchar();
    if (ch == '\n' || ch == EOF) return 1;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
    puts("Input too long. Please try again.");
    return -1;
}

static int read_number(const char *prompt, double min, double max,
                       int integer, double *value)
{
    char buffer[128], *end;
    for (;;) {
        int result = read_line(prompt, buffer, sizeof buffer);
        if (!result) return 0;
        if (result < 0) continue;
        errno = 0;
        double number = strtod(buffer, &end);
        int parsed = end != buffer;
        while (isspace((unsigned char)*end)) ++end;
        if (parsed && !*end && !errno && isfinite(number) &&
            number >= min && number <= max && (!integer || trunc(number) == number)) {
            *value = number;
            return 1;
        }
        printf("Enter %s between %.0f and %.0f.\n", integer ? "an integer" : "a number", min, max);
    }
}

static int read_student(Student *s)
{
    for (;;) {
        int result = read_line("Name: ", s->name, sizeof s->name);
        if (!result) return 0;
        if (result < 0) continue;
        const char *p = s->name;
        while (isspace((unsigned char)*p)) ++p;
        if (*p) break;
        puts("Name must not be blank.");
    }
    double value;
    if (!read_number("Age (1-150): ", 1, 150, 1, &value)) return 0;
    s->age = (int)value;
    if (!read_number("Grade (0-100): ", 0, 100, 0, &value)) return 0;
    s->grade = value;
    return 1;
}

static void print_student(const Student *s)
{
    printf("ID: %d | Name: %s | Age: %d | Grade: %.2f\n",
           s->id, s->name, s->age, s->grade);
}

int main(void)
{
    StudentSystem system = {0};
    for (;;) {
        puts("\nStudent Management System\n1. Add student\n2. List students\n3. Find student\n4. Update student\n5. Delete student\n0. Exit");
        double value;
        if (!read_number("Choice: ", 0, 5, 1, &value) || value == 0) break;
        int choice = (int)value;
        if (choice == 2) {
            if (!system.count) puts("No students registered.");
            for (size_t i = 0; i < system.count; ++i) print_student(&system.students[i]);
            continue;
        }
        if (!read_number("Student ID: ", 1, INT_MAX, 1, &value)) break;
        int id = (int)value;
        const Student *found = student_find(&system, id);
        if (choice == 3) {
            if (found) print_student(found);
            else puts(status_message(SMS_NOT_FOUND));
        } else if (choice == 5) {
            puts(status_message(student_delete(&system, id)));
        } else {
            if (choice == 1 && found) { puts(status_message(SMS_DUPLICATE)); continue; }
            if (choice == 4 && !found) { puts(status_message(SMS_NOT_FOUND)); continue; }
            if (choice == 1 && system.count == MAX_STUDENTS) { puts(status_message(SMS_FULL)); continue; }
            Student student = {.id = id};
            if (!read_student(&student)) break;
            puts(status_message(choice == 1 ? student_add(&system, &student) :
                                             student_update(&system, &student)));
        }
    }
    puts("Goodbye. Session data is discarded.");
    return 0;
}
