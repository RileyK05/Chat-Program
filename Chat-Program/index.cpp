#pragma comment(lib, "ws2_32.lib")

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
//#define MAX_CLIENTS 1000
//#define BUFFER_SIZE 1024

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <string> 
#include <conio.h>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <vector>
#include <chrono>
#include <thread>
#include <compare>
#include <stdio.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

const int MAX_CLIENTS = 10;
const int BUFFER_SIZE = 1024;

void handleClient(SOCKET clientSocket, int clientIndex, SOCKET* clientSockets) {
    char buffer[BUFFER_SIZE];
    int bytesReceived;

    // Open a file for writing, use append mode to keep previous messages
    std::ofstream chatFile("chatLog.txt", std::ios::app);

    while (true) {
        bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);

        if (bytesReceived <= 0) {
            std::cerr << "Client disconnected. Slot: " << clientIndex << std::endl;
            closesocket(clientSocket);
            clientSockets[clientIndex] = INVALID_SOCKET;
            break;
        }

        buffer[bytesReceived] = '\0';
        std::cout << "Received from client " << clientIndex << ": " << buffer << std::endl;

        chatFile << clientIndex << ": " << buffer << std::endl;
        chatFile.flush();

        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if (i != clientIndex && clientSockets[i] != INVALID_SOCKET) {
                send(clientSockets[i], buffer, bytesReceived, 0);
            }
        }

        const char* acknowledgmentMessage = "Message received successfully.\n";

        //send(clientSocket, acknowledgmentMessage, strlen(acknowledgmentMessage), 0);
        //this message is just a verification message to use in case of error checking
    }


    chatFile.close();
}


int main() {

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock" << std::endl;
        return -1;
    }

    // Create a socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket" << std::endl;
        WSACleanup();
        return -1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080);


    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Error binding socket: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Error listening for connections: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Server listening on port 8080\n";


    SOCKET clientSockets[MAX_CLIENTS];
    std::fill(std::begin(clientSockets), std::end(clientSockets), INVALID_SOCKET);

    while (true) {

        SOCKET newClientSocket = accept(serverSocket, NULL, NULL);
        if (newClientSocket == INVALID_SOCKET) {
            std::cerr << "Error accepting connection: " << WSAGetLastError() << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return -1;
        }

        int clientIndex = -1;
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if (clientSockets[i] == INVALID_SOCKET) {
                clientIndex = i;
                break;
            }
        }

        std::cout << "client index: " << clientIndex << std::endl;

        if (clientIndex == -1) {
            std::cerr << "Maximum number of clients reached. Connection rejected." << std::endl;
            closesocket(newClientSocket);
            continue;
        }
        else {
            clientSockets[clientIndex] = newClientSocket;
            std::cout << "New client connected. Slot: " << clientIndex << std::endl;
        }


        std::thread(handleClient, newClientSocket, clientIndex, clientSockets).detach();
    }

    // Clean up
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
