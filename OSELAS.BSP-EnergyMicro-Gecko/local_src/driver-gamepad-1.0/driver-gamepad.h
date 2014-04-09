
#define GPIO_PA_BASE 0x40006000
#define GPIO_PC_BASE 0x40006048

#define GPIO_PC_CTRL     ((unsigned int*)(GPIO_PC_BASE + 0x00))
#define GPIO_PC_MODEL    ((unsigned int*)(GPIO_PC_BASE + 0x04))
#define GPIO_PC_MODEH    ((unsigned int*)(GPIO_PC_BASE + 0x08))
#define GPIO_PC_DOUT     ((unsigned int*)(GPIO_PC_BASE + 0x0c))
#define GPIO_PC_DOUTSET  ((unsigned int*)(GPIO_PC_BASE + 0x10))
#define GPIO_PC_DOUTCLR  ((unsigned int*)(GPIO_PC_BASE + 0x14))
#define GPIO_PC_DOUTTGL  ((unsigned int*)(GPIO_PC_BASE + 0x18))
#define GPIO_PC_DIN      ((unsigned int*)(GPIO_PC_BASE + 0x1c))
#define GPIO_PC_PINLOCKN ((unsigned int*)(GPIO_PC_BASE + 0x20))

#define GPIO_EXTIPSELL ((unsigned int*)(GPIO_PA_BASE + 0x100))
#define GPIO_EXTIPSELH ((unsigned int*)(GPIO_PA_BASE + 0x104))
#define GPIO_EXTIRISE  ((unsigned int*)(GPIO_PA_BASE + 0x108))
#define GPIO_EXTIFALL  ((unsigned int*)(GPIO_PA_BASE + 0x10c))
#define GPIO_IEN       ((unsigned int*)(GPIO_PA_BASE + 0x110))
#define GPIO_IFC       ((unsigned int*)(GPIO_PA_BASE + 0x11c))


int gamepad_open(struct inode *inode, struct file *filp);
int gamepad_release(struct inode *inode, struct file *filp);
ssize_t gamepad_read(struct file *filp, char __user *buff, size_t count, loff_t *offp);
loff_t gamepad_llseek(struct file *filp, loff_t off, int etellerannet);
irqreturn_t interrupt_handler(int irq, void  *dev_id, struct pt_regs *regs);




