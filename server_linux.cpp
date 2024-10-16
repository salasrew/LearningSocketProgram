#include <iostream>
// for Linux
#include <sys/socket.h>
// #include <netinet/in.h>
// for Windows
// #include <winsock2.h>
#include <unistd.h>

// 伺服器 (Server)：

// 監聽指定的 IP 地址和端口，等待客戶端連接。
// 接受客戶端連接後，為每個客戶端創建一個獨立的執行緒來處理通信。
// 接收來自任一客戶端的消息，並將其轉發給所有其他連接的客戶端。

// socket()：用於創建伺服器端的 socket，指定 AF_INET（IPv4 協議）和 SOCK_STREAM（TCP 協議）。
// bind()：將伺服器 socket 綁定到特定的 IP 地址和端口號。
// listen()：使伺服器開始監聽連接，第二個參數表示伺服器允許的最大未處理連接數。
// accept()：接受一個客戶端的連接，並返回一個新的 socket，該 socket 將用來與客戶端進行通信。
// close()：關閉伺服器和客戶端的 socket。

int main()
{
    int port = 8080;

    // 1. 創建一個 socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == -1)
    {
        std::cerr << "Failed to create a socket." << std::endl;
        return -1;
    }

    // 2. 定義伺服器地址
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;           // 使用 IPv4 位址
    server_addr.sin_port = htons(port);         // 設置Port
    server_addr.sin_addr.s_addr = INADDR_ANY;   // 使用任何可用的 IP 地址

    // 3. 綁定 socket 和 IP 地址
    if(bind(server_socket, (struct server_addr*)&server_addr, sizeof(server_addr)) == -1)
    {
        std::cerr << "Failed to bind the socket." << std::endl;
        return -1;
    }

    // 4. 監聽連接
    if(listen(server_socket, 3) == -1)          // 最大等待連線數 3
    {
        std::cerr << "Failed to listen on the socket." << std::endl;
        return -1;
    }

    cout << "Server is listening on port " <<  to_string(port) << "..." << endl;

    // 5. 接受連接
    int client_socke = accept(server_socket, NULL, NULL);
    if(client_socket == -1)
    {
        std::cerr << "Failed to accept the client connection." << std::endl;
        return -1;
    }

    cout << "Client connected!" << std::endl;

    // 6. 關閉 socket
    close(client_socket);
    close(server_socket);

    return 0;
}