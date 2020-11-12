CXXFLAGS = -Wall -g

.PHONY: install
install: mail-in mail-out
	bash install-unpriv.sh $(DEST)
	cp mail-in mail-out $(DEST)/bin
	sudo bash install-priv.sh $(DEST)
	make clean
	echo "Please do not forget to remove users and groups that have been created"

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


.PHONY: delete
delete:
	sudo bash deleteUsers.sh 
	sudo rm -rf $(DEST)

.PHONY: clean
clean:
	rm -rf *.o *.swp mail-in mail-out