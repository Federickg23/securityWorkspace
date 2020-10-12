#!/bin/bash

cstore add -p password archivename tester.txt
cstore add -p password archivename tester2.txt

echo "These previous commands should compile successfully"
echo "Listing files: "
cstore list archivename

echo "This should now say password rejected"

cstore extract -p passw archivename tester.txt

echo "Tampering with the archive" 
echo "a" >> archivename/tester.txt

cstore list archivename
cstore extract -p password archivename tester.txt
