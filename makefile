CC := gcc
LIBS := -lpthread

bin/main: src/*.c
	$(CC) $^ -o $@ $(LIBS)

clean:
	rm bin/main

run:
	./bin/main
