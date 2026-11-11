# Student Management System in C

A menu-driven learning project demonstrating **structures, arrays, pointers,
and functions** with no third-party libraries.

## Features

- Add up to 100 students, each with a unique positive integer ID.
- List all students or find a student by ID.
- Update a student's name, age, and grade while keeping the ID unchanged.
- Delete a student and compact the array.
- Reject duplicate IDs, blank or oversized names, invalid numbers, and out-of-range values.
- Handle end-of-input cleanly, including during an unfinished add or update.

Records are stored in memory for the current session only. Exiting discards them.
This educational application has no database, authentication, or persistence.

## Build and run

Requirements: a C11 compiler (GCC or Clang), Make, and Python 3 for integration tests.
On Linux or macOS:

```sh
make
./build/student_manager
```

Or compile directly:

```sh
cc -std=c11 -Wall -Wextra -Wpedantic -Iinclude src/main.c src/student.c -lm -o student_manager
./student_manager
```

Choose an action from the menu:

```text
1. Add student
2. List students
3. Find student
4. Update student
5. Delete student
0. Exit
```

For example, choose `1`, enter ID `101`, name `Ada Lovelace`, age `20`,
and grade `95.5`. Choose `2` to see the record, or `3` and `101` to find it.
Use `4` and `101` to replace its details, and `5` and `101` to delete it.

Names may contain spaces and occupy up to 79 bytes (excluding the terminating
null byte). Ages must be whole numbers from 1 to 150. Grades must be finite
numbers from 0 to 100. IDs range from 1 to the platform's `INT_MAX`.
Numbers with fractional values are rejected for integer fields. Oversized input
lines are discarded and the prompt repeats.

## Project layout

```text
include/student.h       Data types and public function declarations
src/student.c           Validation and student operations
src/main.c              Menu, input handling, and output
tests/test_student.c     API unit tests
tests/test_cli.py        Executable integration tests
Makefile                Build, test, and sanitizer targets
.github/workflows/ci.yml GitHub Actions test workflow
```

## C concepts demonstrated

- **Structures:** `Student` groups a student's fields; `StudentSystem` holds
  the array and its active record count. Initialize it with `{0}`.
- **Arrays:** `students[MAX_STUDENTS]` stores records; `name[NAME_LENGTH]`
  stores each null-terminated name. Deletion shifts later records left.
- **Pointers:** functions accept system and student pointers; `student_find`
  returns a read-only pointer to a record, or `NULL` when absent. Input helpers
  write through output pointers. A returned record pointer belongs to the
  system and must be looked up again after a mutation.
- **Functions:** storage operations are separate from the interface so they
  can be tested without simulating keyboard input. `Status` distinguishes
  invalid data, duplicate IDs, full capacity, and missing records.

The public API expects a zero-initialized system whose array and count are
managed through the provided functions. Search, update, and delete take O(n)
time; storage capacity is fixed at 100 students.

## Tests

```sh
make test       # Strict warning build, API tests, CLI integration tests
make sanitize   # Repeat tests with AddressSanitizer and UndefinedBehaviorSanitizer
make clean      # Remove generated build files
```

API tests cover CRUD operations, invalid data, duplicate IDs, full capacity,
array compaction, emptying and reusing the system, and null arguments. CLI tests
cover the full workflow, invalid and oversized input, maximum-length names,
non-finite grades, and end-of-input. The sanitizer target requires GCC or Clang
with sanitizer support and uses non-PIE binaries for reliable execution in
Linux sandbox environments. GitHub Actions runs both test targets on pushes
and pull requests.

If your sandbox runs processes under ptrace, LeakSanitizer cannot start. Run
`ASAN_OPTIONS=detect_leaks=0 make sanitize` in that environment; address and
undefined-behavior checks remain enabled. The application uses no heap allocation.
