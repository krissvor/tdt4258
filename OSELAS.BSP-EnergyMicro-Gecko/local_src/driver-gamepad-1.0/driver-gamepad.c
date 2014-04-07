/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>


int majorDevNum = 17;
dev_t deviceNum;
unsigned int deviceCount = 1;
struct cdev *buttons_cdev;

int buttons[] = {0, 0, 0, 0, 0, 0, 0, 0};

struct file_operations fops = {
	.owner = THIS_MODULE,
	.llseek = gamepad_llseek,
	.read = gamepad_read,
	.open = gamepad_open,
	.mmap = gamepad_mmap,
	.release = gamepad_release,
};

/*
 * Function to insert this module into kernel space
 *
 * Returns 0 if successfull, otherwise -1
 */
static int __init gamepad_init(void)
{
	printk("Hello World, here is your module speaking\n");
	
	deviceNum = MKDEV(majorDevNum, 0);
	int err = register_chrdev_region(deviceNum, deviceCount, "GPIO_buttons");
	
	buttons_cdev = cdev_alloc();
	//cdev_init(buttons_cdev, &fops);
	buttons_cdev->owner = THIS_MODULE;
	buttons_cdev->ops = &fops;
	err = cdev_add(buttons_cdev, deviceNum, deviceCount);
	
	//request_irq();
	
	
	return 0;
}

/*
 * Function to cleanup this module from kernel space
 */
static void __exit gamepad_cleanup(void)
{
	 printk("Short life for a small module...\n");
	 
	 
	 cdev_del(buttons_cdev);
	 unregister_chrdev_region(deviceNum, deviceCount);
	 
	 
}

int gamepad_open(struct inode *inode, struct file *filp) {
	//struct cdev *cdev = inode->i_cdev;
	
	return 0;
}

int gamepad_release(struct inode *inode, struct file *filp) {
	
	return 0;
}

ssize_t gamepad_read(struct file *filp, char __user *buff, size_t count, loff_t *offp) {
	if (*offp > 7 && *offp < 0) return -1;
	if (*offp + count > 8) return -1;
	unsigned long err = copy_to_user(buff, &buttons[*offp], count*4);
	if (err) return -EFAULT;
	*offp += count;
	return count;
}

loff_t gamepad_llseek(struct file *filp, loff_t off, int etellerannet) {
	
	return 0;
}

int gamepad_mmap(struct file *filp, struct vm_area_struct *vmarea) {
	
	return 0;
}





module_init(gamepad_init);
module_exit(gamepad_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

