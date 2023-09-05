#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void generatePermutations(char *str, int l, int r) {
    if (l == r) {
        printf("%s\n", str);
    } else {
        for (int i = l; i <= r; i++) {
            // Swap characters at positions l and i
            char temp = str[l];
            str[l] = str[i];
            str[i] = temp;

            // Recursively generate permutations for the remaining characters
            generatePermutations(str, l + 1, r);

            // Restore the original order (backtrack)
            temp = str[l];
            str[l] = str[i];
            str[i] = temp;
        }
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];

    // Creating socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Binding
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listening
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Receiving string from client
    memset(buffer, 0, sizeof(buffer));
    recv(new_socket, buffer, sizeof(buffer), 0);
    printf("Received string from client: %s\n", buffer);

    // Calculate and print permutations
    printf("Permutations of the received string:\n");
    generatePermutations(buffer, 0, strlen(buffer) - 1);

    close(new_socket);
    return 0;
}
