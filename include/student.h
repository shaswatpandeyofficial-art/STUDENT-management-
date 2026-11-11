#ifndef STUDENT_H
#define STUDENT_H
#include <stddef.h>
#define MAX_STUDENTS 100
#define NAME_LENGTH 80

typedef struct {
    int id;
    char name[NAME_LENGTH];
    int age;
    double grade;
} Student;

typedef struct {
    Student students[MAX_STUDENTS];
    size_t count;
} StudentSystem;

typedef enum { SMS_OK, SMS_INVALID, SMS_DUPLICATE, SMS_FULL, SMS_NOT_FOUND } Status;

/* Returned pointers belong to the system; mutations can invalidate them. */
const Student *student_find(const StudentSystem *system, int id);
Status student_add(StudentSystem *system, const Student *student);
Status student_update(StudentSystem *system, const Student *student);
Status student_delete(StudentSystem *system, int id);
const char *status_message(Status status);
#endif
