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

void xor_buf(const BYTE in[], BYTE out[], size_t len)
{
	size_t idx;

	for (idx = 0; idx < len; idx++)
		out[idx] ^= in[idx];
}

void hmac_archive(BYTE *buf, char *archive_name, BYTE *hmac_buf){
    char filenamePath[256]; 
    
    strcpy(filenamePath, archive_name); 
    strcat(filenamePath, "/");
    strcat(filenamePath, ".filenames"); 
    
    std::ifstream infile(filenamePath);
    std::string line;

    char *archive_text = new char[10000];
    int archive_len = 0; 
    BYTE *right = new BYTE[10000];
    BYTE *complete_mac = new BYTE[10000];
    while (std::getline(infile, line)){
        std::ifstream t;
        int length;
        t.open(line);   
        t.seekg(0, std::ios::end);    
        length = t.tellg();           
        t.seekg(0, std::ios::beg);    
        char *buffer = new char[length];
        t.read(buffer, length);       
        t.close(); 
        archive_len += length; 
        strcat(archive_text, buffer);
        delete[] buffer; 
    }
    
    BYTE opad[SHA256_BLOCK_SIZE] = {0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,
	                               0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c};
    
    BYTE ipad[SHA256_BLOCK_SIZE] = {0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,
	                               0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36};
    
    BYTE keyCopy[SHA256_BLOCK_SIZE]; 
    memcpy(keyCopy, buf, SHA256_BLOCK_SIZE); 

    xor_buf(keyCopy, ipad, SHA256_BLOCK_SIZE); 

    memcpy(right, keyCopy, SHA256_BLOCK_SIZE); 
    memcpy(&right[SHA256_BLOCK_SIZE],archive_text, archive_len);
    
    BYTE mac_buf[SHA256_BLOCK_SIZE];
	SHA256_CTX ctx;

    sha256_init(&ctx);
	sha256_update(&ctx, right, strlen((char *)right));
	sha256_final(&ctx, mac_buf);

    memcpy(keyCopy, buf, SHA256_BLOCK_SIZE); 
    
    xor_buf(keyCopy, opad, SHA256_BLOCK_SIZE);

    memcpy(complete_mac, keyCopy, SHA256_BLOCK_SIZE); 
    memcpy(&complete_mac[SHA256_BLOCK_SIZE], mac_buf, SHA256_BLOCK_SIZE); 

    sha256_init(&ctx);
    sha256_update(&ctx, complete_mac, strlen((char *)complete_mac)); 
    sha256_final(&ctx, hmac_buf); 
    
    
    delete[] archive_text; 
    delete[] right; 
    delete[] complete_mac; 
        

}

