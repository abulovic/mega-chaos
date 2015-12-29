CC=g++
CFLAGS=-I.

all: main.o wav_decode
	$(CC) -c main.cpp
	$(CC) main.o -o viewer -lsfml-graphics -lsfml-window -lsfml-system
	$(CC) -c flicker_test.cpp
	$(CC) flicker_test.o -o ftest-app -lsfml-graphics -lsfml-window -lsfml-system

wav_decode: wav_decode.cpp
	$(CC) -o wav_decode -lsndfile wav_decode.cpp

