#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>

asmlinkage unsigned int sys_my_get_time(unsigned int *t_sec, unsigned int *t_nsec)
{
	struct timespec now;

	getnstimeofday(&now);
	(*t_sec) = (unsigned int)now.tv_sec;
	(*t_nsec) = (unsigned int)now.tv_nsec;

	return 0;
}
