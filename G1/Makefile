CC=gcc
FLAGS=--std=c99 -O2 -g

.DEFAULT_GOAL := build

.PHONY: build run scrabble anagram clean

build: g1

g1: g1.c list.c list.h hash.c hash.h
	  ${CC} ${FLAGS} g1.c list.c hash.c -o g1

clean:
	  rm ./g1
	  rm ./*.gch

run: g1
	  ./g1 ${WORD} ${LETTER} ${PLACE}

scrabble: g1
	  ./g1 ${WORD} ${LETTER} ${PLACE}

anagram: g1
	  ./g1 ${WORD}
