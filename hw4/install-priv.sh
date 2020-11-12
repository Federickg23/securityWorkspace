#!/bin/bash

cd "$1" 

chgrp mail ./bin/mail-in
chgrp mail ./bin/mail-out
chown mail ./bin/mail-in
chown mail ./bin/mail-out
chmod u+s ./bin/mail-in
chmod 000 ./bin/mail-out
chmod u+rx ./bin/mail-out
chmod g+s ./bin/mail-out
chmod g+x ./bin/mail-out

chmod 555 ./bin
chmod 555 ./mail


cd mail 
input=("addleness" "analects" "annalistic" "anthropomorphologically" "blepharosphincterectomy" "corector" "durwaun" "dysphasia" "encampment" "endoscopic" "exilic" "forfend" "gorbellied" "gushiness" "muermo" "neckar" "outmate" "outroll" "overrich" "philosophicotheological" "pockwood" "polypose" "refluxed" "reinsure" "repine" "scerne" "starshine" "unauthoritativeness" "unminced" "unrosed" "untranquil" "urushinic" "vegetocarbonaceous" "wamara" "whaledom")

for i in ${input[@]}
do
	chmod 000 "$i"
	chown "$i" "$i"
    chgrp mail "$i"
	chmod u+rwx "$i"
    chmod g+rwx "$i"

done