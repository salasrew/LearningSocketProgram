#include <iostream>
// for Linux
#include <sys/socket.h>
// for Windows
// #include <winsock2.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    int port = 8080;
    // 1. 創建一個 socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket == -1)
    {
        std::cerr << "Failed to create a socket." << std::endl;
        return -1;
    }

    // 2. 定義伺服器地址
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;           // 使用 IPv4 位址
    server_addr.sin_port = htons(port);         // 設置Port
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");   // 本地主機位址

    // 3. 連接到伺服器
    if(connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    {
        std::cerr << "Failed to connect to server." << std::endl;
        return -1;
    }

    cout << "Connected to server!" << std::endl;

    // 4. 關閉 socket
    close(client_socket);

    return 0;
}