bool integrity_test(BYTE *buf, char *archive_name, char* codeFile){
    BYTE *hmac_buf = new BYTE[10000]; 
    hmac_archive(buf, archive_name,hmac_buf);

    std::ifstream t;
    int length;
    t.open(codeFile);   
    t.seekg(0, std::ios::end);    
    length = t.tellg();           
    t.seekg(0, std::ios::beg);    
    char *buffer = new char[length];
    t.read(buffer, length);       
    t.close(); 
    
    BYTE *code = reinterpret_cast<unsigned char*>(buffer); 

    int returnVal = memcmp(hmac_buf, code, length); 
    
    delete[] hmac_buf; 
    delete[] buffer; 

    return returnVal == 0; 

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

void generateIV(BYTE *IV){
    unsigned long long int random_value = 0;
    std::ifstream urandom("/dev/urandom", std::ios::in|std::ios::binary); //Open stream
    size_t size = sizeof(random_value);
    if(urandom){
        for(int idx = 0; idx < 16; idx++){
            urandom.read(reinterpret_cast<char*>(&random_value), size); //Read from urandom
            if(urandom){
                IV[idx] = static_cast<unsigned char>(random_value);
            }
            else {
                std::cerr << "Failed to read from /dev/urandom" << std::endl;
            }
        }
        urandom.close();
    }
    else{
        std::cerr << "Failed to open /dev/urandom" << std::endl;
    }   
}

void print_hex(BYTE str[], int len)
{
	int idx;

	for(idx = 0; idx < len; idx++)
		fprintf(stderr, "%02x", str[idx]);
    std::cout << std::endl;
}

void manage_archive(BYTE *buf, char *archive_name, char **file_names, int files, char* mode){

    //returns -1 if archive is already created
    int created =  mkdir(archive_name, 0775);
    std::cout << "created: " << created << std::endl;
    std::cout << file_names[0] << std::endl; 
    
    char filenamePath[256]; 
    
    strcpy(filenamePath, archive_name); 


    if(strcmp(mode, "add") == 0){
        //std::cout << strcat(archive_name,strcat(slash, "filename.txt")) << std::endl;
        strcat(filenamePath, "/");
        strcat(filenamePath, ".filenames"); 
        std::cout << filenamePath << std::endl; 
        std::ofstream MyFile(filenamePath, std::ofstream::app);

        std::cout << "files: " << files << std::endl;
        for(int i = 0; i < files; i++){
            std::cout << "file_names[i]: " << file_names[i] << std::endl;
            //MyFile << file_names[i] << std::endl; 
            char filePath[256]; 
            
            strcpy(filePath, archive_name);
            strcat(filePath, "/"); 
            strcat(filePath, file_names[i]);
            
            std::ifstream inFile(file_names[i]); 
            
            int length;
            int origLength; 
            inFile.seekg(0, std::ios::end);    
            length = inFile.tellg();  
            origLength = inFile.tellg(); 
            inFile.seekg(0, std::ios::beg);    
            int paddingBytes = length % AES_BLOCK_SIZE; 
            if(paddingBytes == 0){
                length += AES_BLOCK_SIZE;    
            }
            else{
                length += paddingBytes;
            }
            char *buffer = new char[1000];
            std::cout << "Length: " << length << std::endl;  
            for(int idx = 0; idx < origLength; idx++ )
                buffer[idx] = '\0';
            inFile.read(buffer, origLength);       
            inFile.close();                    
            
            
            //int paddingBytes = length % AES_BLOCK_SIZE; 
            for(int idx = origLength; idx < length; idx++ ){
                strcat(buffer, "0"); 
            }
            
            if(paddingBytes >= 0 && paddingBytes < 10 ){
                const char retValue[1] = {(char) paddingBytes};
                strcat(buffer, retValue);
            }
            else if(paddingBytes == 11){
                strcat(buffer,"A");
            }
            else if(paddingBytes == 12) {
                strcat(buffer, "B");
            }
            else if(paddingBytes == 13){
                strcat(buffer, "C");
            }
            else if(paddingBytes == 14){
                strcat(buffer,  "D");
            }
            else{
                strcat(buffer, "E");
            }

            //strcat(buffer, int_to_hex(paddingBytes));

            BYTE *plaintext = reinterpret_cast<unsigned char*>(buffer);


            WORD key_schedule[60];
            BYTE enc_buf[256];
            BYTE *IV = new BYTE [16]; 
            generateIV(IV);

            aes_key_setup(buf, key_schedule, 256); 

            BYTE buf_in[AES_BLOCK_SIZE], buf_out[AES_BLOCK_SIZE], iv_buf[AES_BLOCK_SIZE];
	        int blocks = length/AES_BLOCK_SIZE;


            for (int idx = 0; idx < blocks; idx++) {
		        memcpy(buf_in, &plaintext[idx * AES_BLOCK_SIZE], AES_BLOCK_SIZE);
		        xor_buf(iv_buf, buf_in, AES_BLOCK_SIZE);
		        aes_encrypt(buf_in, buf_out, key_schedule, 256);
		        memcpy(&enc_buf[idx * AES_BLOCK_SIZE], buf_out, AES_BLOCK_SIZE);
		        memcpy(iv_buf, buf_out, AES_BLOCK_SIZE);
	        }
            
            BYTE *ciphertext = new BYTE[16 + length]; 
            memcpy(ciphertext, IV, 16); 
            memcpy(&ciphertext[16], enc_buf, length); 

            std::cout << "before printing to file" << std::endl; 
            print_hex(ciphertext, 16+length); 
        
            std::cout << std::endl;

            //now gotta do hmac checking.
            
            char codeFile[256]; 
            strcat(codeFile, archive_name); 
            strcat(codeFile, "/.code");
            if(created == 0){
                std::ofstream currFile(filePath);
                currFile.write((const char*)ciphertext, sizeof(ciphertext));
                print_hex(ciphertext, 16+length);
                MyFile <<  filePath << std::endl;  
                currFile.close(); 
            }
            else if(FILE *file = fopen(codeFile, "r")){
                if(integrity_test(buf, archive_name, codeFile)){
                    std::ofstream currFile(filePath);
                    currFile.write((const char*)ciphertext, sizeof(ciphertext));
                    MyFile <<  filePath << std::endl;  
                    currFile.close(); 
                    fclose(file); 
                }
                else{
                    std::cout << "Password rejected" << std::endl; 
                    fclose(file);
                    break;
                }
                fclose(file); 
            }
            else{
                std::ofstream currFile(filePath);
                currFile.write((const char*)ciphertext, sizeof(ciphertext));
                MyFile <<  filePath << std::endl;  
                currFile.close(); 
            }

            BYTE *hmac_buf = new BYTE[10000]; 
            hmac_archive(buf, archive_name, hmac_buf);
            std::ofstream code_file(codeFile);
            code_file.write((const char*)hmac_buf, sizeof(hmac_buf));
            code_file.close(); 

            
            delete[] buffer;
            delete[] ciphertext;
            delete[] IV;
        }
        MyFile.close(); 
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
            file_names = new char *[argc-3]; 
            files = argc-3;
            for(int i = 3; i < argc; i++){
                file_names[i-3] = argv[i]; 
                std::cout << file_names[i-3] << std::endl;
            } 
        } else {
            archive_name = argv[4]; 
            file_names = new char *[argc-5];
            //           new char*[argc-5];
            int idx = 0;
            for(int i = 5; i < argc; i++){
                file_names[idx] = argv[i]; 
                std::cout << file_names[idx++] << std::endl; 
            }
            files = idx;
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
    
        manage_archive(buf, archive_name, file_names, files, argv[1]);
    
        delete[] file_names;
    }
    else {
        std::cout << "You have inputted an incorrect command. The options are list, add, extract, and delete." << std::endl; 
    }

}  



