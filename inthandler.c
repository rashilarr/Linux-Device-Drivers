#include <linux/init.h>      // initialization macros
#include <linux/module.h>    // dynamic loading of modules into the kernel
#include <linux/kernel.h>    // kernel stuff
#include <linux/gpio.h>      // GPIO functions/macros
#include <linux/interrupt.h> // interrupt functions/macros

#define GPIO 54
#define GPIO_INT_NAME  "gpio_int"

#define GPIO_HIGH gpio_get_value(GPIO)
#define GPIO_LOW (gpio_get_value(GPIO) == 0)
short int irq_any_gpio    = 0;
int count =0;

static unsigned int yourGPIO = 49; // This is Pin 23 on the P9 header
static unsigned int irqNumber;

static irq_handler_t irqHandler(unsigned int irq, void *dev_id, struct pt_regs *regs);
// This is the IRQ Handler prototype


enum { falling, rising } type; 
static irqreturn_t r_irq_handler(int irq, void *dev_id)
 {
      count++;
    printk(KERN_DEBUG "interrupt received (irq: %d)\n", irq);
        if (irq == gpio_to_irq(GPIO)) 
    {

        type = GPIO_LOW ? falling : rising;

        if(type == falling)
        {
            printk("gpio pin is low\n");    
        }
        else
            printk("gpio pin is high\n");

    }

    return IRQ_HANDLED;
}


void r_int_config(void) {

   if (gpio_request(GPIO, GPIO_INT_NAME )) 
   {
      printk("GPIO request failure: %s\n", GPIO_INT_NAME );
      return;
   }

   if ( (irq_any_gpio = gpio_to_irq(GPIO)) < 0 ) {
      printk("GPIO to IRQ mapping failure %s\n",GPIO_INT_NAME );
      return;
   }

   printk(KERN_NOTICE "Mapped int %d\n", irq_any_gpio);

   if (request_irq(irq_any_gpio,(irq_handler_t ) r_irq_handler, IRQF_TRIGGER_HIGH, GPIO_INT_NAME, NULL)) 
   {
      printk("Irq Request failure\n");
      return;
   }

   return;
}

void __exit r_int_release(void) {

   free_irq(gpio_to_irq(GPIO), NULL);
    gpio_free(GPIO);;
   return;
}

satic int __init init_module(void)
{
    int result = 0;

    gpio_request(yourGPIO, "fancy label");    // Request a GPIO pin from the driver
    // 'yourGPIO' is expected to be an unsigned int, i.e. the GPIO number
    gpio_direction_input(yourGPIO);           // Set GPIO as input
    gpio_set_debounce(yourGPIO, 50);          // Set a 50ms debounce, adjust to your needs
    gpio_export(yourGPIO);                    // The GPIO will appear in /sys/class/gpio
    
        printk("<1>Hello World\n"); 
    r_int_config();
    irqNumber = gpio_to_irq(yourGPIO);        // map your GPIO to an IRQ
    result = request_irq(irqNumber,           // requested interrupt
                       (irq_handler_t) irqHandler, // pointer to handler function
                       IRQF_TRIGGER_RISING, // interrupt mode flag
                       "irqHandler",        // used in /proc/interrupts
                       NULL);               // the *dev_id shared interrupt lines, NULL is okay

  return result;
        return 0;
}

module_init(init_module);    // Do some better naming
module_exit(r_int_release);

MODULE_LICENSE("Your license")
