#include <linux/init.h>
#include <linux/module.h>
#include <linux/blkdev.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("EXAMPLE FOR TEST");

#define BLOCK_MAJOR 240
#define BLOCK_NAME "EXAMPLE"


static int h_init(void){
  int status = register_blkdev(BLOCK_MAJOR,BLOCK_NAME);
  if(status < 0) { 
    printk("Unable to register block device!!\n");
    return -EBUSY;
  }
  printk("Example OK!!\n");
  return 0;
}

static void h_exit(void){
  printk("Exit\n");
  unregister_blkdev(BLOCK_MAJOR,BLOCK_NAME);
}

module_init(h_init);
module_exit(h_exit);
