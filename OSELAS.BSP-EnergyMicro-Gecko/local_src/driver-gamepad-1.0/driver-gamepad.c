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
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>

#include "driver-gamepad.h"


//int majorDevNum = 17;
dev_t deviceNum;
unsigned int deviceCount = 1;
struct cdev *buttons_cdev;
struct class *cl;

char buttons[] = {0, 0, 0, 0, 0, 0, 0, 0};

struct file_operations fops = {
	.owner = THIS_MODULE,
	.llseek = gamepad_llseek,
	.read = gamepad_read,
	.open = gamepad_open,
	//.mmap = gamepad_mmap,
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
	
	//deviceNum = MKDEV(majorDevNum, 0);
	int err = alloc_chrdev_region(&deviceNum, 0, deviceCount, "GPIO_buttons");
	if (err < 0) printk(KERN_ERR "Failed to allocate chrdev region.\n");
	//int err = register_chrdev_region(deviceNum, deviceCount, "GPIO_buttons");
	cl = class_create(THIS_MODULE, "GPIO_buttons");
	device_create(cl, NULL, deviceNum, NULL, "GPIO_buttons");
	
	struct resource *portC_resource = request_region(GPIO_PC_BASE, 0x24, "GPIO Port C");
	if (portC_resource == 0) printk(KERN_ERR "GPIO Port C memory region request failed\n");
	struct resource *GPIO_resource = request_region(GPIO_PA_BASE + 0x100, 0x20, "GPIO");
	if (GPIO_resource == 0) printk(KERN_ERR "GPIO memory region request failed\n");
	
	*GPIO_PC_MODEL = 0x33333333;
	*GPIO_PC_DOUT = 0xff;
	
	*GPIO_EXTIPSELL = 0x22222222;
	*GPIO_EXTIRISE = 0xff;
	*GPIO_EXTIFALL = 0xff;
	*GPIO_IEN = 0xff;
	
	err = request_irq(17, interrupt_handler, 0, "GPIO_buttons", NULL);
	if (err < 0) printk(KERN_ERR "IRQ request 1 failed.\n");
	err = request_irq(18, interrupt_handler, 0, "GPIO_buttons", NULL);
	if (err < 0) printk(KERN_ERR "IRQ request 2 failed.\n");
	
	buttons_cdev = cdev_alloc();
	//cdev_init(buttons_cdev, &fops);
	buttons_cdev->owner = THIS_MODULE;
	buttons_cdev->ops = &fops;
	err = cdev_add(buttons_cdev, deviceNum, deviceCount);
	if (err < 0)  {
		printk(KERN_ERR "Failed to add char device.\n");
	}
	
	return 0;
}

/*
 * Function to cleanup this module from kernel space
 */
static void __exit gamepad_cleanup(void)
{
	printk("Short life for a small module...\n");
	
	cdev_del(buttons_cdev);
	free_irq(1, NULL);
	free_irq(0, NULL);
	
	*GPIO_PC_MODEL = 0x0;
	*GPIO_PC_DOUT = 0x0;
	*GPIO_EXTIPSELL = 0x0;
	*GPIO_EXTIRISE = 0x0;
	*GPIO_EXTIFALL = 0x0;
	*GPIO_IEN = 0x0;
	
	release_region(GPIO_PC_BASE, 0x24);
	release_region(GPIO_PA_BASE + 0x100, 0x20);
	//TODO device and class uncreate and stuff
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
	if (*offp > 7 || *offp < 0) return -EFAULT;
	if (*offp + count > 8) return -EFAULT;
	//unsigned long err = copy_to_user(buff, &buttons[*offp], count);
	unsigned long err = copy_to_user(buff, buttons, count);
	if (err) return -EFAULT;
	//*offp += count;
	return count;
}

loff_t gamepad_llseek(struct file *filp, loff_t off, int etellerannet) {
	
	return -ESPIPE;
}

/*
int gamepad_mmap(struct file *filp, struct vm_area_struct *vmarea) {
	
	return 0;
}
*/

irqreturn_t interrupt_handler(int irq, void *dev_id, struct pt_regs *regs) {
	*GPIO_IFC = 0xFF; //clear irq flag
	int i;
	for (i = 0; i < 8; i++) {
		buttons[i] = ~(*GPIO_PC_DIN | ~(1 << i)) != 0;
	}
	//printk(KERN_DEBUG "Button state is %x\n", ~(*GPIO_PC_DIN));
	
	return IRQ_HANDLED;
}





module_init(gamepad_init);
module_exit(gamepad_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

