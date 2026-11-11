#include "student.h"
#include <ctype.h>
#include <math.h>
#include <string.h>

static int valid(const Student *s)
{
    if (!s || s->id <= 0 || s->age < 1 || s->age > 150 ||
        !isfinite(s->grade) || s->grade < 0 || s->grade > 100 ||
        !memchr(s->name, '\0', NAME_LENGTH)) return 0;
    for (const char *p = s->name; *p; ++p)
        if (!isspace((unsigned char)*p)) return 1;
    return 0;
}

const Student *student_find(const StudentSystem *system, int id)
{
    if (!system) return NULL;
    for (size_t i = 0; i < system->count; ++i)
        if (system->students[i].id == id) return &system->students[i];
    return NULL;
}

Status student_add(StudentSystem *system, const Student *student)
{
    if (!system || !valid(student)) return SMS_INVALID;
    if (student_find(system, student->id)) return SMS_DUPLICATE;
    if (system->count >= MAX_STUDENTS) return SMS_FULL;
    system->students[system->count++] = *student;
    return SMS_OK;
}

Status student_update(StudentSystem *system, const Student *student)
{
    if (!system || !valid(student)) return SMS_INVALID;
    for (size_t i = 0; i < system->count; ++i) {
        if (system->students[i].id == student->id) {
            system->students[i] = *student;
            return SMS_OK;
        }
    }
    return SMS_NOT_FOUND;
}

Status student_delete(StudentSystem *system, int id)
{
    if (!system || id <= 0) return SMS_INVALID;
    for (size_t i = 0; i < system->count; ++i) {
        if (system->students[i].id == id) {
            memmove(&system->students[i], &system->students[i + 1],
                    (system->count - i - 1) * sizeof(Student));
            --system->count;
            return SMS_OK;
        }
    }
    return SMS_NOT_FOUND;
}

const char *status_message(Status status)
{
    switch (status) {
    case SMS_OK: return "Success.";
    case SMS_INVALID: return "Invalid student data.";
    case SMS_DUPLICATE: return "A student with that ID already exists.";
    case SMS_FULL: return "Student capacity reached (100).";
    case SMS_NOT_FOUND: return "Student not found.";
    }
    return "Unknown status.";
}
