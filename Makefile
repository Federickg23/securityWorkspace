
CXXFLAGS = -Wall -g

.PHONY: cstore
cstore: main.o sha256.o
	g++ $(CXXFLAGS) -o cstore main.o sha256.o

main.o: main.cpp sha256.o
	g++ $(CXXFLAGS) -c main.cpp

sha256.o: sha256.c sha256.h
	gcc $(CXXFLAGS) -c  -o sha256.o sha256.c

.PHONY: clean
clean:
	rm -f main.o .main.cpp.swp sha256.o

.PHONY: all
all: clean cstore
