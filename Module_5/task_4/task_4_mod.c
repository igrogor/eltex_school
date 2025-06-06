#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/poll.h>

#define SUCCESS 0
#define DEVICE_NAME "chardev"
#define BUF_LEN 80

static int major;

enum
{
    CDEV_NOT_USED = 0,
    CDEV_EXCLUSIVE_OPEN = 1,
};

char *mes;

static char default_msg[BUF_LEN] = "Hello world!";
static char display_msg[BUF_LEN];
static char user_msg[BUF_LEN];

static int counter = 0;

static atomic_t already_open = ATOMIC_INIT(CDEV_NOT_USED);
static struct class *cls;

static int device_open(struct inode *inode, struct file *file);
static int device_release(struct inode *inode, struct file *file);
static ssize_t device_read(struct file *filp, char __user *buffer,
                           size_t length, loff_t *offset);
static ssize_t device_write(struct file *filp, const char __user *buff,
                            size_t len, loff_t *off);

static struct file_operations chardev_fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release,
};

static int device_open(struct inode *inode, struct file *file)
{
    if (atomic_cmpxchg(&already_open, CDEV_NOT_USED, CDEV_EXCLUSIVE_OPEN) !=
        CDEV_NOT_USED)
        return -EBUSY;

    if (user_msg[0] == '\0')
    {
        // Используем сообщение по умолчанию
        snprintf(display_msg,
                 BUF_LEN,
                 "I already told you %d times %s",
                 counter,
                 default_msg);
    }
    else
    {
        // Используем пользовательское сообщение
        snprintf(display_msg,
                 BUF_LEN,
                 "I already told you %d times %s",
                 counter,
                 user_msg);
    }
    try_module_get(THIS_MODULE);
    return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
    atomic_set(&already_open, CDEV_NOT_USED);
    module_put(THIS_MODULE);
    return SUCCESS;
}

static ssize_t device_read(struct file *filp, char __user *buffer,
                           size_t length, loff_t *offset)
{
    size_t msg_len = strlen(display_msg);
    size_t bytes_to_read;

    if (*offset >= msg_len) return 0;

    bytes_to_read = min(length, msg_len - *offset);

    if (copy_to_user(buffer, display_msg + *offset, bytes_to_read))
        return -EFAULT;

    *offset += bytes_to_read;
    return bytes_to_read;
    /*     int bytes_read = 0;
        const char *msg_ptr = msg;
        if (!*(msg_ptr + *offset))
        {
            *offset = 0;
            return 0;
        }
        msg_ptr += *offset;
        while (length && *msg_ptr)
        {
            put_user(*(msg_ptr++), buffer++);
            length--;
            bytes_read++;
        }
        *offset += bytes_read;
        return bytes_read; */
}

static ssize_t device_write(struct file *filp, const char __user *buff,
                            size_t len, loff_t *off)
{
    if (len >= BUF_LEN) return -EINVAL;

    memset(user_msg, 0, BUF_LEN);

    if (copy_from_user(user_msg, buff, len)) return -EFAULT;

    if (len > 0 && user_msg[len - 1] == '\n')
        user_msg[len - 1] = '\0';
    else
        user_msg[len] = '\0';

    return len;
}

static int __init chardev_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &chardev_fops);
    if (major < 0)
    {
        pr_alert("error, device failed with %d\n", major);
        return major;
    }
    pr_info("I was assigned major number %d.\n", major);
    cls = class_create(DEVICE_NAME);
    device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    pr_info("Device created on /dev/%s\n", DEVICE_NAME);
    return SUCCESS;
}

static void __exit chardev_exit(void)
{
    device_destroy(cls, MKDEV(major, 0));
    class_destroy(cls);
    unregister_chrdev(major, DEVICE_NAME);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TarasBULBAAA");

module_init(chardev_init);
module_exit(chardev_exit);