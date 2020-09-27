#include <iostream> 
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <memory.h>
#include <string.h>
extern "C" {
    #include "sha256.h"
}

void getKey(char* pwd, BYTE *buf){
    SHA256_CTX ctx;
    int idx;
    
    BYTE *pwdArr = reinterpret_cast<unsigned char*>(pwd); 
    std::cout << pwdArr << std::endl;
    sha256_init(&ctx);
    for (idx = 0; idx < 100000; ++idx)
	   sha256_update(&ctx, pwdArr, sizeof(pwdArr)/sizeof(BYTE) );
	sha256_final(&ctx, buf);

}

int main(int argc, char *argv[]){
    if (argc < 3){
        std::cout << "You have inputted the incorrect number of arguments. Please use one of the following formats: " << std::endl;
        std::cout << "cstore list archivename" << std::endl;
        std::cout << "cstore add [-p password] archivename file" << std::endl;
        std::cout << "cstore extract [-p password] archivename file" << std::endl;
        std::cout << "cstore delete [-p password] archivename file" << std::endl; 
    }
    else if (argc == 3 && strcmp(argv[1], "list") == 0){
        //Do a thing here to list the archive
        std::cout << "Entered list archive" << std::endl;
    }
    else if (strcmp(argv[1], "add") == 0 || strcmp(argv[1], "extract") == 0 || strcmp(argv[1], "delete") == 0){

        std::cout << "Entered " << argv[1] << std::endl;
        bool password_found = false; 
        char *pwd; 
        //Look for -p and if there is a -p, the next term is password
        //if not, prompt the user for the password 
        for( int i = 0; i < argc-1; i++){
            if(strcmp(argv[i], "-p") == 0){

                password_found = true; 
                pwd = argv[i+1];
                break; 
                //password == argv +1
            }
        }

        if (!password_found){
            //getpass, figure out return type, and make the password that return type
            pwd = getpass("Password:");
        }
        
        std::cout << pwd << std::endl;
        //sha256 on password 10000 times. 
        BYTE buf[SHA256_BLOCK_SIZE];
        memset(buf, 0, sizeof(buf));
        getKey(pwd, buf);
        for(int i = 0; i < strlen(buf); i++){
            std::cout << buf[i]; 
        }
        std::cout << std::endl;
    }
    else {
        std::cout << "You have inputted an incorrect command. The options are list, add, extract, and delete." << std::endl; 
    }

}  


