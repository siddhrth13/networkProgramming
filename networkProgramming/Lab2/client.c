#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];

    // Creating socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Converting IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connecting to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // User menu
    int choice;
    printf("Choose an option:\n");
    printf("1. Search for a string in the file\n");
    printf("2. Replace a string in the file\n");
    printf("3. Rearrange the file content\n");
    printf("4. Quit\n");
    scanf("%d", &choice);

    // Send user's choice to the server
    snprintf(buffer, sizeof(buffer), "%d", choice);
    send(sock, buffer, strlen(buffer), 0);

    switch (choice) {
        case 1: {
            // Search for a string
            printf("Enter the string to search: ");
            scanf("%s", buffer);
            send(sock, buffer, strlen(buffer), 0);

            // Receive and display the response from the server
            memset(buffer, 0, sizeof(buffer));
            valread = read(sock, buffer, sizeof(buffer));
            printf("Occurrences of the string: %s\n", buffer);
            break;
        }
        case 2: {
            // Replace a string
            printf("Enter the string to replace: ");
            scanf("%s", buffer);
            send(sock, buffer, strlen(buffer), 0);

            printf("Enter the replacement string: ");
            scanf("%s", buffer);
            send(sock, buffer, strlen(buffer), 0);

            // Receive and display the response from the server
            memset(buffer, 0, sizeof(buffer));
            valread = read(sock, buffer, sizeof(buffer));
            printf("%s\n", buffer);
            break;
        }
        case 3: {
            // Rearrange the file content
            // Receive and display the response from the server
            memset(buffer, 0, sizeof(buffer));
            valread = read(sock, buffer, sizeof(buffer));
            printf("%s\n", buffer);
            break;
        }
        case 4: {
            // Quit
            printf("Quitting...\n");
            break;
        }
        default: {
            printf("Invalid choice\n");
            break;
        }
    }

    close(sock);
    return 0;
}
