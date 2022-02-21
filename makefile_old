# List your *.h files here:
headers = matrix.h exceptions.h

# List your *.cpp files here:
sources = matrix.cpp testmatrix.cpp

program: $(headers) $(sources)
	g++ -g -Wall -pedantic $(sources) -o program

.PHONY: clean check debug

clean:
	-rm program

check: program
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./program

debug: program
	gdb program
