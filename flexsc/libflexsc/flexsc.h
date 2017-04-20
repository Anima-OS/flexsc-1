#define _GNU_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <stdarg.h>
#include <sched.h>
#include "flexsc_cpu.h"

#define SYSCALL_FLEXSC_REGISTER 400
#define SYSCALL_FLEXSC_WAIT 401
#define SYSCALL_FLEXSC_HOOK 402

#define FLEXSC_STATUS_FREE 0 
#define FLEXSC_STATUS_SUBMITTED 1
#define FLEXSC_STATUS_DONE 2
#define FLEXSC_STATUS_BUSY 3

#define FLEXSC_ERR_ALLOC 600

struct flexsc_reg_info {
    unsigned long max_threads;
    unsigned long stack_base;
    unsigned long stack_size;
};

struct flexsc_cpuinfo {
    int user_cpu;
    int kernel_cpu;
};

void init_cpuinfo_default(struct flexsc_cpuinfo *cpuinfo);

struct flexsc_sysentry {
    long args[6];
    unsigned nargs;
    unsigned short rstatus;
    unsigned short sysnum;
    unsigned sysret;
} ____cacheline_aligned_in_smp;

struct flexsc_init_info {
    struct flexsc_sysentry *sysentry; /* Pointer to first sysentry */
    unsigned npages; /* Number of Syspages */
    struct flexsc_cpuinfo cpuinfo; 
};

struct flexsc_cb {
    void (*callback) (struct flexsc_cb *);
    void *args;
    int64_t ret;
};

struct flexsc_sysentry *flexsc_register(struct flexsc_init_info *info);
void flexsc_wait(void);
int init_info(struct flexsc_init_info *);

/* Find free sysentry and returns it */
struct flexsc_sysentry *free_syscall_entry(void);

void flexsc_hook(void);

pid_t gettid(void);

inline static void __flexsc_register(struct flexsc_reg_info *info) 
{
    syscall(400, info);
}
