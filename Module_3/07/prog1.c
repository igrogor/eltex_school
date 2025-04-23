#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <stdlib.h>

/* Задание 07 (Очереди сообщений POSIX, 3 балла).
Написать две программы, использующие одну очередь сообщений для двухстороннего взаимодействия (персональный чат)
Сообщения отправляются по очереди (пинг-понг).
Для завершения обмена предусмотреть отправку данных с заранее известным приоритетом. */

/* int main() {
    mqd_t ds;
    struct mq_attr mes;
    mes.mq_maxmsg = 64;
    mes.mq_msgsize = 256;
    ds = mq_open("/name", O_CREAT|O_RDWR, 0666, &mes);
    char text[] = "fghj";
    mq_send(ds, text, strlen(text), 1);
    mq_close(ds);
    mq_unlink("/name");
    return 0;
} */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <errno.h>

int main() {
    mqd_t ds;
    struct mq_attr mes = {
        .mq_flags = 0,      // Важно: флаги должны быть 0
        .mq_maxmsg = 64,    // Макс. количество сообщений
        .mq_msgsize = 256,  // Размер одного сообщения
        .mq_curmsgs = 0     // Текущее количество сообщений (инициализация)
    };

    // Удаляем очередь, если она существует (игнорируем ошибку, если её нет)
    mq_unlink("/name");

    // Создаём очередь с указанными атрибутами
    ds = mq_open("/name", O_CREAT | O_RDWR, 0666, &mes);
    if (ds == (mqd_t)-1) {
        perror("mq_open failed");
        exit(1);
    }

    char text[] = "fghj";
    if (mq_send(ds, text, strlen(text), 1) == -1) {
        perror("mq_send failed");
    }

    mq_close(ds);
    return 0;
}