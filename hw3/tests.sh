#!/bin/bash
cd "$1" 

echo "MAIL FROM:<reinsure>
RCPT TO:<scerne>
RCPT TO:<starshine>
DATA
Hello World!
..Hello World Again!
.
MAIL FROM:<overrich>
RCPT TO:<necker>
DATA
." >> inputs/00001

echo "One message should be printed to scerne and starshine, along with an invalid recipient error"

echo "MAIL FROM:<reinsure>
RCPT TO:<scerne>
RCPT TO:<starshine>
DATA
Hello World!
..Hello World Again!
MAIL FROM:<overrich>
RCPT TO:<necker>
DATA
." >> inputs/00002

echo "One message should be printed to scerne and starshine"


echo "MAIL FROM:<reinsure>
RCPT TO:<scee>
RCPT TO:<starshine>
DATA
Hello World!
." >> inputs/00003

echo "One message should be printed to starshine, with invalid rcpt"

echo "MAIL FROM:<reinsure>
RCPT TO:<scerne>
RCPT TO:<starshine>

Hello World!
." >> inputs/00004

echo "No messages should be printed, invalid header"

echo "RCPT TO:<scerne>
MAIL FROM:<reinsure>
RCPT TO:<starshine>
DATA
Hello World!
Hello World Again!
." >> inputs/00005
echo "No messages should be printed, invalid header"

echo "MAIL FROM:<reinsure>
RCPT TO:<starshine>

DATA
Hello World!
Hello World Again!
." >> inputs/00006
echo "No messages should be printed, invalid header"

echo "MAIL FROM:<unminced>
RCPT TO: <urushinic>
DATA

." >> inputs/00007
echo "Should print an empty message to urushinic"

echo "MAIL FROM:<reinsure>
RCPT TO:<starshine>
DATA
Hello World!" >> inputs/00008
echo "No messages should be printed, invalid message"


for i in inputs/*
do
    echo $i
	bin/mail-in <$i
done

