#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

/* Задание 1 по модулю 5: Написать модуль ядра Hello World для своей версии
ядра. Поменять описание модуля, добавить себя как автора и придумать свою
лицензию. Результаты выложить на github или др. общедоступный git. Cсылку на git
выслать в ЛС для проверки. Скрины запуска модуля не забываем. */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EGORTERENTEV");

static int __init hello_init(void)
{
    printk(KERN_INFO "Hello world!\n");
    return 0;
}

static void __exit hello_cleanup(void)
{
    printk(KERN_INFO "Cleaning up module. \n");
}

module_init(hello_init);
module_exit(hello_cleanup);