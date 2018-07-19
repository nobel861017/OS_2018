#define _GNU_SOURCE //https://stackoverflow.com/questions/36794338/implicit-declaration-of-function-sched-setaffinity
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include <errno.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <time.h>

#define THREAD_NUM 2

pthread_barrier_t barrier;

void* thread_func(void *arg){
	int x = *(int *)arg, y = *((int *)arg+1),cpu = 0;
	if(y == 1){
		cpu_set_t set;
		CPU_ZERO(&set);
		CPU_SET(cpu, &set);
		int rt = sched_setaffinity(0, sizeof(set), &set);
		struct sched_param parameter;
		parameter.sched_priority = sched_get_priority_max(SCHED_FIFO);
		sched_setscheduler(0, SCHED_FIFO, &parameter);
	}
	pthread_barrier_wait(&barrier);
	for(int i = 0; i < 3; i++){
		printf("Thread %d is running\n",x);
		clock_t start = clock();
		while(1){
			if((clock() - start)/CLOCKS_PER_SEC >= 0.5) break;
		}
	}
	
	return 0;
}

/*void* arg_par(int thr_num, int sched_num){
	int tmp[2];
	tmp[0] = thr_num, tmp[1] = sched_num;
	return (void*) tmp;
}*/

int main(int argc, char *argv[]){
	
	int default_sched = 0; // 0:default, 1:FIFO
	if(argc > 1) if(strcmp(argv[1],"SCHED_FIFO") == 0) default_sched = 1;
	
	pthread_t thread_id[THREAD_NUM];
	pthread_barrier_init(&barrier, NULL, THREAD_NUM);
	
	for(int i = 0; i < THREAD_NUM; i++){
		printf("Thread %d was created\n", i + 1);
		//pthread_create(&thread_id[i],NULL,thread_func,arg_par(i+1,default_sched));
		/*int tmp[2];
		tmp[0] = i+1, tmp[1] = default_sched;*/
		int *arg = (int *)malloc(2*sizeof(int));
		*arg = i + 1;
		*(arg+1) = default_sched;
		int rt = pthread_create(&thread_id[i],NULL,thread_func,(void*) arg);
		/*if(rt != 0) printf("Thread create err!\n");
		else printf("Thread create success!\n");
		*/
	}
	for (int i = 0; i < THREAD_NUM; i++) pthread_join(thread_id[i], NULL);
	return 0;
}