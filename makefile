LIBS=-lpthread

obj/*.o: src/*.c
	gcc -c -o $@ $< $(LIBS)

bin/pacman: obj/*.o
	ld -o $@ $< $(LIBS)

clean:
	rm obj/*.o
