
CXXFLAGS = -Wall -g

.PHONY: cstore
cstore: main.o sha256.o aes.o
	g++ $(CXXFLAGS) -o cstore main.o sha256.o aes.o

main.o: main.cpp sha256.o aes.o
	g++ $(CXXFLAGS) -c main.cpp

sha256.o: sha256.c sha256.h
	gcc $(CXXFLAGS) -c  -o sha256.o sha256.c

aes.o: aes.c aes.h 
	gcc $(CXXFLAGS) -c -o aes.o aes.c

.PHONY: clean
clean:
	rm -f main.o .main.cpp.swp sha256.o aes.o vgcore.*

.PHONY: all
all: clean cstore
