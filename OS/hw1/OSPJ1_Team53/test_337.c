#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>

int main(){
	syscall(337);
	return 0;
}
