#!/bin/bash

cd $HOME/ca 

mkdir tests

cd $HOME/ca/tests

mkdir certs crl newcerts private csr 
chmod 700 private
touch index.txt

echo 1000 > serial

echo "Testing expired certificate"

cd $HOME/ca 

openssl genrsa -aes256 \
      -out tests/private/tester.key.pem 2048

chmod 400 tests/private/tester.key.pem

openssl req -config intermediate/openssl.cnf \
      -key tests/private/tester.key.pem \
      -new -sha256 -out tests/csr/tester.csr.pem

openssl ca -config $HOME/ca/intermediate/openssl.cnf \
      -extensions usr_cert -days 375 -notext -md sha256 \
      -startdate 120815080000Z -enddate 120815090000Z  \
      -in tests/csr/tester.csr.pem \
      -out tests/certs/tester.cert.pem

chmod 444 tests/certs/tester.cert.pem

openssl x509 -noout -text \
      -in tests/certs/tester.cert.pem

openssl verify -CAfile intermediate/certs/ca-chain.cert.pem \
      tests/certs/tester.cert.pem      

echo "Testing certificate that is not ready yet"

cd $HOME/ca 

openssl genrsa -aes256 \
      -out tests/private/tester2.key.pem 2048

chmod 400 tests/private/tester2.key.pem

openssl req -config intermediate/openssl.cnf \
      -key tests/private/tester2.key.pem \
      -new -sha256 -out tests/csr/tester2.csr.pem

openssl ca -config $HOME/ca/intermediate/openssl.cnf \
      -extensions usr_cert -days 375 -notext -md sha256 \
      -startdate 220815080000Z -enddate 220815090000Z  \
      -in tests/csr/tester2.csr.pem \
      -out tests/certs/tester2.cert.pem

chmod 444 tests/certs/tester2.cert.pem

openssl x509 -noout -text \
      -in tests/certs/tester2.cert.pem

openssl verify -CAfile intermediate/certs/ca-chain.cert.pem \
      tests/certs/tester2.cert.pem     

cat tests/certs/tester.cert.pem \
      certs/ca.cert.pem > tests/certs/ca-chain.cert.pem
chmod 444 tests/certs/ca-chain.cert.pem

openssl s_client -cert $HOME/ca/tests/certs/tester.cert.pem \
      -key $HOME/ca/tests/private/tester.key.pem \
      -CAfile $HOME/ca/tests/certs/ca-chain.cert.pem \
      -verify_return_error -ign_eof \
      -showcerts 