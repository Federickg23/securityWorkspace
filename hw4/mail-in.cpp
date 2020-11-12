#include <string.h>
#include <strings.h>
#include <dirent.h>
#include <algorithm>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <queue>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

bool checkSender(std::string username){
    std::string path = "./mail/";
    // std::cout << username << std::endl;
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

int messageHandler(std::string message){
    // std::cout << message << std::endl; 
    int begin = message.find("To:");
    int end = message.substr(begin).find("\n");
    std::string userList = message.substr(begin + 4, end-2);
    // std::cout << "Users: " << userList << std::endl;
    std::queue<std::string> users; 
    while(userList.find(",") != std::string::npos){
        users.push(userList.substr(0, userList.find(",")));
        userList = userList.substr(userList.find(",")+2);
    }
    users.push(userList.substr(0, userList.size()-2));

    while(!users.empty()){

        int fd[2];
        pid_t p;
        if (pipe(fd) == -1) {
            fprintf(stderr, "Pipe Failed\n");
            return 1;       
        }

        p = fork();
        if (p < 0) {
            fprintf(stderr, "Fork Failed\n");
            return 1;
        }
        else if (p == 0) {
            // Child process
            // std::cout << "Entering child process" << std::endl;
            // std::cout << users.front() << "." << std::endl;
            close(fd[1]);  // Close the writing end of the pipe
            close(STDIN_FILENO);  // Close the current stdin
            dup2(fd[0], STDIN_FILENO);  // Replace stdin with the reading end of the pipe

            execl("./bin/mail-out", "./bin/mail-out", users.front().c_str(), NULL);  // Invoke mail-out
            // exit(255);  
            return 2; 
        }
        else {
            // Parent process
            int status;
            close(fd[0]);  // Close the reading end of the pipe
            write(fd[1], message.c_str(), strlen(message.c_str()) + 1);
            // p = wait(&status);
            waitpid(p, &status, 0); 
            // std::cout << "p: " << p << std::endl;
            // printf("Child exit status: %d\n", WEXITSTATUS(status));
            
        }
        users.pop(); 

    }

    return 0;

}


std::string parseMessage(std::string message){

    bool isData = false; 
    std::istringstream origMessage (message);
    std::string formattedMessage = "";
    std::string line; 
    std::string recipients = "To: ";

    std::string emptyResult = "";
    int count = 0; 
    while(std::getline(origMessage, line)){
        if(!isData){
            if(line.find("MAIL") != std::string::npos && line.find("FROM:") != std::string::npos){
                if(count != 0){
                    fprintf(stderr, "Control lines out of order\n"); 
                    return emptyResult;
                }
                int begin = line.find("<");
                int end = line.find(">");
                if(begin == std::string::npos || end == std::string::npos){
                    fprintf(stderr, "FROM Line improperly formatted\n");
                    return emptyResult;
                    break; 
                }
                if(checkSender(line.substr(begin+1, end-begin-1))){
                    formattedMessage += "From: " + line.substr(begin+1, end-begin-1) + "\n";
                    // std::cout << formattedMessage << std::endl;
                }
                else{
                    fprintf(stderr, "Sender name improperly formatted\n");
                    return emptyResult;
                }
                count ++; 
            }
            else if(line.find("RCPT") != std::string::npos && line.find("TO:") != std::string::npos){
                if (count < 1){
                    fprintf(stderr, "Control lines out of order\n"); 
                    return emptyResult;
                }
                int begin = line.find("<");
                int end = line.find(">");
                if(begin == std::string::npos || end == std::string::npos){
                    fprintf(stderr,  "RCPT Line improperly formatted\n");
                    break; 
                } else{
                    recipients += line.substr(begin+1, end-begin-1) + ", "; 
                }
                count ++; 
            }
            else if(line == "DATA"){
                formattedMessage += recipients.substr(0, recipients.size()-2);
                formattedMessage += "\n\n";
                isData = true; 
            }
            else{
                fprintf(stderr,  "headers improperly formatted\n");
                return emptyResult; 
            }
        }
        else{
        formattedMessage += line + "\n"; 
        }
    }

    return formattedMessage;

}

int main(){
    std::queue<std::string> messages; 
    
    std::string message = ""; 

    bool endFound = false; 
    bool validMessage = true; 

    float high = 1e9;
    for (std::string line; std::getline(std::cin, line);) {
        // std::cout << "Current line: " << line << std::endl;
        if(line.length() >= high){
            fprintf(stderr, "Input line too long");
            return 1;
        }
       
        if(line == "."){
            message += "."; 
            if(validMessage){
                messages.push(message);
            }
            message = ""; 
            endFound = true; 
            validMessage = true; 
        }
        else{        
            message += line; 
            message += "\n";
            endFound = false; 
        }

        
    }
    if (!endFound){
        if (messages.size() < 1){
            fprintf(stderr,  "Files inputted incorrectly. Please ensure each message has a terminating period.\n");
            // return 1; 
            validMessage = false; 
        }
        else {
            fprintf(stderr,  "Warning: one or more files inputted incorrectly\n");
            // return 1; 
            validMessage = false; 
        }
            
            
    }
    while(!messages.empty()){
        // std::cout << messages.front() << std::endl;
        std::string parsedMsg = parseMessage(messages.front());
        if(!parsedMsg.empty()){
            // std::cout << "Parsed message: " << parsedMsg << std::endl;
            if(messageHandler(parsedMsg) == 2){
                // std::cout << "Found child" << std::endl;
                break; 
            }
        }
        messages.pop();
        // std::cout <<  "message terminated" << std::endl;
    }
    
    

    return 0;

}
