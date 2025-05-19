#include <stdio.h>

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

#define str1 "Enter 1 parametr\r\n"
#define str2 "Enter 2 parametr\r\n"
#define str3 "input\t + \t - \t * \t /\r\n"
#define str4 "error input"

#define FILE_CMD "file"
#define FILE_ACK "ACK_FILE"

// прототип функции, обслуживающей
// подключившихся пользователей
void dostuff(int);

void savefile(int sock, char *buff)
{
    char filename[256];
    long filesize;
    FILE *file;

    // Получаем имя файла
    read(sock, filename, sizeof(filename));
    filename[strlen(filename)] = '\0';

    // Получаем размер файла
    read(sock, &filesize, sizeof(filesize));

    // Открываем файл для записи
    file = fopen(filename, "wb");
    if (!file)
    {
        perror("File open error");
        return;
    }

    // Принимаем данные
    long remaining = filesize;
    while (remaining > 0)
    {
        int n = read(sock, buff, sizeof(buff));
        fwrite(buff, 1, n, file);
        remaining -= n;
    }

    fclose(file);
    write(sock, FILE_ACK, strlen(FILE_ACK));
    printf("File %s received (%ld bytes)\n", filename, filesize);
}

// Функция обработки ошибок
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

// глобальная переменная – количество
// активных пользователей
int nclients = 0;

// макрос для печати количества активных
// пользователей
void printusers()
{
    if (nclients)
    {
        printf("%d user on-line\n", nclients);
    }
    else
    {
        printf("No User on line\n");
    }
}

// функция по варианту
int myfunc(int a, int b)
{
    return a + b;
}

// int addition(int arg1, int arg2)
// {
//     return arg1 + arg2;
// }

int subtraction(int arg1, int arg2)
{
    return arg1 - arg2;
}

int multiplication(int arg1, int arg2)
{
    return arg1 * arg2;
}

int division(int arg1, int arg2)
{
    if (arg2 == 0)
    {
        printf("error\n");
        exit(1);
    }
    else
    {
        return (float)arg1 / arg2;
    }
}

int main(int argc, char *argv[])
{
    char buff[1024];  // Буфер для различных нужд
    printf("TCP SERVER DEMO\n");

    int sockfd, newsockfd;  // дескрипторы сокетов
    int portno;             // номер порта
    int pid;                // id номер потока
    socklen_t clilen;  // размер адреса клиента типа socklen_t
    struct sockaddr_in serv_addr,
        cli_addr;  // структура сокета сервера и клиента

    // ошибка в случае если мы не указали порт
    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
    // Шаг 1 - создание сокета
    // AF_INET     - сокет Интернета
    // SOCK_STREAM  - потоковый сокет (с
    //      установкой соединения)
    // 0 - по умолчанию выбирается TCP протокол

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // ошибка при создании сокета
    if (sockfd < 0) error("ERROR opening socket");

    // Шаг 2 - связывание сокета с локальным адресом
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr =
        INADDR_ANY;  // сервер принимает подключения на все IP-адреса
    serv_addr.sin_port = htons(portno);
    // вызываем bind для связывания
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    // Шаг 3 - ожидание подключений, размер очереди - 5
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    // Шаг 4 - извлекаем сообщение из очереди
    // цикл извлечения запросов на подключение из очереди
    while (1)
    {
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0) error("ERROR on accept");
        nclients++;  // увеличиваем счетчик
                     // подключившихся клиентов
                     /*
                     // вывод сведений о клиенте
                     struct hostent *hst;
                     hst = gethostbyaddr((char *)&cli_addr.sin_addr, 4, AF_INET);
                     printf("+%s [%s] new connect!\n",
                     (hst) ? hst->h_name : "Unknown host",
                     (char*)inet_ntoa(cli_addr.sin_addr));*/
        printusers();

        pid = fork();
        if (pid == -1) error("ERROR on fork");
        if (pid == 0)
        {
            close(sockfd);
            dostuff(newsockfd);
            exit(0);
        }
        else
            close(newsockfd);
    } /* end of while */
    close(sockfd);
    return 0; /* we never get here */
}

void dostuff(int sock)
{
    // int n,a,b;
    int bytes_recv;  // размер принятого сообщения
    int a, b;        // переменные для myfunc
    char buff[20 * 1024];

    bytes_recv = read(sock, buff, sizeof(buff));
    if (bytes_recv < 0) error("ERROR reading from socket");
    buff[bytes_recv] = 0;

    if (strncmp(buff, FILE_CMD, strlen(FILE_CMD)) == 0)
    {
        savefile(sock, buff);
    }
    else
    {
        char znak;
        write(sock, str3, sizeof(str3));
        bytes_recv = read(sock, &buff[0], sizeof(buff));
        if (bytes_recv < 0) error("ERROR reading from socket");
        znak = buff[0];
        // printf("%c\n", znak);

        // отправляем клиенту сообщение
        write(sock, str1, sizeof(str1));

        // обработка первого параметра
        bytes_recv = read(sock, &buff[0], sizeof(buff));
        if (bytes_recv < 0) error("ERROR reading from socket");
        a = atoi(buff);  // преобразование первого параметра в int

        // отправляем клиенту сообщение
        write(sock, str2, sizeof(str2));

        bytes_recv = read(sock, &buff[0], sizeof(buff));
        if (bytes_recv < 0) error("ERROR reading from socket");

        b = atoi(buff);  // преобразование второго параметра в int
        switch (znak)
        {
            case '+':
                a = myfunc(a, b);

                snprintf(buff,
                         strlen(buff),
                         "%d",
                         a);  // преобразование результата выполнения в строку
                buff[strlen(buff)] =
                    '\n';  // добавление к сообщению символа конца строки
                break;
            case '-':
                a = subtraction(a, b);

                snprintf(buff,
                         strlen(buff),
                         "%d",
                         a);  // преобразование результата выполнения в строку
                buff[strlen(buff)] =
                    '\n';  // добавление к сообщению символа конца строки
                break;
            case '*':
                a = multiplication(a, b);

                snprintf(buff,
                         strlen(buff),
                         "%d",
                         a);  // преобразование результата выполнения в строку
                buff[strlen(buff)] =
                    '\n';  // добавление к сообщению символа конца строки
                break;
            case '/':
                if (b == 0)
                {
                    strcpy(buff, "error input\n");
                }
                else
                {
                    a = division(a, b);

                    snprintf(
                        buff,
                        strlen(buff),
                        "%d",
                        a);  // преобразование результата выполнения в строку
                    buff[strlen(buff)] =
                        '\n';  // добавление к сообщению символа конца строки
                }

                break;
            default:
                strcpy(buff, str4);
                break;
        }
    }
    // a = myfunc(a, b);  // вызов пользовательской функции

    // отправляем клиенту результат
    write(sock, &buff[0], sizeof(buff));

    nclients--;  // уменьшаем счетчик активных клиентов
    printf("-disconnect\n");
    printusers();
    return;
}
