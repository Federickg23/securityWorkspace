CXXFLAGS = -Wall -g

.PHONY: install
install: mail-in mail-out
	bash create-tree.sh $(DEST)
	cp mail-in mail-out $(DEST)/bin
	make clean

.PHONY: mail-in
mail-in: mail-in.o
	g++ $(CXXFLAGS) -o mail-in mail-in.o 

mail-in.o: mail-in.cpp 
	g++ $(CXXFLAGS) -c mail-in.cpp

.PHONY: mail-out
mail-out: mail-out.o
	g++ $(CXXFLAGS) -o mail-out mail-out.o 

mail-out.o: mail-out.cpp 
	g++ $(CXXFLAGS) -c mail-out.cpp

.PHONY: tests
test: 
	bash tests.sh $(DEST)

.PHONY: clean
clean:
	rm -rf *.o *.swp mail-in mail-out