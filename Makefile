build: calc.c
	gcc -Wall calc.c -o calc -lm
clean:
	rm -r calc
