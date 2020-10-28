#include <string.h>
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
    path += username; 

    transform(path.begin(), path.end(), path.begin(), ::tolower);
    
    struct stat info;

    int statRC = stat( path.c_str(), &info );
    if( statRC != 0 )
    {
        if (errno == ENOENT)  { return 0; } 
        if (errno == ENOTDIR) { return 0; } 
        return -1;
    }

    return ( info.st_mode & S_IFDIR ) ? 1 : 0;
}

int messageHandler(std::string message){
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
    while(std::getline(origMessage, line)){
        if(!isData){
            if(line.find("MAIL") != std::string::npos && line.find("FROM:") != std::string::npos){
                int begin = line.find("<");
                int end = line.find(">");
                if(begin == std::string::npos || end == std::string::npos){
                    fprintf(stderr, "FROM Line improperly formatted\n");
                    break; 
                }
                if(checkSender(line.substr(begin+1, end-begin-1))){
                    formattedMessage += "From: " + line.substr(begin+1, end-begin-1) + "\n";
                    // std::cout << formattedMessage << std::endl;
                }
                else{
                    fprintf(stderr, "Sender name improperly formatted\n");
                    std::string emptyResult;
                    return emptyResult;
                }
            }
            else if(line.find("RCPT") != std::string::npos && line.find("TO:") != std::string::npos){
                int begin = line.find("<");
                int end = line.find(">");
                if(begin == std::string::npos || end == std::string::npos){
                    fprintf(stderr,  "RCPT Line improperly formatted");
                    break; 
                } else{
                    recipients += line.substr(begin+1, end-begin-1) + ", "; 

                }
            }
            else if(line == "DATA"){
                formattedMessage += recipients.substr(0, recipients.size()-2);
                formattedMessage += "\n\n";
                isData = true; 
            }
            else{
                fprintf(stderr,  "headers improperly formatted");
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
    for (std::string line; std::getline(std::cin, line);) {
        // std::cout << "Current line: " << line << std::endl;
       
        if(line == "."){
            message += "."; 
            messages.push(message);
            message = ""; 
            endFound = true; 
        }
        else{        
            message += line; 
            message += "\n";
            endFound = false; 
        }

        
    }
    if (!endFound){
        if (messages.size() < 1){
            fprintf(stderr,  "Files inputted incorrectly. Please ensure each message has a terminating period.");
            return 1; 
        }
        else 
            fprintf(stderr,  "Warning: one or more files inputted incorrectly");
            
            
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
