#include <iostream> 
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <memory.h>
#include <string.h>
#include <sys/stat.h>
extern "C" {
    #include "sha256.h"
    #include "aes.h"
}

void getKey(char* pwd, BYTE *buf){
    SHA256_CTX ctx;
    int idx;
    
    BYTE *pwdArr = reinterpret_cast<unsigned char*>(pwd);
    
    sha256_init(&ctx);
    sha256_update(&ctx, pwdArr, strlen(pwd));
    sha256_final(&ctx, buf);

    for ( idx = 0; idx < 100000; ++idx){ 
        sha256_init(&ctx);
        sha256_update(&ctx, buf, SHA256_BLOCK_SIZE);
        sha256_final(&ctx, buf); 
    }
	sha256_final(&ctx, buf);

}

void manage_archive(BYTE *buf, char *archive_name, char **file_names, int files, char* mode){

    //returns -1 if archive is already created
    mkdir(archive_name, 0);
    
    if(strcmp(mode, "add") == 0){
        std::fstream FileNames;
        FileNames.open("filenames.txt");
        for(int i = 0; i < files; i++){
            std::fstream MyFile;
            MyFile.open(strcat(archive_name, strcat((char *)"/", file_names[i])));
            MyFile << "test";
            FileNames << file_names[i] << std::endl; 
            MyFile.close();
        }
        FileNames.close();
    }



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
        //std::cout << "Entered list archive" << std::endl;
    }
    else if (strcmp(argv[1], "add") == 0 || strcmp(argv[1], "extract") == 0 || strcmp(argv[1], "delete") == 0){

        std::cout << "Entered " << argv[1] << std::endl;
        bool password_found = false; 
        char *pwd; 
        char *archive_name; 
        char **file_names; 
        int files; 
        //Look for -p and if there is a -p, the next term is password
        //if not, prompt the user for the password 
        //for( int i = 0; i < argc-1; i++){
        if(strcmp(argv[2], "-p") == 0){

            password_found = true; 
            pwd = argv[3];
        //        break; 
        }
        //}

        if (!password_found){
            pwd = getpass("Password:");
            archive_name = argv[2]; 
            file_names = new char*[argc-3]; 
            files = argc-3;
            for(int i = 3; i < argc; i++){
                file_names[i] = argv[i]; 
                std::cout << file_names[i] << std::endl;
            }
        } else {
            archive_name = argv[4]; 
            file_names = new char*[argc-5];
            files = argc-5;
            for(int i = 4; i < argc; i++){
                file_names[i] = argv[i]; 
                std::cout << file_names[i] << std::endl; 
            }
        }

        std::cout << archive_name << std::endl; 
       

        
        //std::cout << pwd << std::endl;
        //sha256 on password 10000 times. 
        BYTE buf[SHA256_BLOCK_SIZE];
        memset(buf, 0, sizeof(buf));
        getKey(pwd, buf);
    
        for (int i = 0; i < SHA256_BLOCK_SIZE; i++){
            std::cout << buf[i] ;

        }
        std::cout << std::endl;
    
        manage_archive(buf, archive_name, file_names,files, argv[1]);

    }
    else {
        std::cout << "You have inputted an incorrect command. The options are list, add, extract, and delete." << std::endl; 
    }

}  



