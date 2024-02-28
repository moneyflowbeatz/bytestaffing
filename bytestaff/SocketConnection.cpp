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
    // Отправка запроса
    int sendResult = send(socketDescriptor, reinterpret_cast<const char*>(data.data()), data.size() * sizeof(uint8_t), 0);
    if (sendResult == SOCKET_ERROR) {
        std::cerr << "Ошибка отправки запроса" << std::endl;
        closesocket(socketDescriptor);
        WSACleanup();
        return -1;
    }
    std::cout << "Отправлен запрос типа: " << type << std::endl;
    return 0;
}

//int SocketConnection::Response() {
//    // Получение ответа 
//    while (true) {
//        int recvResult = recv(socketDescriptor, reinterpret_cast<char*>(response), sizeof(response), 0);
//        if (recvResult == SOCKET_ERROR) {
//            std::cerr << "Ошибка приема ответа" << std::endl;
//            closesocket(socketDescriptor);
//            WSACleanup();
//            return -1;
//        }
//
//        // Обработка ответа
//        // ...
//        std::cout << "Полученный ответ: ";
//        for (int i = 0; i < recvResult; i++) {
//            std::cout << "0x" << std::hex << static_cast<int>(response[i]) << " "; // Вывод значений типа uint8_t в шестнадцатеричном формате
//        }
//        std::cout << std::endl << std::endl;
//        break;
//    }
//    return 0; // Возвращаем успешный результат
//}

int SocketConnection::Response() {
    // Получение ответа 
    
    while (true) {
        uint8_t buffer[256]; // Буфер для временного хранения полученных данных
        int recvResult = recv(socketDescriptor, reinterpret_cast<char*>(buffer), sizeof(buffer), 0);
        if (recvResult == SOCKET_ERROR) {
            std::cerr << "Ошибка приема ответа" << std::endl;
            closesocket(socketDescriptor);
            WSACleanup();
            return -1;
        }

        // Добавление полученных данных в вектор
        for (int i = 0; i < recvResult; i++) {
            response.push_back(buffer[i]);
        }

        // Обработка ответа
        // ...

        // Вывод значений вектора в шестнадцатеричном формате
        std::cout << "Полученный ответ: ";
        
        for (uint8_t value : response) {
            std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(value) << " ";
            // Вывод описания соответствующего байта
            switch (value) {
            case 0x00:
                std::cout << " - Старт системы\n";
                break;
            case 0x05:
                std::cout << " - Самодиагностика\n";
                break;
            case 0x0F :
                std::cout << " - Событие программного сторожевого таймера\n";
                break;
            case 0x04:
                std::cout << " - Обновление неизмерительной части ПО\n";
                break;
            case 0x07:
                std::cout << " - Смена адреса\n";
                break;
            case 0x08 :
                std::cout << " - Смена пароля\n";
                break;
            case 0x09:
                std::cout << " - Смена NTP сервера\n";
                break;
            case 0x0B:
                std::cout << " - Смена источника точного времени(GPS,NTP - сервер)\n";
                break;        
            case 0x12:
                std::cout << " - Вкл / Выкл ответа УСПД на ping ICMP\n";
                break;
            case 0x16:
                std::cout << " - Вкл / Выкл автоматического обновления УСПД\n";
                break;
            case 0x17:
                std::cout << " - Изменение настроек автоматического обновления УСПД\n";
                break;
            case 0x18:
                std::cout << " - Изменение интервала опроса сервера автоматического обновления\n";
                break;
            case 0x30:
                std::cout << " - Загрузка конфигурации УСПД\n";
                break;
            case 0x31:
                std::cout << " - Загрузка конфигурации УСПД, конфигурация изменена вручную или повреждена\n";
                break;
            case 0x40:
                std::cout << " - Изменена конфигурация сетевой службы УСПД\n";
                break;
            case 0x50:
                std::cout << " - Изменено состояние вывода PPS(RS232->PPS)\n";
                break;
            case 0x60:
                std::cout << " - Автоматическая очистка данных\n";
                break;
            case 0x61:
                std::cout << " - Ручная очистка данных\n";
                break;
            case 0x01:
                std::cout << " - Синхронизация времени вручную\n";
                break;
            case 0x02:
                std::cout << " - Синхронизация времени по NTP\n";
                break;
            case 0x03:
                std::cout << " - Синхронизация времени по М2М\n";
                break;
            case 0x0A:
                std::cout << " - Коррекция времени вручную\n";
                break;
            case 0x0C:
                std::cout << " - Низкое напряжение в батарее RTC\n";
                break;
            case 0x0D:
                std::cout << " - Нормальное напряжение в батарее RTC\n";
                break;
            case 0x10:
                std::cout << " - Смена часового пояса\n";
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
    return 0; // Возвращаем успешный результат
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