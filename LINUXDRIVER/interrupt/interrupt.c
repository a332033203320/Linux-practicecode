#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/interrupt.h>
#include<linux/workqueue.h>
#include<linux/delay.h>

#define dev_id 1226
#define KBD_DATA_REG        0x60    
#define KBD_STATUS_REG      0x64  
#define KBD_SCANCODE_MASK   0x7f  
#define KBD_STATUS_MASK     0x80 
  
static int irq;
static char *dev_name;
struct myirq { int devid; };
struct myirq mydev = { dev_id };
struct work_struct work;

module_param(irq, int, 0644);
module_param(dev_name, charp, 0644);

MODULE_AUTHOR("LEE");
MODULE_DESCRIPTION("IRQ FOR TEST");
MODULE_LICENSE("DUAL BSD/GPL");

static void work_handler(struct work_struct *works) {
  //printk("%s(%d): %s\n", __FILE__, __LINE__, __func__);
  //do something
  printk("ESC is pressed\n");
}


static irqreturn_t irq_handler(int irq, void* dev) {
  struct myirq temp = *(struct myirq*)dev;  
  if(temp.devid == dev_id) {
    unsigned char key_code = inb(KBD_DATA_REG);
    printk("keycode: %d\n ", (int)key_code);
    if(key_code == 0x01) {
      INIT_WORK(&work,work_handler);
      schedule_work(&work);
    } 
  }
  return IRQ_HANDLED;
}

static int i_init(void) {
  printk("IRQ INIT\n");
  //create irq
  if(request_irq(irq,irq_handler,IRQF_SHARED,dev_name,&mydev)) return -EFAULT;  
  return 0;
} 

static void i_exit(void) {
  printk("IRQ EXIT\n");
  //destroy irq
  free_irq(irq,&mydev);
}

module_init(i_init);
module_exit(i_exit);
