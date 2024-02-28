#include "Server.h"
#include <Winsock2.h>

Server::Server(const char* ipAddress, int port) {
    this->ipAddress = ipAddress;
    this->port = port;
}

Server::~Server() {
    WSACleanup();
}

void Server::Start() {
    // ������������� Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "������ ��� ������������� Winsock.\n";
        return;
    }

    // �������� ������
    listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listeningSocket == INVALID_SOCKET) {
        std::cerr << "������ ��� �������� ������.\n";
        return;
    }

    // �������� ������ � IP-������ � �����
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    hint.sin_addr.s_addr = inet_addr(ipAddress);

    if (bind(listeningSocket, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR) {
        std::cerr << "������ ��� �������� ������.\n";
        closesocket(listeningSocket);
        return;
    }

    // �������� �����������
    if (listen(listeningSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "������ ��� �������� �����������.\n";
        closesocket(listeningSocket);
        return;
    }

    std::cout << "������ �������. �������� �����������...\n";

    // �������� � ��������� �����������
    sockaddr_in client;
    int clientSize = sizeof(client);
    SOCKET clientSocket = accept(listeningSocket, (sockaddr*)&client, &clientSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "������ ��� �������� �����������.\n";
        closesocket(listeningSocket);
        return;
    }

    char host[NI_MAXHOST];      // ���� �������
    char service[NI_MAXSERV];   // ���� �������

    ZeroMemory(host, NI_MAXHOST);
    ZeroMemory(service, NI_MAXSERV);

    if (getsockname(clientSocket, (sockaddr*)&client, &clientSize) == 0) {
        char* ip = inet_ntoa(client.sin_addr);
        int port = ntohs(client.sin_port);
        std::cout << "����������� ����������� �� �����: " << ip << ", �� �����: " << port << std::endl;
    }
    else {
        std::cout << "������ ��� ��������� ���������� � �����������." << std::endl;
    }
    
    ProcessRequest(clientSocket);

    // �������� �������
    closesocket(listeningSocket);
    closesocket(clientSocket);

    // ������� �������� Winsock
    WSACleanup();
}

void Server::ProcessRequest(SOCKET clientSocket) {
    const int bufferSize = 4096;
    char buffer[bufferSize];

    // Receive data from the client
    int bytesReceived = recv(clientSocket, buffer, bufferSize, 0);
    if (bytesReceived == SOCKET_ERROR) {
        std::cerr << "������ ��������� ������ �� �������\n";
        return;
    }

    // Process the received data
    std::string request(buffer, bytesReceived);

    if (request == "Ping") {
        // Prepare the response
        std::string response = "Pong";

        // Send the response back to the client
        int bytesSent = send(clientSocket, response.c_str(), response.size() + 1, 0);
        if (bytesSent == SOCKET_ERROR) {
            std::cerr << "������ �������� ������ �������\n";
        }
    }
    else {
        std::cerr << "������� ����������� ������.\n";
    }
}