#!/bin/bash

cd $HOME/ca

mkdir $HOME/ca/server

cd $HOME/ca/server

mkdir certs crl newcerts private csr 
chmod 700 private
touch index.txt
echo 1000 > serial

cd $HOME/ca

openssl genrsa -aes256 \
      -out server/private/web_server.key.pem 2048
chmod 400 server/private/web_server.key.pem

#Create certificate for server 
openssl req -config intermediate/openssl.cnf \
      -key server/private/web_server.key.pem \
      -new -sha256 -out server/csr/web_server.csr.pem



openssl ca -config $HOME/ca/intermediate/openssl.cnf \
      -extensions server_cert -days 375 -notext -md sha256 \
      -in server/csr/web_server.csr.pem \
      -out server/certs/web_server.cert.pem

chmod 444 server/certs/web_server.cert.pem

openssl x509 -noout -text \
      -in server/certs/web_server.cert.pem



openssl verify -CAfile intermediate/certs/ca-chain.cert.pem \
      server/certs/web_server.cert.pem

# openssl s_server -cert $HOME/ca/server/certs/web_server.cert.pem -accept 4433 \
#       -key $HOME/ca/server/private/web_server.key.pem \
#       -WWW -verify_return_error \
#       -CAfile $HOME/ca/intermediate/certs/ca-chain.cert.pem \
#       -ign_eof  