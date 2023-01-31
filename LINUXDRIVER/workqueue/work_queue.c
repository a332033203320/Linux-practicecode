#include "linux/kernel.h"
#include "linux/workqueue.h"
#include "linux/module.h"
#include "linux/delay.h"
#include "linux/init.h"

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("WORKQUEUE FOR TEST");
MODULE_AUTHOR("LEE");

struct workqueue_struct *work_q; 
struct work_struct work;

void debug(struct work_struct *work_struct) {
  printk("%s(%d):%s...\n", __FILE__, __LINE__, __func__);
}

static int w_init(void) {
  printk("WORKQUEUE INIT\n");
  work_q = alloc_workqueue("workqueue",0,0);
  INIT_WORK(&work, debug);
  queue_work(work_q, &work); //schedule_work(&work);
  return 0;
}

static void w_exit(void) {
  printk("WORKQUEUE EXIT\n");
  destroy_workqueue(work_q);
}

module_init(w_init);
module_exit(w_exit);

