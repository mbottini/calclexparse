CC=g++
CFLAGS=--std=c++11 -Wall


all: main

main: lexer.o parser.o main.cpp
	${CC} ${CFLAGS} lexer.o parser.o main.cpp -o lexparse

lexer.o: lexer.cpp tokens.h
	${CC} ${CFLAGS} -c lexer.cpp

parser.o: parser.cpp tokens.h expr.h
	${CC} ${CFLAGS} -c parser.cpp

clean:
	rm -f lexparse
	rm -f *.o
