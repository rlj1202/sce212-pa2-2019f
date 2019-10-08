TARGET	= pa2
CFLAGS	= -g

all: pa2

pa2: pa2.c
	gcc $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -rf pa2 *.o pa2.dSYM
