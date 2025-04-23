#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <stdlib.h>
/* int main() {
    mqd_t ds;
    struct mq_attr mes;
    int prio;
    ds = mq_open("/name", O_RDWR);
    //mq_send(ds, "dfsf", 4, 1);
    char ff[12];
    mq_receive(ds, ff, 256, &prio);
    printf("%s\n", ff);
    mq_close(ds);
    return 0;
} */



int main() {
    mqd_t ds;
    struct mq_attr mes;
    int prio;

    // Открываем очередь только для чтения (без O_CREAT, чтобы не создавать новую)
    ds = mq_open("/name", O_RDONLY);
    if (ds == (mqd_t)-1) {
        perror("mq_open failed");
        exit(1);
    }

    // Получаем атрибуты очереди
    if (mq_getattr(ds, &mes) == -1) {
        perror("mq_getattr failed");
        exit(1);
    }

    // Выделяем буфер достаточного размера (+1 для '\0')
    char ff[mes.mq_msgsize + 1];

    // Принимаем сообщение
    ssize_t received = mq_receive(ds, ff, mes.mq_msgsize, &prio);
    if (received == -1) {
        perror("mq_receive failed");
        exit(1);
    }

    ff[received] = '\0'; // Добавляем нуль-терминатор
    printf("Received: %s\n", ff);

    mq_close(ds);
    mq_unlink("/name"); // Удаляем очередь после использования
    return 0;
}