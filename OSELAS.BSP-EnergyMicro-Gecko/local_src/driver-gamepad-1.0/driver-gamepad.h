
#define GPIO_PA_BASE 0x40006000
#define GPIO_PC_BASE 0x40006048

#define GPIO_PC_CTRL     0x00
#define GPIO_PC_MODEL    0x04
#define GPIO_PC_MODEH    0x08
#define GPIO_PC_DOUT     0x0c
#define GPIO_PC_DOUTSET  0x10
#define GPIO_PC_DOUTCLR  0x14
#define GPIO_PC_DOUTTGL  0x18
#define GPIO_PC_DIN      0x1c
#define GPIO_PC_PINLOCKN 0x20

#define GPIO_EXTIPSELL 0x00
#define GPIO_EXTIPSELH 0x04
#define GPIO_EXTIRISE  0x08
#define GPIO_EXTIFALL  0x0c
#define GPIO_IEN       0x10
#define GPIO_IFC       0x1c


int gamepad_open(struct inode *inode, struct file *filp);
int gamepad_release(struct inode *inode, struct file *filp);
ssize_t gamepad_read(struct file *filp, char __user *buff, size_t count, loff_t *offp);
loff_t gamepad_llseek(struct file *filp, loff_t off, int);
int gamepad_mmap(struct file *filp, struct vm_area_struct *vmarea);
irqreturn_t interrupt_handler(int irq, void  *dev_id, struct pt_regs *regs);



