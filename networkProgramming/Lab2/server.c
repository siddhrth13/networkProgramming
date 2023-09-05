#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Function to count occurrences of a substring in a string
int countSubstring(const char *str, const char *substr) {
    int count = 0;
    const char *ptr = str;
    while ((ptr = strstr(ptr, substr)) != NULL) {
        count++;
        ptr++;
    }
    return count;
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

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Receiving option from client
    memset(buffer, 0, sizeof(buffer));
    read(new_socket, buffer, sizeof(buffer));
    int choice = atoi(buffer);

    switch (choice) {
        case 1: {
            // Search for a string
            memset(buffer, 0, sizeof(buffer));
            read(new_socket, buffer, sizeof(buffer));
            char searchString[BUFFER_SIZE];
            strcpy(searchString, buffer);

            // Opening file
            FILE *file = fopen("sample.txt", "r");
            if (file == NULL) {
                send(new_socket, "File not present", sizeof("File not present"), 0);
                close(new_socket);
                exit(EXIT_SUCCESS);
            }

            // Reading file contents and counting occurrences
            char fileContent[BUFFER_SIZE];
            fread(fileContent, 1, sizeof(fileContent), file);
            fclose(file);

            int count = countSubstring(fileContent, searchString);

            // Sending the count back to the client
            char countStr[BUFFER_SIZE];
            snprintf(countStr, sizeof(countStr), "%d", count);
            send(new_socket, countStr, strlen(countStr), 0);
            close(new_socket);
            break;
        }
        case 2: {
            // Replace a string
            memset(buffer, 0, sizeof(buffer));
            read(new_socket, buffer, sizeof(buffer));
            char str1[BUFFER_SIZE];
            strcpy(str1, buffer);

            memset(buffer, 0, sizeof(buffer));
            read(new_socket, buffer, sizeof(buffer));
            char str2[BUFFER_SIZE];
            strcpy(str2, buffer);

            // Opening file
            FILE *file = fopen("sample.txt", "r");
            if (file == NULL) {
                send(new_socket, "File not present", sizeof("File not present"), 0);
                close(new_socket);
                exit(EXIT_SUCCESS);
            }

            // Reading file contents and replacing
            char fileContent[BUFFER_SIZE];
            fread(fileContent, 1, sizeof(fileContent), file);
            fclose(file);

            char *pos = strstr(fileContent, str1);
            if (pos != NULL) {
                // Replace str1 with str2
                strncpy(pos, str2, strlen(str2));
                send(new_socket, "String replaced", sizeof("String replaced"), 0);
            } else {
                send(new_socket, "String not found", sizeof("String not found"), 0);
            }

            // Writing the updated content back to the file
            file = fopen("sample.txt", "w");
            if (file == NULL) {
                perror("File write error");
                send(new_socket, "File write error", sizeof("File write error"), 0);
                close(new_socket);
                exit(EXIT_FAILURE);
            }
            fwrite(fileContent, 1, strlen(fileContent), file);
            fclose(file);

            close(new_socket);
            break;
        }
        case 3: {
            // Rearrange the entire text in ascending order of ASCII values
            FILE *file = fopen("sample.txt", "r");
            if (file == NULL) {
                send(new_socket, "File not present", sizeof("File not present"), 0);
                close(new_socket);
                exit(EXIT_SUCCESS);
            }

            // Reading file contents
            char fileContent[BUFFER_SIZE];
            fread(fileContent, 1, sizeof(fileContent), file);
            fclose(file);

            // Sorting the content
            for (int i = 0; i < strlen(fileContent) - 1; i++) {
                for (int j = 0; j < strlen(fileContent) - i - 1; j++) {
                    if (fileContent[j] > fileContent[j + 1]) {
                        char temp = fileContent[j];
                        fileContent[j] = fileContent[j + 1];
                        fileContent[j + 1] = temp;
                    }
                }
            }

            // Writing the sorted content back to the file
            file = fopen("sample.txt", "w");
            if (file == NULL) {
                perror("File write error");
                send(new_socket, "File write error", sizeof("File write error"), 0);
                close(new_socket);
                exit(EXIT_FAILURE);
            }
            fwrite(fileContent, 1, strlen(fileContent), file);
            fclose(file);

            send(new_socket, "File rearranged", sizeof("File rearranged"), 0);
            close(new_socket);
            break;
        }
        case 4: {
            close(new_socket);
            break;
        }
        default: {
            close(new_socket);
            break;
        }
    }

    return 0;
}
