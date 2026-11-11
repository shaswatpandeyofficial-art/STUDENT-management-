CC = cc
CPPFLAGS = -Iinclude
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Werror -O2
LDLIBS = -lm
BUILD = build

.PHONY: all test sanitize clean
all: $(BUILD)/student_manager

$(BUILD):
	mkdir -p $@

$(BUILD)/student_manager: src/main.c src/student.c include/student.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) src/main.c src/student.c -o $@ $(LDFLAGS) $(LDLIBS)

$(BUILD)/test_student: tests/test_student.c src/student.c include/student.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) tests/test_student.c src/student.c -o $@ $(LDFLAGS) $(LDLIBS)

test: $(BUILD)/student_manager $(BUILD)/test_student
	./$(BUILD)/test_student
	python3 tests/test_cli.py ./$(BUILD)/student_manager

sanitize:
	$(MAKE) BUILD=build/sanitize CFLAGS='-std=c11 -Wall -Wextra -Wpedantic -Werror -g -O1 -fsanitize=address,undefined -fno-omit-frame-pointer -fno-pie' LDFLAGS='-fsanitize=address,undefined -no-pie' test

clean:
	rm -rf build
