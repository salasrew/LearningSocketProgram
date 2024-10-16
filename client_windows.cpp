#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
    int port = 8080;
    // Windows 環境下初始化 WinSock
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "Failed to initialize WinSock." << std::endl;
        return -1;
    }

    // 1. 創建一個 socket Client
    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket == INVALID_SOCKET)
    {
        std::cerr << "Failed to create a socket." << std::endl;
        WSACleanup();
        return -1;
    }

    // 2. 定義伺服器地址
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;           // 使用 IPv4 位址
    server_addr.sin_port = htons(port);         // 設置Port
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");   // 本地主機位址

    // 3. 連接到伺服器
    if(connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
    {
        std::cerr << "Failed to connect to server." << std::endl;
        closesocket(client_socket);
        WSACleanup();
        return -1;
    }

    std::cout << "Connected to server!" << std::endl;

    // 4. 發送訊息給伺服器
    std::string message = "Hello from client!";
    send(client_socket, message.c_str(), message.size(), 0);

    // 5. 接收伺服器的回應
    char buffer[1024];
    int bytes_received = recv(client_socket, buffer, 1024, 0);
    if(bytes_received == SOCKET_ERROR)
    {
        std::cerr << "Failed to receive data from server." << std::endl;
    }
    else
    {
        buffer[bytes_received] = '\0';
        std::cout << "Server says: " << buffer << std::endl;
    }
    
    // 6. 關閉 socket
    closesocket(client_socket);
    WSACleanup();

    return 0;
}