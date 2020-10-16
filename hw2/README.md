# Security Homework 2

### Federick Gonzalez (fag2113)

When running the bash scripts, if the permissions are somehow not properly set, please run `chmod +x` on all 5 of the scripts. 

Begin by running `assignment.sh`. Several prompts will appear to the screen, asking for a user generated password and verification, I thought this would be more secure than hardcoding a password to be used each time one wanted to start everything. The user will also be asked to supply information for the root certificate information, and then again for the intermediate certificate information. Make sure when filling out this informational step, the Country Name, Organization Name, and Common Name are the same. Once this process is complete, the terminal will print out instructions to follow.

The instructions are to run `bash server_setup.sh` and `bash client_setup.sh`. These will allow the user to specify information they would like included on the server's certificate and the client certificate. Once this is finished, run `bash server_start.sh` in this terminal window. Open a secondary terminal window and run `bash client_start.sh`. Currently the only file available is index.html, so a GET request will be created for that file. If all goes well, it should print out "The retrieval from the server is working properly". 

Finally, once that is finished, run `bash tests.sh. The tests are to check two different kinds of invalid certificates. First, an expired certificate is tested, and the console should print out that the certificate is indeed expired. The second is a certificate set in the future which is not yet ready. The console should print that it's invalid because it is not yet ready. Finally, there is an attempted server connection which should fail. 
