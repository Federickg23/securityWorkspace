CXXFLAGS = -Wall -g

.PHONY: install
install: mail-in mail-out
	bash install-unpriv.sh $(DEST)
	cp mail-in mail-out $(DEST)/bin
	sudo bash install-priv.sh $(DEST)
	make clean
	echo "Please do not forget to remove users and groups that have been created"

mail-in: mail-in.o mail_utils.o mail-out
	g++ -std=c++17 mail-in.o mail_utils.o -lstdc++fs -o mail-in

mail-out: mail-out.o mail_utils.o
	g++ -std=c++17 mail-out.o mail_utils.o -lstdc++fs -o mail-out

mail-in.o: mail-in.cpp
	g++ -std=c++17 -c mail-in.cpp

mail-out.o: mail-out.cpp
	g++ -std=c++17 -c mail-out.cpp

mail_utils.o: mail_utils.cpp
	g++ -std=c++17 -c mail_utils.cpp


.PHONY: delete
delete:
	sudo bash deleteUsers.sh 
	sudo rm -rf $(DEST)

.PHONY: clean
clean:
	rm -rf *.o *.swp mail-in mail-out