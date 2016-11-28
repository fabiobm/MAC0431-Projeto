CC = gcc
CFLAGS = -lm -Wall -O3

paralelo: paralelo.c
	$(CC) -fopenmp paralelo.c -o paralelo $(CFLAGS)

sequencial:
	$(CC) sequencial.c -o sequencial $(CFLAGS)

clean:
	@rm -f paralelo sequencial