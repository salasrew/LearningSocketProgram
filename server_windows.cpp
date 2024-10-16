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

    // 1. 創建一個 socket Server
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == INVALID_SOCKET)
    {
        std::cerr << "Failed to create a socket." << std::endl;
        WSACleanup();
        return -1;
    }

    // 2. 定義伺服器地址
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;           // 使用 IPv4 位址
    server_addr.sin_port = htons(port);         // 設置Port
    server_addr.sin_addr.s_addr = INADDR_ANY;   // 使用任何可用的 IP 地址

    // 3. 綁定 socket 和 IP 地址
    if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
    {
        std::cerr << "Failed to bind the socket." << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return -1;
    }

    // 4. 監聽連接
    if(listen(server_socket , 5 ) == SOCKET_ERROR )
    {
        std::cerr << "Failed to listen on the socket." << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return -1;
    }

    std::cout << "Server is listening on port " << std::to_string(port) << "..." << std::endl;

    // 5. 接受連接
    sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);
    SOCKET client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
    if(client_socket == INVALID_SOCKET)
    {
        std::cerr << "Failed to accept the client connection." << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return -1;
    }

    std::cout << "Client connected!" << std::endl;

    // 6. 接收客戶端訊息
    char buffer[1024];
    int bytes_received = recv(client_socket, buffer, 1024, 0);
    if(bytes_received > 0)
    {
        buffer[bytes_received] = '\0';
        std::cout << "Received: " << buffer << std::endl;
    }

    // 7. 回應客戶端
    std::string response = "Hello from server!";
    send(client_socket, response.c_str(), response.size(), 0);

    // 8. 關閉 socket
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}