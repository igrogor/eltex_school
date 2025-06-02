#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/string.h>
#include <linux/sysfs.h>

#include <linux/module.h>
#include <linux/configfs.h>
#include <linux/init.h>
#include <linux/tty.h>          /* For fg_console, MAX_NR_CONSOLES */
#include <linux/kd.h>           /* For KDSETLED */
#include <linux/vt.h>
#include <linux/console_struct.h>       /* For vc_cons */
#include <linux/vt_kern.h>
#include <linux/timer.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TarasBULBAAA");

static int signal;
static struct kobject *example_kobject;

struct timer_list my_timer;
struct tty_driver *my_driver;

static int _kbledstatus = 0;

#define BLINK_DELAY   HZ/5
#define ALL_LEDS_ON   0x07
#define RESTORE_LEDS  0xFF


static void timer_func(struct timer_list *timer) {
    if (_kbledstatus == signal)
    {
        _kbledstatus = RESTORE_LEDS;
    }else{
        _kbledstatus = signal;
        (my_driver->ops->ioctl)(vc_cons[fg_console].d->port.tty, KDSETLED, _kbledstatus);
        my_timer.expires = jiffies + BLINK_DELAY;
        add_timer(&my_timer);
    }
}


static ssize_t print_signal(struct kobject *kobj, struct kobj_attribute *attr,
                            char *buf)
{
    return sprintf(buf, "%d\n", signal);
}

static ssize_t set_signal(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    sscanf(buf, "%du", &signal);
    return count;
}

static struct kobj_attribute attribute =
    __ATTR(signal, 0775, print_signal, set_signal);



static int __init main_init(void)
{
    int error = 0;
    example_kobject = kobject_create_and_add("LedIn", kernel_kobj);
    error = sysfs_create_file(example_kobject, &attribute.attr);

    for (int i = 0; i < MAX_NR_CONSOLES; i++)
    {
        if(!vc_cons[i].d) break;
    }
    my_driver = vc_cons[fg_console].d->port.tty->driver;
    timer_setup(&my_timer, timer_func, 0);
    my_timer.expires = jiffies + BLINK_DELAY;
    add_timer(&my_timer);

    return error;
}

static void __exit main_exit(void)
{
    kobject_put(example_kobject);
}

module_init(main_init);
module_exit(main_exit);



