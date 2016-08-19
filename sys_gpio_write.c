#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>

#include <linux/version.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/gpio.h>
#include <linux/irq.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/io.h>

#define GPIO 48
#define GPIO_ADDR 0x4804C000

unsigned int gpioaddr;


struct kobject *example_kobject;

static int foo;

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
        return sprintf(buf, "%d\n", foo);
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr, char *buf, size_t count)
{
//        sscanf(buf, "%du", &foo);
     /* To turn on/off the led */
    switch(buf[0])
    {
        case '0' :      iowrite32(1 << 16, gpioaddr + 0x190);
                        break;
        case '1' :      iowrite32(1 << 16, gpioaddr + 0x194);
                        break;
    }

    return count;
}

static struct kobj_attribute foo_attribute =__ATTR(foo, 0660, foo_show, foo_store);

static int __init mymodule_init(void) 
{
    int error = 0;

    printk(KERN_INFO "Module initialized successfully \n");

    gpioaddr = ioremap(GPIO_ADDR, 0x400);
    iowrite32(~(1 << 16), gpioaddr + 0x134);
    printk(KERN_INFO "GPIO_ADDR is %p \n", gpioaddr);


    example_kobject = kobject_create_and_add("kobject_example", kernel_kobj);
    if(!example_kobject)
         return -ENOMEM;
    error = sysfs_create_file(example_kobject, &foo_attribute.attr);
    if (error) 
    {
        printk(KERN_INFO "failed to create the foo file in /sys/kernel/kobject_example \n");
    }

    return 0;  
}

static void __exit mymodule_exit(void)
{
    iounmap(gpioaddr);
    kobject_put(example_kobject);
    printk (KERN_INFO "Module un initialized successfully \n");
}

module_init(mymodule_init);
module_exit(mymodule_exit);

MODULE_LICENSE("GPL");



