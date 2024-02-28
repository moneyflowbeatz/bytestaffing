#pragma once
#ifndef SERVER_H
#define SERVER_H
#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <iostream>

class Server {
public:
    Server(const char* ipAddress, int port);
    ~Server();

    void Start();
    void ProcessRequest(SOCKET clientSocket);
    // Другие методы для обработки запросов

private:
    const char* ipAddress;
    int port;
    WSADATA wsaData;
    SOCKET listeningSocket;
};

#endif
#endif
