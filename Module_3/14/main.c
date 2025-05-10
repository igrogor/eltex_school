#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

/* Задание 14 (UDP сокеты, 2 балла)
Реализовать чат для двух клиентов с использованием UDP-сокетов.
 */

int main(int argc, char *argv[])
{
    // if (argc != 3) {
    //     exit(1);
    // }

    int port = atoi(argv[1]);
    
    int peer_port = atoi(argv[2]);
   


    struct sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(port);
    local_addr.sin_addr.s_addr = inet_addr("0.0.0.0");

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr));

    struct sockaddr_in peer_addr;
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_port = htons(peer_port);
    peer_addr.sin_addr.s_addr = inet_addr("0.0.0.0");

    inet_pton(AF_INET, argv[3], &peer_addr.sin_addr);
    inet_pton(AF_INET, argv[3], &peer_addr.sin_addr);


    pid_t pid = fork();

    switch (pid)
    {
        case -1:
            exit(EXIT_FAILURE);
            break;
        case 0:
            char message[BUFFER_SIZE];
            while (1)
            {
                fgets(message, BUFFER_SIZE, stdin);
                sendto(sock,
                       message,
                       strlen(message),
                       0,
                       (struct sockaddr *)&peer_addr,
                       sizeof(peer_addr));
            }
            break;
        default:
            struct sockaddr_in from_addr;
            socklen_t addr_len = sizeof(from_addr);
            char buffer[BUFFER_SIZE];

            while(1) {
                ssize_t len = recvfrom(sock, buffer, BUFFER_SIZE, 0,  (struct sockaddr*)&from_addr, &addr_len); 
                buffer[len] = '\0';
                printf("new message:: %s", buffer);
            }
            break;
    }
    close(sock);
    return 0;
}