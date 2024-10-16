#include <iostream>
#include <winsock2.h>
#include <thread> 
#include <vector>

#pragma comment(lib, "ws2_32.lib")

std::vector<SOCKET> clients;

// 處理客戶端連接的函數
void handle_client(SOCKET client_socket)
{
    char buffer[1024];
    int bytes_received;

    // 接收並處理客戶端數據
    while ((bytes_received = recv(client_socket, buffer, 1024, 0)) > 0)
    {
        buffer[bytes_received] = '\0';
        std::cout << "Received: " << buffer << std::endl;

        // 迭代所有客戶端並發送訊息
        for(SOCKET sock: clients)
        {
            // 自己不回傳
            if(sock != client_socket)
            {
                send(sock, buffer, bytes_received, 0);
            }
        }

        // // 回傳數據給客戶端
        // send(client_socket, buffer, bytes_received, 0);
    }
    std::cout << "Client disconnected." << std::endl;
    closesocket(client_socket);  // 關閉客戶端socket
}

int main()
{
    int port = 8080;


    // Windows 環境下初始化 WinSock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "Failed to initialize WinSock." << std::endl;
        return -1;
    }

    // 1. 創建一個 socket Server
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET)
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
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
    {
        std::cerr << "Failed to bind the socket." << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return -1;
    }

    // 4. 監聽連接
    if (listen(server_socket, 5) == SOCKET_ERROR)
    {
        std::cerr << "Failed to listen on the socket." << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return -1;
    }

    std::cout << "Server is listening on port " << std::to_string(port) << "..." << std::endl;

    // 5. 無限循環接受新的客戶端
    sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);

    while (true)
    {
        SOCKET client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_socket == INVALID_SOCKET)
        {
            std::cerr << "Failed to accept the client connection." << std::endl;
            continue; // 繼續等待下個連接
        }


        // 將客戶端 socket 加入 clients 容器
        clients.push_back(client_socket);
        std::cout << "Client connected!" << std::endl;

        // 使用 thread 創建一個新執行緒來處理該客戶端
        std::thread(handle_client, client_socket).detach();
    }

    // 6. 關閉伺服器socket
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
