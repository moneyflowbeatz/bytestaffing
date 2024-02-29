# bytestaffing
A dev-program designed to calculate the checksum (CRC16) of packets received through requests to the server using sockets
The program is aimed at helping developers monitor data integrity when processing server requests
## **How you can use it?**
First, you must issue a request to the server, transmitted in bytes. The host and port is specified in the following code:
```
if (connection.connectToServer("213.222.245.173", 61189)) {...
```
If the connection is successful, you can work with the current network
Further in the vector you can arrange the byte stream that will be sent to the server: 
```
std::vector<uint8_t> data01 = {0x73, ...};
```
The next steps are to send the request and convert the response:
```
connection.Connect(data01, "\033[1;31m01 - Ping\033[0m");
connection.Response();
```
And finally, we get the answer, which we put into the clipboard and then output along with the calculated checksum:
```
connection.ByteStuffing(data01);
uint16_t result01 = connection.CalculateCRC16(data01);
std::cout << "\033[1;33mCRC16:\033[0m " << result01 << std::endl;
```
