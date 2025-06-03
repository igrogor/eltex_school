#include <linux/configfs.h>
#include <linux/console_struct.h>
#include <linux/console_struct.h> /* For vc_cons */
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kd.h>
#include <linux/kd.h> /* For KDSETLED */
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/timer.h>
#include <linux/tty.h>
#include <linux/tty.h> /* For fg_console, MAX_NR_CONSOLES */
#include <linux/vt.h>
#include <linux/vt_kern.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TarasBULBAAA");

static int signal = 0;
static struct kobject *example_kobject;
static struct timer_list my_timer;
static struct tty_driver *my_driver;
static int _kbledstatus = 0;
static int saved_led_state = 0;

#define BLINK_DELAY msecs_to_jiffies(200)

static void timer_func(struct timer_list *timer)
{
    if (signal == 0)
    {
        // Restore original state
        if (my_driver && my_driver->ops && my_driver->ops->ioctl)
        {
            my_driver->ops->ioctl(
                vc_cons[fg_console].d->port.tty, KDSETLED, saved_led_state);
        }
        return;
    }

    _kbledstatus = (_kbledstatus == signal) ? 0 : signal;

    if (my_driver && my_driver->ops && my_driver->ops->ioctl)
    {
        my_driver->ops->ioctl(
            vc_cons[fg_console].d->port.tty, KDSETLED, _kbledstatus);
    }

    mod_timer(&my_timer, jiffies + BLINK_DELAY);
}

static ssize_t print_signal(struct kobject *kobj, struct kobj_attribute *attr,
                            char *buf)
{
    return sprintf(buf, "%d\n", signal);
}

static ssize_t set_signal(struct kobject *kobj, struct kobj_attribute *attr,
                          const char *buf, size_t count)
{
    int new_val;
    if (sscanf(buf, "%d", &new_val) != 1) return -EINVAL;

    signal = new_val;

    if (signal != 0)
    {
        mod_timer(&my_timer, jiffies + BLINK_DELAY);
    }
    else
    {
        del_timer_sync(&my_timer);
        if (my_driver && my_driver->ops && my_driver->ops->ioctl)
        {
            my_driver->ops->ioctl(
                vc_cons[fg_console].d->port.tty, KDSETLED, saved_led_state);
        }
    }
    return count;
}

static struct kobj_attribute attribute =
    __ATTR(signal, 0664, print_signal, set_signal);

static int __init main_init(void)
{
    int error = 0;
    struct tty_struct *tty;

    example_kobject = kobject_create_and_add("LedIn", kernel_kobj);
    if (!example_kobject) return -ENOMEM;

    error = sysfs_create_file(example_kobject, &attribute.attr);
    if (error)
    {
        kobject_put(example_kobject);
        return error;
    }

    if (fg_console < 0 || fg_console >= MAX_NR_CONSOLES ||
        !vc_cons[fg_console].d)
    {
        pr_err("Invalid console index\n");
        error = -ENODEV;
        goto error_out;
    }

    tty = vc_cons[fg_console].d->port.tty;
    if (!tty || !tty->driver)
    {
        pr_err("No tty available\n");
        error = -ENODEV;
        goto error_out;
    }

    my_driver = tty->driver;
    if (!my_driver->ops || !my_driver->ops->ioctl)
    {
        pr_err("Driver operations not available\n");
        error = -EOPNOTSUPP;
        goto error_out;
    }

    if (my_driver->ops->ioctl(tty, KDGETLED, (unsigned long)&saved_led_state))
    {
        pr_err("Failed to get LED state\n");
        saved_led_state = 0;
    }

    timer_setup(&my_timer, timer_func, 0);
    return 0;

error_out:
    sysfs_remove_file(example_kobject, &attribute.attr);
    kobject_put(example_kobject);
    return error;
}

static void __exit main_exit(void)
{
    del_timer_sync(&my_timer);

    if (my_driver && my_driver->ops && my_driver->ops->ioctl)
    {
        my_driver->ops->ioctl(
            vc_cons[fg_console].d->port.tty, KDSETLED, saved_led_state);
    }

    sysfs_remove_file(example_kobject, &attribute.attr);
    kobject_put(example_kobject);
}

module_init(main_init);
module_exit(main_exit);