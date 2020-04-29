#include <linux/linkage.h>
#include <linux/kernel.h>

asmlinkage int sys_my_print(char *mes)
{
	printk("%s", mes);
	return 0;
}
