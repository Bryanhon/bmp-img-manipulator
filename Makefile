OBJECTS          = main.o helper.o
CC               = gcc

all: $(OBJECTS)
	$(CC) -Og -g -I . -L . -lm $? -o a.out

main.o: main.c helper.h
	$(CC) -c -Og -g main.c

helper.o: helper.c
	$(CC) -c -Og -g helper.c

.PHONY: clean
clean:
	rm *.o
	rm a.out
