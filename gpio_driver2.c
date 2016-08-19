#include <linux/module.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
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
// #define GPIO_ADDR 0x44e10840
#define GPIO_ADDR 0x4804C000

unsigned int gpioaddr;

static int __init gpio_drv_init(void)
{
    printk(KERN_INFO "Inside the %s function\n",__FUNCTION__);

    gpioaddr = ioremap(GPIO_ADDR, 0x400);   // gpio48 is gpio1[16];0x4804c000 which is base address of gpio1;0x400 is size

    iowrite32(~(1 << 16), gpioaddr + 0x134); // to set the gpio1[16] as output; 0x134 is offset of GPIO_OE register(o/p enable)
    iowrite32(1 << 16, gpioaddr + 0x194); // 0x194 - offset of GPIO_SETDATAOUT 

    return 0;
}

static void __exit gpio_drv_exit(void)
{
    printk(KERN_INFO "Inside the %s function\n",__FUNCTION__);
    iowrite32(1 << 16, gpioaddr + 0x190); // 0x190 - offset of GPIO_CLEARDATAOUT
    iounmap(gpioaddr);
}

module_init(gpio_drv_init);
module_exit(gpio_drv_exit);

MODULE_LICENSE("GPL");
