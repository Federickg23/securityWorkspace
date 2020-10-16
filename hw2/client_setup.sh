#!/bin/bash

cd $HOME/ca

mkdir $HOME/ca/client

cd $HOME/ca/client

mkdir certs crl newcerts private csr 
chmod 700 private
touch index.txt
echo 1000 > serial

cd $HOME/ca

openssl genrsa -aes256 \
      -out client/private/web_client.key.pem 2048
chmod 400 client/private/web_client.key.pem

openssl req -config intermediate/openssl.cnf \
      -key client/private/web_client.key.pem \
      -new -sha256 -out client/csr/web_client.csr.pem

openssl ca -config $HOME/ca/intermediate/openssl.cnf \
      -extensions usr_cert -days 375 -notext -md sha256 \
      -in client/csr/web_client.csr.pem \
      -out client/certs/web_client.cert.pem

chmod 444 client/certs/web_client.cert.pem

openssl x509 -noout -text \
      -in client/certs/web_client.cert.pem

openssl verify -CAfile intermediate/certs/ca-chain.cert.pem \
      client/certs/web_client.cert.pem

openssl s_client -cert client/certs/web_client.cert.pem \
      -key client/private/web_client.key.pem \
      -verify_return_error \
      -showcerts -CAfile $HOME/ca/intermediate/certs/ca-chain.cert.pem \