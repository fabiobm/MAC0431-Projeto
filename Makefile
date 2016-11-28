CC = gcc
CFLAGS = -lm -Wall

paralelo: paralelo.c
	$(CC) -fopenmp paralelo.c -o paralelo $(CFLAGS)

sequencial:
	$(CC) sequencial.c -o sequencial $(CFLAGS)

clean:
	@rm -f paralelo sequencial