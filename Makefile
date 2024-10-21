build:
	gcc -Wall -g -std=c99 -o quadtree main.c quadtreeFunctions.c
run:
	./quadtree
clean:
	rm quadtree
valgrind:
	valgrind --leak-check=yes --track-origins=yes ./quadtree