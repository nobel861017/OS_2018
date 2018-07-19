#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>

int main(){
	long a, b;
	printf("Insert a and b:\n");
	scanf("%ld %ld",&a,&b);
	syscall(337);
	printf("multiply: %ld x %ld = %ld\n",a,b,syscall(338,a,b));
	printf("min: %ld\n",syscall(339,a,b));
	return 0;
}
