#include "SocketConnection.h" 
#include <ws2tcpip.h>
#include <iostream> 
#include <ctime>
#include <string>
#include <vector>
#include <iomanip>
#pragma comment(lib, "ws2_32.lib") 

SocketConnection::SocketConnection() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock\n";
    }

    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketDescriptor == INVALID_SOCKET) {
        std::cerr << "Failed to create socket\n";
    }
}

SocketConnection::~SocketConnection() {
    closesocket(socketDescriptor);
    WSACleanup();
}

bool SocketConnection::connectToServer(const char* ipAddress, int port) {
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    if (inet_pton(AF_INET, ipAddress, &(serverAddress.sin_addr)) <= 0) {
        std::cerr << "Invalid address or address not supported\n";
        return false;
    }

    if (connect(socketDescriptor, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect to the server\n";
        return false;
    }

    return true;
}

void SocketConnection::closeConnection() {
    closesocket(socketDescriptor);
}

int SocketConnection::Connect(const std::vector<uint8_t>& data, const char* type) {
    // �������� �������
    int sendResult = send(socketDescriptor, reinterpret_cast<const char*>(data.data()), data.size() * sizeof(uint8_t), 0);
    if (sendResult == SOCKET_ERROR) {
        std::cerr << "������ �������� �������" << std::endl;
        closesocket(socketDescriptor);
        WSACleanup();
        return -1;
    }
    std::cout << "��������� ������ ����: " << type << std::endl;
    return 0;
}

//int SocketConnection::Response() {
//    // ��������� ������ 
//    while (true) {
//        int recvResult = recv(socketDescriptor, reinterpret_cast<char*>(response), sizeof(response), 0);
//        if (recvResult == SOCKET_ERROR) {
//            std::cerr << "������ ������ ������" << std::endl;
//            closesocket(socketDescriptor);
//            WSACleanup();
//            return -1;
//        }
//
//        // ��������� ������
//        // ...
//        std::cout << "���������� �����: ";
//        for (int i = 0; i < recvResult; i++) {
//            std::cout << "0x" << std::hex << static_cast<int>(response[i]) << " "; // ����� �������� ���� uint8_t � ����������������� �������
//        }
//        std::cout << std::endl << std::endl;
//        break;
//    }
//    return 0; // ���������� �������� ���������
//}

int SocketConnection::Response() {
    // ��������� ������ 
    
    while (true) {
        uint8_t buffer[256]; // ����� ��� ���������� �������� ���������� ������
        int recvResult = recv(socketDescriptor, reinterpret_cast<char*>(buffer), sizeof(buffer), 0);
        if (recvResult == SOCKET_ERROR) {
            std::cerr << "������ ������ ������" << std::endl;
            closesocket(socketDescriptor);
            WSACleanup();
            return -1;
        }

        // ���������� ���������� ������ � ������
        for (int i = 0; i < recvResult; i++) {
            response.push_back(buffer[i]);
        }

        // ��������� ������
        // ...

        // ����� �������� ������� � ����������������� �������
        std::cout << "���������� �����: ";
        
        for (uint8_t value : response) {
            std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(value) << " ";
            // ����� �������� ���������������� �����
            switch (value) {
            case 0x00:
                std::cout << " - ����� �������\n";
                break;
            case 0x05:
                std::cout << " - ���������������\n";
                break;
            case 0x0F :
                std::cout << " - ������� ������������ ����������� �������\n";
                break;
            case 0x04:
                std::cout << " - ���������� ��������������� ����� ��\n";
                break;
            case 0x07:
                std::cout << " - ����� ������\n";
                break;
            case 0x08 :
                std::cout << " - ����� ������\n";
                break;
            case 0x09:
                std::cout << " - ����� NTP �������\n";
                break;
            case 0x0B:
                std::cout << " - ����� ��������� ������� �������(GPS,NTP - ������)\n";
                break;        
            case 0x12:
                std::cout << " - ��� / ���� ������ ���� �� ping ICMP\n";
                break;
            case 0x16:
                std::cout << " - ��� / ���� ��������������� ���������� ����\n";
                break;
            case 0x17:
                std::cout << " - ��������� �������� ��������������� ���������� ����\n";
                break;
            case 0x18:
                std::cout << " - ��������� ��������� ������ ������� ��������������� ����������\n";
                break;
            case 0x30:
                std::cout << " - �������� ������������ ����\n";
                break;
            case 0x31:
                std::cout << " - �������� ������������ ����, ������������ �������� ������� ��� ����������\n";
                break;
            case 0x40:
                std::cout << " - �������� ������������ ������� ������ ����\n";
                break;
            case 0x50:
                std::cout << " - �������� ��������� ������ PPS(RS232->PPS)\n";
                break;
            case 0x60:
                std::cout << " - �������������� ������� ������\n";
                break;
            case 0x61:
                std::cout << " - ������ ������� ������\n";
                break;
            case 0x01:
                std::cout << " - ������������� ������� �������\n";
                break;
            case 0x02:
                std::cout << " - ������������� ������� �� NTP\n";
                break;
            case 0x03:
                std::cout << " - ������������� ������� �� �2�\n";
                break;
            case 0x0A:
                std::cout << " - ��������� ������� �������\n";
                break;
            case 0x0C:
                std::cout << " - ������ ���������� � ������� RTC\n";
                break;
            case 0x0D:
                std::cout << " - ���������� ���������� � ������� RTC\n";
                break;
            case 0x10:
                std::cout << " - ����� �������� �����\n";
                break; 
            case 0x73:
                std::cout << " - Start 1\n";
                break;
            case 0x55:
                std::cout << " - Start 2 | Stop\n";
                break;
            default:
                break;
            }
        }
        
        
        std::cout << std::endl << std::endl;

        break;
    }
    return 0; // ���������� �������� ���������
}

uint16_t SocketConnection::CalculateCRC16(const std::vector<uint8_t>& data) {
    uint16_t crc = 0xFFFF;

    for (const auto& byte : data) {
        crc ^= (byte << 8);

        for (int i = 0; i < 8; i++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            }
            else {
                crc <<= 1;
            }
        }
    }

    return crc;
}

void SocketConnection::ByteStuffing(std::vector<uint8_t>& data) {
    for (auto it = data.begin(); it != data.end(); ++it) {
        if (*it == 0x55) {
            *it = 0x73;
            it = data.insert(it + 1, 0x11);
        }
        else if (*it == 0x73) {
            *it = 0x73;
            it = data.insert(it + 1, 0x22);
        }
    }
     
}