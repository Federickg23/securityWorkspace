#!/bin/bash

sudo rm -rf "$1"
	mkdir "$1"

cd "$1" 

mkdir bin mail inputs tmp lib
cp ../mail-in ../mail-out ./bin
sudo groupadd mail
sudo chgrp mail ./bin/mail-in
sudo chgrp mail ./bin/mail-out
sudo chown mail ./bin/mail-in
sudo chown mail ./bin/mail-out
sudo chmod 010 ./bin/mail-out
sudo usermod -G mail mail



# sudo chmod 111 ./bin


# NEXT STEPS
# Add group for mail-out, make it so that mail-out can read and write to all of the directories? 
# change group of mail-in and mail-out, make it so that only mail-in can call mail-out? 
# change permissions of the bin so that after mail-in and mail-out are added, the files cannot be changed. 

cd mail 
input=("addleness" "analects" "annalistic" "anthropomorphologically" "blepharosphincterectomy" "corector" "durwaun" "dysphasia" "encampment" "endoscopic" "exilic" "forfend" "gorbellied" "gushiness" "muermo" "neckar" "outmate" "outroll" "overrich" "philosophicotheological" "pockwood" "polypose" "refluxed" "reinsure" "repine" "scerne" "starshine" "unauthoritativeness" "unminced" "unrosed" "untranquil" "urushinic" "vegetocarbonaceous" "wamara" "whaledom" "Federickg23")

for i in ${input[@]}
do
	mkdir "$i" 
	sudo chmod 000 "$i"
	sudo chown "$i" "$i"
	sudo chmod u+rwx "$i"
done
cd ../inputs
