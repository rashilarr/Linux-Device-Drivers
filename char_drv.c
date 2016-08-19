#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/fs.h>

int file_open (struct inode *pinode, struct file *pfile)
{
    printk(KERN_INFO "Inside the %s function\n",__FUNCTION__);
    return 0;
}

size_t file_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
    printk(KERN_INFO "Inside the %s function\n",__FUNCTION__);
    return 0;
}

ssize_t file_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
    printk(KERN_INFO "Inside the %s function\n",__FUNCTION__);
    return length;
}

int file_close (struct inode *pinode, struct file *pfile)
{
    printk(KERN_INFO "Inside the %s function\n",__FUNCTION__);
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



static int __init char_drv_init(void)
{
    printk(KERN_INFO "Inside the %s function\n",__FUNCTION__);

    /* Register with the kernel and indicate that we are registering character device driver */
    register_chrdev(240 /* Major Number */,
                    "char_drv" /* Name of the driver */,
                     &fops /* file operations */); 

    return 0;
}

static void __exit char_drv_exit(void)
{
    printk(KERN_INFO "Inside the %s function\n",__FUNCTION__);

    /* unregister the character device driver */
    unregister_chrdev(240, "char_drv");
}

module_init(char_drv_init);
module_exit(char_drv_exit);

MODULE_LICENSE("GPL");
