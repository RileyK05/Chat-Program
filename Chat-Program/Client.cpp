/*
delete the comment out on client side before use





#pragma comment(lib, "ws2_32.lib")

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <conio.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>

const int BUFFER_SIZE = 1024;

void receiveMessages(SOCKET clientSocket) {
    char receiveBuffer[BUFFER_SIZE];
    int receiveBytes;

    while (true) {
        receiveBytes = recv(clientSocket, receiveBuffer, BUFFER_SIZE, 0);

        if (receiveBytes <= 0) {
            std::cerr << "Error receiving message from server" << WSAGetLastError() << std::endl;
            break;
        }

        receiveBuffer[receiveBytes] = '\0';
        std::cout << receiveBuffer << std::endl;
    }
}

int main() {

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock" << std::endl;
        return -1;
    }


    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket" << std::endl;
        WSACleanup();
        return -1;
    }


    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;

    // Use inet_pton to convert the IP address to binary form
    if (inet_pton(AF_INET, "IP address" ////PUT ACTUAL IP ADDRESS IN HERE////, &(serverAddress.sin_addr)) != 1) {
        std::cerr << "Invalid IP address" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    serverAddress.sin_port = htons(8080); // Use the same port as the server


    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Error connecting to server" << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Connected to the server" << std::endl;


    std::cout << "Enter your username: ";
    std::string username;
    std::getline(std::cin, username);


    if (send(clientSocket, username.c_str(), username.size(), 0) == SOCKET_ERROR) {
        std::cerr << "Error sending username to server" << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    // Create a thread to receive messages from the server
    std::thread(receiveMessages, clientSocket).detach();

    // Loop to continuously send messages
    while (true) {
        std::string message;
        std::getline(std::cin, message);

        // Combine username and message into a single string
        std::string fullMessage = username + ": " + message + "\n";

        // Send the combined message to the server
        if (send(clientSocket, fullMessage.c_str(), fullMessage.size(), 0) == SOCKET_ERROR) {
            std::cerr << "Error sending data to server" << WSAGetLastError() << std::endl;
            closesocket(clientSocket);
            WSACleanup();
            return -1;
        }


    }

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}






*/