#include <iostream>
#include <vector>
#include <cstdint>
#include <iomanip>
#include "SocketConnection.h"
#include <string>
#include <ws2tcpip.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")




int main()
{
    setlocale(LC_ALL, "ru");
    /*std::vector<uint8_t> data = { 0x73, 0x55, 0x40, 0x00, 0xFF, 0xFF, 0xF0, 0xFF, 0x2D, 0xF0, 0x00, 0x00, 0x06, 0xCA, 0x25, 0x55 };
    ByteStuffing(data);
    std::cout << "Byte-stuffed data: ";
    for (const auto& byte : data) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
    }
    std::cout << std::endl;
    std::cout << "CRC16: " << std::hex << CalculateCRC16(data) << std::endl;*/

    




    SocketConnection connection;
    if (connection.connectToServer("213.222.245.173", 61189)) {
        std::vector<uint8_t> data01 = {0x73, 0x55, 0x62, 0x00, 0xF0, 0xFF, 0xFF, 0xFF, 0x01, 0x84, 0xBB, 0x15, 0x02, 0x00, 0x00, 0x9F, 0xF3, 0x55};
        std::cout << "===============================================" << std::endl;
        connection.Connect(data01, "\033[1;31m01 - Ping\033[0m");
        connection.Response();
        connection.ByteStuffing(data01);
        uint16_t result01 = connection.CalculateCRC16(data01);
        std::cout << "\033[1;33mCRC16:\033[0m " << result01 << std::endl;
        
        

        std::vector<uint8_t> data04 = { 0x73, 0x55, 0x69, 0x00, 0xF0, 0xFF, 0xFF, 0xFF, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x61, 0x64, 0x6D, 0x69, 0x6E, 0x00, 0x30, 0x00, 0x96, 0x20, 0x55 };
        std::cout << "===============================================" << std::endl;
        connection.Connect(data04, "\033[1;31m04 - Authenticate\033[0m");
        connection.Response();
        connection.ByteStuffing(data04);
        uint16_t result04 = connection.CalculateCRC16(data04);
        std::cout << "\033[1;33mCRC16:\033[0m " << result04 << std::endl;



        std::vector<uint8_t> data0A = { 0x73, 0x55, 0x61, 0x00, 0xF0, 0xFF, 0xFF, 0xFF, 0x0A, 0x01, 0xDD, 0x8D, 0x13, 0x02, 0x65, 0x00, 0x55 };
        std::cout << "===============================================" << std::endl;
        connection.Connect(data0A, "\033[1;31m0A - ReadFactoryString\033[0m");
        connection.Response();
        connection.ByteStuffing(data0A);
        uint16_t result0A = connection.CalculateCRC16(data0A);
        std::cout << "\033[1;33mCRC16:\033[0m " << result0A << std::endl;
       


        std::vector<uint8_t> data1B = {0x73, 0x55, 0x60, 0x00, 0xF0, 0xFF, 0xFF, 0xFF, 0x1B, 0x01, 0xDD, 0x8D, 0x13, 0x0A, 0xBD, 0x55};
        std::cout << "===============================================" << std::endl;
        connection.Connect(data1B, "\033[1;31m1B - ReadDataTimeEx\033[0m");
        connection.Response();
        connection.ByteStuffing(data1B);
        uint16_t result1B = connection.CalculateCRC16(data1B);
        std::cout << "\033[1;33mCRC16:\033[0m " << result1B << std::endl;



        std::vector<uint8_t> data30 = { 0x73, 0x55, 0x62, 0x00, 0xF0, 0xFF, 0xFF, 0xFF, 0x30, 0x01, 0xDD, 0x8D, 0x13, 0x00, 0x00, 0x04, 0x66, 0x55 };
        std::cout << "===============================================" << std::endl;
        connection.Connect(data30, "\033[1;31m30 - GetInfo\033[0m");
        connection.Response();
        connection.ByteStuffing(data30);
        uint16_t result30 = connection.CalculateCRC16(data30);
        std::cout << "\033[1;33mCRC16:\033[0m " << result30 << std::endl;

        

        std::vector<uint8_t> data4F = { 0x73, 0x55, 0x60, 0x00, 0xF0, 0xFF, 0xFF, 0xFF, 0x4F, 0x01, 0xDD, 0x8D, 0x13, 0x3E, 0x21, 0x55 };
        std::cout << "===============================================" << std::endl;
        connection.Connect(data4F, "\033[1;31m4F - GetVersion\033[0m");
        connection.Response();
        connection.ByteStuffing(data4F);
        uint16_t result4F = connection.CalculateCRC16(data4F);
        std::cout << "\033[1;33mCRC16:\033[0m " << result4F << std::endl;
        std::cout << "===============================================" << std::endl;
        


        connection.closeConnection();
    }



    return 0;
}