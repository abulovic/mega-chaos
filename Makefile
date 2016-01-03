CC=g++ -std=c++11 -Wall -O3 -march=native
CFLAGS=-I.

all: wav_decode
	$(CC) -c main.cpp
	$(CC) main.o -o viewer -lsfml-graphics -lsfml-window -lsfml-system
	$(CC) -c flicker_test.cpp
	$(CC) flicker_test.o -o ftest-app -lsfml-graphics -lsfml-window -lsfml-system

wav_decode: wav_decode.cpp
	$(CC) -o wav_decode wav_decode.cpp -lsndfile

decimate: decimator.cpp
	$(CC) -o decimator decimator.cpp