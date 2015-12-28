CC=g++
CFLAGS=-I.

all: main.o
	$(CC) -c main.cpp
	$(CC) main.o -o viewer -lsfml-graphics -lsfml-window -lsfml-system
	$(CC) -c flicker_test.cpp
	$(CC) flicker_test.o -o ftest-app -lsfml-graphics -lsfml-window -lsfml-system

