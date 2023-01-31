#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>  
#include<linux/uaccess.h> /*copy_*_user*/
#include<linux/slab.h> /*kmalloc*/
#include"ioc.h"


MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("LEE");
MODULE_DESCRIPTION("EXAMPLE FOR CHARACTER DEVICE");

static int major, minor;
struct cdev *cdevp = NULL;


static int dev_open(struct inode* inode, struct file* fp) {
  printk("Open file: %s\n", __func__);
  return 0;
}

static int dev_release(struct inode* inode, struct file* fp) {
  printk("Release file: %s\n", __func__);
  return 0;
}

static int val = 0;
static long dev_ioctl(struct file* fp, unsigned int cmd, unsigned long args) {
  int err, ret, temp;
  if(_IOC_TYPE(cmd) != IOC_MAGIC || _IOC_NR(cmd) > IOC_MAXNR) { 
    return -EINVAL; 
  }
  if(_IOC_DIR(cmd) & _IOC_READ) { 
    err = !access_ok(VERIFY_WRITE,(void*) args,_IOC_SIZE(cmd)); 
  }
  else if(_IOC_DIR(cmd) & _IOC_WRITE) {
    err = !access_ok(VERIFY_READ,(void*) args,_IOC_SIZE(cmd)); 
  }
  if(err) { return -EFAULT; }

  switch(cmd) {
    case SETNUM:
      ret = __get_user(val, (int*)args);
      if(ret) { return ret; }
      printk("%s: Get val from user: %d", __func__, val);
      break;
      case GETNUM: 
      ret = __put_user(val, (int*)args);
      if(ret) { return ret; }
      printk("%s: Set val to user: %d", __func__, val);
      break; 
    case XNUM:
      temp = val;
      ret = __get_user(val, (int*)args);
      if(ret) { return ret; }
      ret = __put_user(temp, (int*)args);
      if(ret) { return ret; }
      printk("%s: Exchange val to kernel: %d, to user: %d\n", __func__, val,temp);
  };
  return 0;
}


static const char str[] = "Welcome to my device";
static const ssize_t size = sizeof(str);
static ssize_t dev_read(struct file* fp, char* buf, size_t count, loff_t* fps) {
  if(*fps >= size) { return 0; }
  printk("Read file: %s\n", __func__);
  if(*fps+count > size) { count = size-*fps; }
  if(copy_to_user(buf,str,count)) { return -EFAULT; }
  *fps += count;
  return count;
}

static char wbuff[16];
static const ssize_t wsize = sizeof(wbuff);
static ssize_t dev_write(struct file* fp, const char* buf, size_t count, loff_t* fps) {
  printk("Write file: %s %lld\n", __func__, *fps);
  //if(*fps >= wsize) { return 0; }
  //if(*fps+count > wsize) { count = wsize-*fps; }
  if(copy_from_user(wbuff,buf,count)) { return -EFAULT; }
  wbuff[(count&7)] = '\0';
  printk("Write data is: %s\n", wbuff);
  *fps += count;
  return count;
}


struct file_operations fops = {
  .owner = THIS_MODULE,
  .open = dev_open,
  .release = dev_release,
  .unlocked_ioctl = dev_ioctl,
  .read = dev_read,
  .write = dev_write,
};

static int c_init(void) {
  dev_t dev; //MKDEV(major,minor);
  int ret = alloc_chrdev_region(&dev,0,1,"MyDev");
  if(ret < 0) { printk("Registration error\n"); return ret; }
  major = MAJOR(dev); minor = MINOR(dev);
  printk("major: %d, minor %d\n", major, minor);
  cdevp = kmalloc(sizeof(struct cdev), GFP_KERNEL);
  if(cdevp == NULL) { printk("Memory allocation error\n"); return -ENOMEM; }  
  cdev_init(cdevp,&fops);
  ret = cdev_add(cdevp,dev,1);
  if(ret < 0) { 
   printk("Cdev addition error\n"); 
   goto failure;
  }
  return 0;
failure:
  if(cdevp) {
    kfree(cdevp);
    cdevp = NULL; 
  }
  return ret;
}

static void c_exit(void) {
  dev_t dev = MKDEV(major,minor);
  if(cdevp) { 
    cdev_del(cdevp);
    kfree(cdevp);
    cdevp = NULL;
  }
  unregister_chrdev_region(dev,1);
  printk("Cdev is unregister\n");
}

module_init(c_init);
module_exit(c_exit);
