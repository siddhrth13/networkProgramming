#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void sortNumbers(char *input, char *output) {
    int numbers[100], sortedNumbers[100];
    int numCount = 0;

    // Extract numbers from input string
    char *token = strtok(input, " ");
    while (token != NULL) {
        numbers[numCount++] = atoi(token);
        token = strtok(NULL, " ");
    }

    // Sort the numbers in ascending order
    for (int i = 0; i < numCount; i++) {
        sortedNumbers[i] = numbers[i];
    }

    for (int i = 0; i < numCount - 1; i++) {
        for (int j = 0; j < numCount - i - 1; j++) {
            if (sortedNumbers[j] > sortedNumbers[j + 1]) {
                int temp = sortedNumbers[j];
                sortedNumbers[j] = sortedNumbers[j + 1];
                sortedNumbers[j + 1] = temp;
            }
        }
    }

    // Convert the sorted numbers back to a string
    output[0] = '\0';
    for (int i = 0; i < numCount; i++) {
        char numStr[20];
        sprintf(numStr, "%d", sortedNumbers[i]);
        strcat(output, numStr);
        strcat(output, " ");
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];
    char sortedNumbersBuffer[BUFFER_SIZE];
    char sortedCharsBuffer[BUFFER_SIZE];

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

    // Receiving alphanumeric string from client
    memset(buffer, 0, sizeof(buffer));
    recv(new_socket, buffer, sizeof(buffer), 0);
    printf("Received string from client: %s\n", buffer);

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process: Sort numbers in ascending order
        sortNumbers(buffer, sortedNumbersBuffer);
        printf("Child Process ID: %d\n", getpid());
        send(new_socket, sortedNumbersBuffer, strlen(sortedNumbersBuffer), 0);
        close(new_socket);
        exit(EXIT_SUCCESS);
    } else {
        // Parent process: Sort characters in descending order
        int len = strlen(buffer);
        for (int i = 0; i < len; i++) {
            for (int j = i + 1; j < len; j++) {
                if (buffer[i] < buffer[j]) {
                    char temp = buffer[i];
                    buffer[i] = buffer[j];
                    buffer[j] = temp;
                }
            }
        }
        printf("Parent Process ID: %d\n", getpid());
        send(new_socket, buffer, strlen(buffer), 0);
        close(new_socket);
    }

    return 0;
}
