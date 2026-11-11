#include "student.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    StudentSystem system = {0};
    Student a = {1, "Ada Lovelace", 20, 95.5};
    assert(student_find(&system, 1) == NULL);
    assert(student_add(&system, &a) == SMS_OK);
    assert(student_add(&system, &a) == SMS_DUPLICATE);
    assert(system.count == 1);
    assert(strcmp(student_find(&system, 1)->name, "Ada Lovelace") == 0);
    a.grade = 100;
    assert(student_update(&system, &a) == SMS_OK);
    assert(student_find(&system, 1)->grade == 100);
    a.id = 2;
    assert(student_update(&system, &a) == SMS_NOT_FOUND);
    a.age = 0;
    assert(student_add(&system, &a) == SMS_INVALID);
    a.age = 20;
    a.grade = NAN;
    assert(student_add(&system, &a) == SMS_INVALID);
    a.grade = INFINITY;
    assert(student_add(&system, &a) == SMS_INVALID);
    a.grade = -1;
    assert(student_add(&system, &a) == SMS_INVALID);
    a.grade = 101;
    assert(student_add(&system, &a) == SMS_INVALID);
    a.grade = 0;
    memset(a.name, 'x', sizeof a.name);
    assert(student_add(&system, &a) == SMS_INVALID);
    strcpy(a.name, "   ");
    assert(student_add(&system, &a) == SMS_INVALID);
    strcpy(a.name, "Test Student");
    a.id = 0;
    assert(student_add(&system, &a) == SMS_INVALID);
    a.id = 2;
    assert(student_add(NULL, &a) == SMS_INVALID);
    assert(student_add(&system, NULL) == SMS_INVALID);
    assert(student_update(NULL, &a) == SMS_INVALID);
    assert(student_delete(NULL, 1) == SMS_INVALID);
    assert(student_find(NULL, 1) == NULL);
    for (int id = 2; id <= MAX_STUDENTS; ++id) {
        a.id = id;
        assert(student_add(&system, &a) == SMS_OK);
    }
    a.id = MAX_STUDENTS + 1;
    assert(student_add(&system, &a) == SMS_FULL);
    assert(system.count == MAX_STUDENTS);
    assert(student_delete(&system, 50) == SMS_OK);
    assert(system.count == MAX_STUDENTS - 1);
    assert(student_find(&system, 50) == NULL);
    assert(student_find(&system, 51)->id == 51);
    assert(student_delete(&system, 1) == SMS_OK);
    assert(system.students[0].id == 2);
    assert(student_delete(&system, MAX_STUDENTS) == SMS_OK);
    assert(student_delete(&system, MAX_STUDENTS) == SMS_NOT_FOUND);
    assert(student_delete(&system, 0) == SMS_INVALID);
    for (int id = 2; id < MAX_STUDENTS; ++id)
        if (id != 50) assert(student_delete(&system, id) == SMS_OK);
    assert(system.count == 0);
    a.id = 1;
    assert(student_add(&system, &a) == SMS_OK);
    puts("All student API tests passed.");
    return 0;
}
