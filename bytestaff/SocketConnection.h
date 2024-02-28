#pragma once
#ifndef SOCKET_CONNECTION_H
#define SOCKET_CONNECTION_H

#include <winsock2.h>
#include <string>
#include <vector>


class SocketConnection {
public:
    SocketConnection();
    ~SocketConnection();
    bool connectToServer(const char* ipAddress, int port);
    void closeConnection();
    int Connect(const std::vector<uint8_t>& data, const char* type);
    int Response();
    void ByteStuffing(std::vector<uint8_t>& data);
    uint16_t CalculateCRC16(const std::vector<uint8_t>& data);
    
    // Другие методы для отправки и получения данных
private:
    SOCKET socketDescriptor;
    std::vector<uint8_t> response; // Вектор типа uint8_t для записи ответа
};

#endif
