#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "header.h"
#include <string.h>

/*
Скорректировать решение задачи о книге контактов из модуля 2 так, чтобы список контактов хранился в файле.
Использовать небуферизованный ввод-вывод.
При запуске программы список считывается из файла, при завершении программы список записывается в файл.
Учесть, что при запуске программы может не быть сохраненных данных.
*/

int main()
{
    Directory list[SIZE];
    int cur_size = 0;
    load(list, &cur_size);
    menu(list, &cur_size);
    save(list, cur_size);
    return 0;
}
