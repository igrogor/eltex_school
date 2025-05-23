#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define FILE_CMD "file"
#define FILE_ACK "ACK_FILE"

void error(const char *msg) {
    perror(msg);
    exit(0);
}

void send_file(int sock, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("File open error");
        return;
    }

    // Отправляем команду "file"
    send(sock, FILE_CMD, strlen(FILE_CMD), 0);

    // Отправляем имя файла с завершающим нулем
    send(sock, filename, strlen(filename) + 1, 0);

    // Получаем размер файла
    fseek(file, 0L, SEEK_END);
    long filesize = ftell(file);
    rewind(file);
    send(sock, &filesize, sizeof(filesize), 0);

    // Отправляем содержимое
    char buff[20 * 1024];
    while (!feof(file)) {
        int n = fread(buff, 1, sizeof(buff), file);
        send(sock, buff, n, 0);
    }
    fclose(file);

    // Ждем подтверждение
    int n = recv(sock, buff, sizeof(buff) - 1, 0);
    buff[n] = '\0';
    printf("File sent: %s\n", buff);
}

int main(int argc, char *argv[]) {
    int my_sock, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buff[20 * 1024];

    printf("TCP DEMO CLIENT\n");

    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);
    my_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (my_sock < 0) error("ERROR opening socket");

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(my_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    while (1) {
        printf("Enter command (1 - math/2 - file/3 - quit): ");
        int command;
        scanf("%d", &command);
        getchar(); // Убираем символ новой строки после scanf

        if (command == 2) {
            printf("Enter file path: ");
            fgets(buff, sizeof(buff), stdin);
            buff[strcspn(buff, "\n")] = '\0';
            send_file(my_sock, buff);
        } else if (command == 3) {
            printf("Exit...\n");
            close(my_sock);
            return 0;
        } else if (command == 1) {
            // Отправляем команду "math"
            send(my_sock, "math", strlen("math"), 0);

            // Получаем запрос на операцию
            n = recv(my_sock, buff, sizeof(buff) - 1, 0);
            buff[n] = '\0';
            printf("S=>C: %s", buff);

            // Отправляем операцию
            printf("Enter operation (+, -, *, /): ");
            fgets(buff, sizeof(buff), stdin);
            buff[strcspn(buff, "\n")] = '\0';
            send(my_sock, buff, strlen(buff), 0);

            // Получаем запрос на первый параметр
            n = recv(my_sock, buff, sizeof(buff) - 1, 0);
            buff[n] = '\0';
            printf("S=>C: %s", buff);

            // Отправляем первый параметр
            printf("Enter first parameter: ");
            fgets(buff, sizeof(buff), stdin);
            buff[strcspn(buff, "\n")] = '\0';
            send(my_sock, buff, strlen(buff), 0);

            // Получаем запрос на второй параметр
            n = recv(my_sock, buff, sizeof(buff) - 1, 0);
            buff[n] = '\0';
            printf("S=>C: %s", buff);

            // Отправляем второй параметр
            printf("Enter second parameter: ");
            fgets(buff, sizeof(buff), stdin);
            buff[strcspn(buff, "\n")] = '\0';
            send(my_sock, buff, strlen(buff), 0);

            // Получаем результат
            n = recv(my_sock, buff, sizeof(buff) - 1, 0);
            buff[n] = '\0';
            printf("Result: %s\n", buff);
        }
    }
    close(my_sock);
    return 0;
}