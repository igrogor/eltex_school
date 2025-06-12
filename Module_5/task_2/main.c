#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define SIZE 10

/* Задание 2 по модулю 5: Написать модуль ядра для своей версии ядра, который
будет обмениваться информацией с userspace через proc. Адаптировать для своей
версии ядра (Структура обработчиков). Избавиться от харкода (маг чисел) и
изолировать переменные модуля (static). Результаты выложить на github или др.
общедоступный git. Cсылку на git выслать в ЛС для проверки. Скрины запуска,
работы и тестирования работы модуля прилагаем. */

static int len, temp;
char *msg;

ssize_t read_proc(struct file *filp, char *buf, size_t count, loff_t *offp)
{
    if (count > temp)
    {
        count = temp;
    }
    temp = temp - count;
    copy_to_user(buf, msg, count);
    if (count == 0) temp = len;
    return count;
}

ssize_t write_proc(struct file *filp, const char *buf, size_t count,
                   loff_t *offp)
{
    copy_from_user(msg, buf, count);
    len = count;
    temp = len;
    return count;
}

static const struct proc_ops proc_fops = {
    proc_read : read_proc,
    proc_write : write_proc,
};

void create_new_proc_entry(void)
{  // use of void for no arguments is compulsory now
    proc_create("proc", 0, NULL, &proc_fops);
    msg = kmalloc(SIZE * sizeof(char), GFP_KERNEL);
}

int proc_init(void)
{
    create_new_proc_entry();
    return 0;
}

void proc_cleanup(void)
{
    remove_proc_entry("proc", NULL);
    kfree(msg);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TarasBULBAAA");
module_init(proc_init);
module_exit(proc_cleanup);