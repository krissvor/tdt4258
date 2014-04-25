/*
 * This is a gamepad driver Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>

#include <linux/mm.h>

#include "driver-gamepad.h"


dev_t deviceNum;
unsigned int deviceCount = 1;
struct cdev *buttons_cdev;
struct class *cl;
void __iomem *gpio_mem;
void __iomem *portc_mem;

char buttons[] = {0, 0, 0, 0, 0, 0, 0, 0};

/* Possible file operations for this driver */
struct file_operations fops = {
	.owner = THIS_MODULE,
	.llseek = gamepad_llseek,
	.read = gamepad_read,
	.mmap = gamepad_mmap,
	.open = gamepad_open,
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
	
	/* Allocate character device and create dev node */
	int err = alloc_chrdev_region(&deviceNum, 0, deviceCount, "GPIO_buttons");
	if (err < 0) printk(KERN_ERR "Failed to allocate chrdev region.\n");
	cl = class_create(THIS_MODULE, "GPIO_buttons");
	device_create(cl, NULL, deviceNum, NULL, "GPIO_buttons");
	
	/* Request usage of GPIO memory region */
	struct resource *portC_resource = request_mem_region(GPIO_PC_BASE, 0x24, "GPIO Port C");
	if (portC_resource == 0) printk(KERN_ERR "GPIO Port C memory region request failed\n");
	struct resource *GPIO_resource = request_mem_region(GPIO_PA_BASE + 0x100, 0x20, "GPIO");
	if (GPIO_resource == 0) printk(KERN_ERR "GPIO memory region request failed\n");
	
	/* Remap GPIO memory to virtual address space. In our case this basically does nothing */
	gpio_mem = ioremap_nocache(GPIO_PA_BASE+0x100, 0x20);
	printk(KERN_DEBUG "gpio_mem addr: %p\n", gpio_mem);
	if (gpio_mem == NULL) printk(KERN_ERR "GPIO remap failed\n");
	portc_mem = ioremap_nocache(GPIO_PC_BASE, 0x24);
	printk(KERN_DEBUG "portc_mem addr: %p\n", portc_mem);
	if (portc_mem == NULL) printk(KERN_ERR "GPIO port c remap failed\n");

/*
	*GPIO_PC_MODEL = 0x33333333;
	*GPIO_PC_DOUT = 0xff;
	
	*GPIO_EXTIPSELL = 0x22222222;
	*GPIO_EXTIRISE = 0xff;
	*GPIO_EXTIFALL = 0xff;
	*GPIO_IEN = 0xff;
*/
	
	/* Configure GPIO input and interrupts */
	iowrite32(0x33333333, portc_mem+GPIO_PC_MODEL);
	iowrite32(0xff, portc_mem+GPIO_PC_DOUT);
	iowrite32(0x22222222, gpio_mem+GPIO_EXTIPSELL);
	iowrite32(0xff, gpio_mem+GPIO_EXTIRISE);
	iowrite32(0xff, gpio_mem+GPIO_EXTIFALL);
	iowrite32(0xff, gpio_mem+GPIO_IEN);
	
	/* Setup interrupt handler */
	err = request_irq(17, interrupt_handler, 0, "GPIO_buttons", NULL);
	if (err < 0) printk(KERN_ERR "IRQ request 1 failed.\n");
	err = request_irq(18, interrupt_handler, 0, "GPIO_buttons", NULL);
	if (err < 0) printk(KERN_ERR "IRQ request 2 failed.\n");
	
	/* Register file operations and activate the driver */
	buttons_cdev = cdev_alloc();
	buttons_cdev->owner = THIS_MODULE;
	buttons_cdev->ops = &fops;
	err = cdev_add(buttons_cdev, deviceNum, deviceCount);
	if (err < 0)  {
		printk(KERN_ERR "Failed to activate char device.\n");
	}
	
	return 0;
}

/*
 * Function to cleanup this module from kernel space
 */
static void __exit gamepad_cleanup(void)
{
	printk("Short life for a small module...\n");
	
	/* Deactivate driver */
	cdev_del(buttons_cdev);
	/* Free interrupts */
	free_irq(17, NULL);
	free_irq(18, NULL);
/*
	*GPIO_IEN = 0x0;
	*GPIO_EXTIRISE = 0x0;
	*GPIO_EXTIFALL = 0x0;
*/
	
	/* Disable interrupts from GPIO */
	iowrite32(0x0, gpio_mem+GPIO_IEN);
	iowrite32(0x0, gpio_mem+GPIO_EXTIRISE);
	iowrite32(0x0, gpio_mem+GPIO_EXTIFALL);
	
	printk(KERN_DEBUG "Unmap IO\n");
	iounmap(gpio_mem);
	iounmap(portc_mem);
	
	printk(KERN_DEBUG "Release region\n");
	release_mem_region(GPIO_PC_BASE, 0x24);
	release_mem_region(GPIO_PA_BASE + 0x100, 0x20);
	
	printk(KERN_DEBUG "Destroy device and class\n");
	device_destroy(cl, deviceNum);
	class_destroy(cl);
	
	printk(KERN_DEBUG "Unregister char dev\n");
	unregister_chrdev_region(deviceNum, deviceCount);
}

/* Open driver from userspace. Not much to do here. All relevant configuration is done on module loading */
int gamepad_open(struct inode *inode, struct file *filp) {
	//struct cdev *cdev = inode->i_cdev;
	
	return 0;
}

/* Close driver from userspace */
int gamepad_release(struct inode *inode, struct file *filp) {
	
	return 0;
}

/* Read data from driver */
ssize_t gamepad_read(struct file *filp, char __user *buff, size_t count, loff_t *offp) {
	if (*offp > 7 || *offp < 0) return -EFAULT;
	if (*offp + count > 8) return -EFAULT;
	//unsigned long err = copy_to_user(buff, &buttons[*offp], count);
	unsigned long err = copy_to_user(buff, buttons, count);
	if (err) return -EFAULT;
	//*offp += count;
	return count;
}

/* Seeking the driver data. Does nothing since seeking in 8 bytes is not very useful */
loff_t gamepad_llseek(struct file *filp, loff_t off, int etellerannet) {
	
	return -ESPIPE; // Just return proper error code. Seek is not possible
}

/* Memory map buttons array to userspace program */
int gamepad_mmap(struct file *filp, struct vm_area_struct *vmarea) {
	printk(KERN_DEBUG "Page shift: %i\n", PAGE_SHIFT);
	if (remap_pfn_range(vmarea, vmarea->vm_start, (long)buttons >> PAGE_SHIFT,
						vmarea->vm_end - vmarea->vm_start, vmarea->vm_page_prot))
					return -EAGAIN;
	
	return 0;
}

/* Handles interrupts from GPIO */
irqreturn_t interrupt_handler(int irq, void *dev_id, struct pt_regs *regs) {
	//*GPIO_IFC = 0xFF; //clear irq flag
	iowrite32(0xff, gpio_mem+GPIO_IFC);
	int data = ioread32(portc_mem+GPIO_PC_DIN);
	int i;
	for (i = 0; i < 8; i++) {
		buttons[i] = ~(data | ~(1 << i)) != 0;
	}
	//printk(KERN_DEBUG "Button state is %x\n", ~(data));
	
	return IRQ_HANDLED;
}





module_init(gamepad_init);
module_exit(gamepad_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

