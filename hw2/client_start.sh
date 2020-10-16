#!/bin/bash

openssl s_client -cert $HOME/ca/client/certs/web_client.cert.pem \
      -key $HOME/ca/client/private/web_client.key.pem \
      -CAfile $HOME/ca/intermediate/certs/ca-chain.cert.pem \
      -verify_return_error -ign_eof \
      -showcerts 