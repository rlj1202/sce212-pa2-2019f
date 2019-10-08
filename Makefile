TARGET	= pa2
CFLAGS	= -g

all: pa2

pa2: pa2.c
	gcc $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -rf pa2 *.o pa2.dSYM

.PHONY: test-basic
test-basic: pa2 testcases/basic
	./pa2 < testcases/basic 2>&1 >/dev/null

.PHONY: test-load
test-load: pa2 testcases/load testcases/program-basic
	./pa2 < testcases/load 2>&1 >/dev/null

.PHONY: test-run-1
test-run-1: pa2 testcases/run-lv1 testcases/program-basic
	./pa2 < testcases/run-lv1 2>&1 >/dev/null

.PHONY: test-run-2
test-run-2: pa2 testcases/run-lv2 testcases/program-fibonacci
	./pa2 < testcases/run-lv2 2>&1 >/dev/null
