#!/bin/bash
if [ -d "$1" ] 
then
    echo "Directory $1 exists. Please remove with the sudo command" 
    exit 1
else 
    mkdir "$1"
fi

cd "$1" 

mkdir bin mail tmp lib

cd mail 
input=("addleness" "analects" "annalistic" "anthropomorphologically" "blepharosphincterectomy" "corector" "durwaun" "dysphasia" "encampment" "endoscopic" "exilic" "forfend" "gorbellied" "gushiness" "muermo" "neckar" "outmate" "outroll" "overrich" "philosophicotheological" "pockwood" "polypose" "refluxed" "reinsure" "repine" "scerne" "starshine" "unauthoritativeness" "unminced" "unrosed" "untranquil" "urushinic" "vegetocarbonaceous" "wamara" "whaledom")

for i in ${input[@]}
do
	mkdir "$i" 
done
