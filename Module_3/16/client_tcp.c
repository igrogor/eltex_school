#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define FILE_CMD "file"  // Команда для передачи файла
#define FILE_ACK "ACK_FILE"  // Подтверждение приема файла

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void send_file(int sock, const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        perror("File open error");
        return;
    }

    // Отправляем команду "file"
    send(sock, FILE_CMD, strlen(FILE_CMD), 0);

    // Отправляем имя файла
    send(sock, filename, strlen(filename), 0);

    // Получаем размер файла
    fseek(fp, 0L, SEEK_END);
    long filesize = ftell(fp);
    rewind(fp);
    send(sock, &filesize, sizeof(filesize), 0);

    // Отправляем содержимое
    char buff[20 * 1024];
    while (!feof(fp))
    {
        int n = fread(buff, 1, sizeof(buff), fp);
        send(sock, buff, n, 0);
    }

    fclose(fp);

    // Ждем подтверждение
    recv(sock, buff, sizeof(buff), 0);
    printf("File sent: %s\n", buff);
}

int main(int argc, char *argv[])
{
    int my_sock, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    // char buff[256];
    char buff[1024];
    printf("TCP DEMO CLIENT\n");

    if (argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }
    // извлечение порта
    portno = atoi(argv[2]);

    // Шаг 1 - создание сокета
    my_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (my_sock < 0) error("ERROR opening socket");
    // извлечение хоста
    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    // заполенние структуры serv_addr
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    // установка порта
    serv_addr.sin_port = htons(portno);

    // Шаг 2 - установка соединения
    if (connect(my_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    // Шаг 3 - чтение и передача сообщений
    while ((n = recv(my_sock, &buff[0], sizeof(buff) - 1, 0) ) > 0)
    {
        printf("Enter command (1 - math/2 - file/3 - quit): ");

        // fgets(buff, sizeof(buff), stdin);
        // buff[strcspn(buff, "\n")] = 0;
        int command;
        scanf("%d", &command);

        if (command == 2)
        {
            printf("Enter file path: ");
            fgets(buff, sizeof(buff), stdin);
            buff[strcspn(buff, "\n")] = 0;
            send_file(my_sock, buff);
        }
        else if (command == 3)
        {
            // Корректный выход
            printf("Exit...");
            close(my_sock);
            return 0;
        }
        else if (command == 1)
        {
            // выводим на экран
            printf("S=>C:%s", buff);

            // читаем пользовательский ввод с клавиатуры
            printf("S<=C:");
            fgets(&buff[0], sizeof(buff) - 1, stdin);

            // передаем строку клиента серверу
            send(my_sock, &buff[0], strlen(&buff[0]), 0);
        }
    }
    printf("Recv error \n");
    close(my_sock);
    return -1;

    printf("Please enter the message: ");
    bzero(buff, 256);
    fgets(buff, 255, stdin);
    n = write(my_sock, buff, strlen(buff));
    if (n < 0) error("ERROR writing to socket");

    bzero(buff, 256);
    n = read(my_sock, buff, 255);
    if (n < 0) error("ERROR reading from socket");
    printf("%s\n", buff);

    printf("Please enter the message: ");
    bzero(buff, 256);
    fgets(buff, 255, stdin);
    n = write(my_sock, buff, strlen(buff));
    if (n < 0) error("ERROR writing to socket");

    bzero(buff, 256);
    n = read(my_sock, buff, 255);
    if (n < 0) error("ERROR reading from socket");
    printf("%s\n", buff);
    close(my_sock);
    return 0;
}
