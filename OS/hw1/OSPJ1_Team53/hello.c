#include <linux/kernel.h>
#include <linux/linkage.h>

asmlinkage int sys_hello(void){
        printk("HELLO SYSTEM CALL B05902121 B05902019\n");
        return 0;
}

