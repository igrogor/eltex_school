

/*
Задание 16 (TCP сокеты, 3 балла)
Добавить в программу из Задания 15 возможность передачи файлов.
*/

/* Пример простого TCP сервера
   Порт является аргументом, для запуска сервера неободимо ввести:
   # ./[имя_скомпилированного_файла] [номер порта]
   # ./server 57123
*/


#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define str1 "Enter 1 parameter\r\n"
#define str2 "Enter 2 parameter\r\n"
#define str3 "input\t + \t - \t * \t /\r\n"
#define str4 "error input"

#define FILE_CMD "file"
#define FILE_ACK "ACK_FILE"

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int nclients = 0;

void printusers() {
    if (nclients) {
        printf("%d user on-line\n", nclients);
    } else {
        printf("No User on line\n");
    }
}

void savefile(int sock) {
    char filename[256];
    long filesize;
    FILE *file;
    char buff[20 * 1024];

    // Получаем имя файла
    int n = recv(sock, filename, sizeof(filename) - 1, 0);
    filename[n] = '\0';

    // Получаем размер файла
    recv(sock, &filesize, sizeof(filesize), MSG_WAITALL);

    // Открываем файл для записи
    file = fopen(filename, "wb");
    if (!file) {
        perror("File open error");
        return;
    }

    // Принимаем данные
    long remaining = filesize;
    while (remaining > 0) {
        n = recv(sock, buff, sizeof(buff), 0);
        fwrite(buff, 1, n, file);
        remaining -= n;
    }
    fclose(file);

    write(sock, FILE_ACK, strlen(FILE_ACK));
    printf("File %s received (%ld bytes)\n", filename, filesize);
}

void dostuff(int sock) {
    char buff[20 * 1024];
    while (1) {
        int bytes_recv = recv(sock, buff, sizeof(buff) - 1, 0);
        if (bytes_recv <= 0) break;
        buff[bytes_recv] = '\0';

        if (strncmp(buff, FILE_CMD, strlen(FILE_CMD)) == 0) {
            savefile(sock);
        } else if (strncmp(buff, "math", strlen("math")) == 0) {
            // Отправляем запрос на операцию
            write(sock, str3, strlen(str3));

            // Получаем операцию
            bytes_recv = recv(sock, buff, sizeof(buff) - 1, 0);
            if (bytes_recv <= 0) break;
            buff[bytes_recv] = '\0';
            char znak = buff[0];

            // Отправляем запрос на первый параметр
            write(sock, str1, strlen(str1));

            // Получаем первый параметр
            bytes_recv = recv(sock, buff, sizeof(buff) - 1, 0);
            if (bytes_recv <= 0) break;
            buff[bytes_recv] = '\0';
            int a = atoi(buff);

            // Отправляем запрос на второй параметр
            write(sock, str2, strlen(str2));

            // Получаем второй параметр
            bytes_recv = recv(sock, buff, sizeof(buff) - 1, 0);
            if (bytes_recv <= 0) break;
            buff[bytes_recv] = '\0';
            int b = atoi(buff);

            // Выполняем операцию
            int result;
            switch (znak) {
                case '+':
                    result = a + b;
                    snprintf(buff, sizeof(buff), "%d\n", result);
                    break;
                case '-':
                    result = a - b;
                    snprintf(buff, sizeof(buff), "%d\n", result);
                    break;
                case '*':
                    result = a * b;
                    snprintf(buff, sizeof(buff), "%d\n", result);
                    break;
                case '/':
                    if (b == 0) {
                        strcpy(buff, "error: division by zero\n");
                    } else {
                        result = a / b;
                        snprintf(buff, sizeof(buff), "%d\n", result);
                    }
                    break;
                default:
                    strcpy(buff, "error: invalid operation\n");
                    break;
            }
            write(sock, buff, strlen(buff));
        }
    }
    nclients--;
    printf("-disconnect\n");
    printusers();
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno, pid;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    printf("TCP SERVER DEMO\n");

    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0) error("ERROR on accept");
        nclients++;
        printusers();

        pid = fork();
        if (pid == -1) error("ERROR on fork");
        if (pid == 0) {
            close(sockfd);
            dostuff(newsockfd);
            exit(0);
        } else {
            close(newsockfd);
        }
    }
    close(sockfd);
    return 0;
}
