# Chris Wilhelm cwilhel8
# Udochukwu Nwosu unwosu6
# Midterm Project - Solution Makefile

CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -g

## TODO: Write your own Makefile commands to link and compile your project as well as the cleanup

project: img_processing.o project.o ppm_io.o
	$(CC) -o project img_processing.o project.o ppm_io.o -lm

project.o: project.c img_processing.h ppm_io.h
	$(CC) $(CFLAGS) -c project.c

img_processing.o: img_processing.c img_processing.h ppm_io.h
	$(CC) $(CFLAGS) -c img_processing.c -lm

## Below are commands to link and compile the checkerboard program
# Links together files needed to create the checkerboard executable
checkerboard : checkerboard.o ppm_io.o
	$(CC) -static -o $@ checkerboard.o ppm_io.o

# Compile the ppm i/o source code
ppm_io.o: ppm_io.c ppm_io.h
	$(CC) $(CFLAGS) -c ppm_io.c

# Removes all object files and the executable named project, so we can start fresh
clean:
	rm -f *.o checkerboard project
