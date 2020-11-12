#include <string.h>
#include <strings.h>
#include <string>
#include <dirent.h>
#include <algorithm>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <queue>
#include <fstream>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <filesystem>

static std::string nextFileName(const char *path)
{
    // std::string::size_type sz;
    struct dirent *entry;
    DIR *dir = opendir(path);
    int val = 0; 
    char fileName[255];
    
    std::string newFile = ""; 

    if (dir == NULL) {
        std::string empty;
        return empty;
    }

    while ((entry = readdir(dir)) != NULL) {
        // printf("%s\n",entry->d_name);
        strncpy(fileName, entry->d_name, 254);
        fileName[254] = '\0';
        if(strcmp(fileName, "..") != 0 && strcmp(fileName, ".") != 0){
            // std::cout << "fileName: " << fileName << std::endl;
            // std::cout << "atoi: " << atoi ( fileName ) << std::endl;
            if (val < atoi ( fileName ))
                val = atoi(fileName);
        }
    }
    val += 1; 
    // std::cout << "Val: " << val << std::endl;
    if(val < 10){
        char c[2];
        sprintf(c, "%d", val);
        newFile += "0000";
        newFile += c;
        // std::cout << newFile << std::endl;
    }
    else if (val < 100){
        char c[3];
        sprintf(c, "%d", val);
        newFile += "000";
        newFile += c;
    }
    else if (val < 1000){
        char c[4];
        sprintf(c, "%d", val);
        newFile += "00";
        newFile += c;
    }
    else if (val < 10000) {
        char c[5];
        sprintf(c, "%d", val);
        newFile += "0";
        newFile += c;
    }
    else {
        char c[6];
        sprintf(c, "%d", val);
        newFile += c;
    }

    closedir(dir);

    return newFile; 
}

bool checkRCPT(std::string username){
    std::string path = "./mail/";


    // transform(path.begin(), path.end(), path.begin(), ::tolower);

    struct stat info;

    // int statRC = stat( path.c_str(), &info );
    // if( statRC != 0 )
    // {
    //     if (errno == ENOENT)  { return 0; } 
    //     if (errno == ENOTDIR) { return 0; } 
    //     return -1;
    // }

    // return ( info.st_mode & S_IFDIR ) ? 1 : 0;

    DIR *d = opendir(path.c_str());
    struct dirent *entry = nullptr;
    if (d != nullptr) {
        while ((entry = readdir(d)))
            if(entry != nullptr){
                if(strcasecmp(entry->d_name, username.c_str())==0){
                    closedir(d);
                    return true;
                }  
                // printf ("%s\n", entry->d_name);
            }
    }
    closedir(d); 
    return false;
}

int main(int argc, char *argv[]){
    // std::cout << "In execl" << std::endl;
    // std::cout << argv[1] << std::endl; 
    std::string rcpt;

    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments\n");
        return -1;
    }

    rcpt = argv[1];
    //run test on user. 
    // std::cout << rcpt << std::endl;
    if(!checkRCPT(rcpt)){
        fprintf(stderr, "Invalid recipient: ");
        fprintf(stderr, rcpt.c_str());
        fprintf(stderr, "\n");
        return -2; 
    }

    std::string message = ""; 
    std::string line; 
    while (std::getline(std::cin, line)) {
        // std::cout << "Current line: " << line << std::endl;
        if(line.front() == '.'){
            if(line == ".")
                break;
            message += line.substr(1) + "\n"; 
            
        }
        else {
            message += line + "\n";
        }
    }

    std::string filepath = "./mail/"+rcpt;
    std::string newFile = nextFileName(filepath.c_str());
    // std::cout << "Next file: " << newFile << std::endl; 
    
    rcpt += "/"; 
    newFile.insert(0, rcpt);
    newFile.insert(0, "./mail/");
    // std::cout << "Filepath: " << newFile << std::endl; 
    // printf ("Next file: %s\n", newFile.c_str());
    std::ofstream MyFile(newFile.c_str(), std::ofstream::app);
    MyFile.write(message.c_str(), message.size());
    MyFile.close();
    
    // std::cout << "File closed" << std::endl;
    return 0; 
}