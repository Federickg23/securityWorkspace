CC  = g++
CXX = g++ -O0 # -fno-elide-constructors

CXXFLAGS = -Wall -g

.PHONY: cstore
cstore: main.o sha256.o
	$(CXX) $(CXXFLAGS) -o cstore main.o sha256.o

main.o: main.cpp sha256.h
	$(CXX) $(CXXFLAGS) -c main.cpp

sha256.o: sha256.c sha256.h
	$(CXX) $(CXXFLAGS) -c sha256.c

.PHONY: clean
clean:
	rm -f main.o .main.cpp.swp sha256.o

.PHONY: all
all: clean cstore
