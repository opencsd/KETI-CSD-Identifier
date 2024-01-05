#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <thread>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h" 

#include "keti_log.h"

#define CSD_PROXY_PORT 40300
#define CSD_INPUT_INTERFACE_PORT 40303
#define BUFF_SIZE 4096

using namespace std;
using namespace rapidjson;

void SendSnippetToCSD(string Snippet);

//Run Proxy Server
int main(int argc, char** argv){
    if (argc >= 2) {
        KETILOG::SetLogLevel(stoi(argv[1]));
    }else{
        KETILOG::SetDefaultLogLevel();
    }

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(CSD_PROXY_PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 8) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::string msg = "Run CSD Proxy Server (" +std::string(inet_ntoa(address.sin_addr))+":" +std::to_string(CSD_PROXY_PORT) +")";
    KETILOG::WARNLOG("CSD Proxy", msg);

	while (1){
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
            perror("accept");
            exit(EXIT_FAILURE);
        }

        KETILOG::TRACELOG("CSD Proxy", "Accepted Pushdown Snippet Request");

        char socketsize[4];
        char ipaddr[100];
        
        std::string sockbuf = "";
        char buffer[BUFF_SIZE] = {0};
        size_t length;
        read( new_socket , &length, sizeof(length));

        int numread;
        while(1) {
            if ((numread = read( new_socket , buffer, BUFF_SIZE - 1)) == -1) {
                perror("read");
                exit(1);
            }
            length -= numread;
            buffer[numread] = '\0';
            sockbuf += buffer;

            if (length == 0)
                break;
        }

        thread(SendSnippetToCSD,sockbuf).detach();

        close(new_socket);
	}

    close(server_fd);

    return 0;
}

//Send Snippet To Selected CSD
void SendSnippetToCSD(string pushdownSnippet){
    Document document;
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	
	document.Parse(pushdownSnippet.c_str());
	string ipaddr = document["csdIP"].GetString();

    KETILOG::DEBUGLOG("CSD Proxy", "Send Pushdown Snippet To CSD#"+ipaddr);

    StringBuffer snippetBuf;
    Writer<StringBuffer> writer(snippetBuf);
    document["Snippet"].Accept(writer);
    string snippet = snippetBuf.GetString();
    
    KETILOG::TRACELOG("CSD Proxy",snippet);

    struct sockaddr_in serv_addr;
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ipaddr.c_str());
    serv_addr.sin_port = htons(CSD_INPUT_INTERFACE_PORT);

    connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

    size_t len = strlen(snippet.c_str());
    send(sock,&len,sizeof(len),0);
    send(sock,(char*)snippet.c_str(),strlen(snippet.c_str()),0);
    
    close(sock);
}
