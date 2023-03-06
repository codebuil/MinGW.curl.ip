#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#define BUF_SIZE 1024

int main(int argc,char *argv[]) {
    char hosts[BUF_SIZE];
    char hostsip[BUF_SIZE];
    WSADATA wsaData;
    SOCKET client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];
    char request[BUF_SIZE];
    printf("\033c\033[44;30m");
    strcpy(hosts,"GET / HTTP/1.1\r\nHost: ");
    strcpy(hostsip,"www-microsoft.com");
    if(argc>1)strcpy(hostsip,argv[1]);
    strcat(hosts,hostsip);
    strcat(hosts,"\r\nConnection: close\r\n\r\n");
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Error initializing Winsock.\n");
        return 1;
    }

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket == INVALID_SOCKET) {
        printf("Error creating socket.\n");
        WSACleanup();
        return 1;
    }

    // Connect to server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(hostsip); // IP address of google.com
    server_addr.sin_port = htons(80); // HTTP port
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Error connecting to server.\n");
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    // Construct HTTP request
    sprintf(request, "GET / HTTP/1.1\r\nHost: www.google.com\r\nConnection: close\r\n\r\n");

    // Send HTTP request
    send(client_socket, request, strlen(request), 0);

    // Receive HTTP response
    int recv_bytes;
    while ((recv_bytes = recv(client_socket, buffer, BUF_SIZE - 1, 0)) > 0) {
        buffer[recv_bytes] = '\0';
        printf("%s", buffer);
    }

    // Cleanup
    closesocket(client_socket);
    WSACleanup();

    return 0;
}
