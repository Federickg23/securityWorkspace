#!/bin/bash

cd $HOME/ca/server

openssl s_server -cert $HOME/ca/server/certs/web_server.cert.pem -accept 4433 \
      -key $HOME/ca/server/private/web_server.key.pem \
      -WWW -verify_return_error -Verify 1\
      -CAfile $HOME/ca/intermediate/certs/ca-chain.cert.pem \
      -ign_eof  
