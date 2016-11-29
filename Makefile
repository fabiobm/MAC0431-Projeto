CC = gcc
CFLAGS = -lm -Wall -O3

paralelo: paralelo.c
	$(CC) -fopenmp paralelo.c -o paralelo $(CFLAGS)

clean:
	@rm -f paralelo
