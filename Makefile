CC=clang++
#CFLAGS=-std=c++98 -Wall -O2
CFLAGS = -std=c++17 -Wall -O2
CLIBS = -lm

all : main.cpp float24.h
		$(CC) $(CFLAGS) -o fp24 main.cpp

.PHONY: clean
clean :
		-rm fp24 
