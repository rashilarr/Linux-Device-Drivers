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

//static dev_t first;

static dev_t first; 
static struct cdev c_dev; 
static struct class *cl;


int gpiochar;
unsigned int gpioaddr;

int file_open (struct inode *pinode, struct file *pfile)
{
   // int ret;
    printk(KERN_INFO "Inside the %s function\n",__FUNCTION__);

    /* ioremap  */
    gpioaddr = ioremap(GPIO_ADDR, 0x400);
    iowrite32(~(1 << 16), gpioaddr + 0x134);
    printk(KERN_INFO "GPIO_ADDR is %d \n", gpioaddr);


    return 0;
}

size_t file_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
    unsigned int gpio_datain;
    char str[2];
    printk(KERN_INFO "Inside the %s function\n",__FUNCTION__);

    gpio_datain = ioread32(gpioaddr + 0x138);
    printk("GPIO Val = %x\n", gpio_datain);
    if(gpio_datain & (1 << 17))
        str[0] = '1';
    else
        str[0] = '0';
    str[1] = '\0';
    /* To read the value of led */
    if(copy_to_user(buffer, str, 2) != 0)
        return EFAULT;
    
    return 0;
}

ssize_t file_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
    printk(KERN_INFO "Inside the %s function\n",__FUNCTION__);

    /* To turn on/off the led */ 
    switch(buffer[0])
    {
        case '0' :	iowrite32(1 << 16, gpioaddr + 0x190);
                        break;
        case '1' :	iowrite32(1 << 16, gpioaddr + 0x194);
                        break;
    }
    
    return length;
}

int file_close (struct inode *pinode, struct file *pfile)
{
    printk(KERN_INFO "Inside the %s function\n",__FUNCTION__);
    
    iounmap(gpioaddr);
    return 0;
}


/* To hold the file operations performed on the device */
struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = file_open,
    .read    = file_read,
    .write   = file_write,
    .release = file_close,
};



static int __init gpio_drv_init(void)
{
    printk(KERN_INFO "Inside the %s function\n",__FUNCTION__);

    int ret,dev_ret;

    if ((ret = alloc_chrdev_region(&first, 0, 1, "gpio")) < 0)
    {
	return ret;
    }
    if (IS_ERR(cl = class_create(THIS_MODULE, "chardrv")))
    {
	unregister_chrdev_region(first, 1);
	return PTR_ERR(cl);
    }
    if (IS_ERR(dev_ret = device_create(cl, NULL, first, NULL, "gpio_readwrite")))
    {
	class_destroy(cl);
	unregister_chrdev_region(first, 1);
	return PTR_ERR(dev_ret);
    }

    cdev_init(&c_dev, &fops);
    if ((ret = cdev_add(&c_dev, first, 1)) < 0)
    {
	device_destroy(cl, first);
	class_destroy(cl);
	unregister_chrdev_region(first, 1);
	return ret;
    }


    return 0;
}

static void __exit gpio_drv_exit(void)
{
    printk(KERN_INFO "Inside the %s function\n",__FUNCTION__);

    /* unregister the character device driver */

    cdev_del(&c_dev);
    device_destroy( cl, first );
    class_destroy( cl );
    unregister_chrdev_region(first, "gpio_readwrite");
}

module_init(gpio_drv_init);
module_exit(gpio_drv_exit);

MODULE_LICENSE("GPL");
