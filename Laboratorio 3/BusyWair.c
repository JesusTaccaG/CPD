#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int flag = 0;
int n = 10;
int thread_count = 2;

static void *Thread_sum(void *rank)
{
	long my_rank = (long) rank;
	double factor;
	long long i;
	long long my_n = n/thread_count;
	long long my_first_i = my_n*my_rank;
	long long my_last_i = my_first_i + my_n;
	if (my_first_i % 2 == 0)
		factor = 1.0;
	else
		factor = -1.0;
	long sum = 0;
	printf("asdasd\n");
	for (i = my_first_i; i < my_last_i; i++, factor = -factor){
		while (flag != my_rank);
		sum += factor/(2*i+1);
		printf("%ld\n", sum );
		flag = (flag+1) % thread_count;
	}
	printf("el valor de flag a cambiado a %d.\n", flag);
	return NULL;
}

static void *f2(void *p)
{
	for (int i = 0; i < 4; ++i){
    	int local_i = 0;
   	 	sleep(5);   /* sleep for 10 seconds */
    	flag=local_i;
   		printf("t2 ha cambiado el valor de flag a %d.\n", local_i);
	}
    return NULL;
}

int main()
{
    int rc;
    pthread_t t1, t2;

    rc = pthread_create(&t1, NULL, Thread_sum, NULL);
    if (rc != 0) {
        fprintf(stderr, "pthread Thread_sum fallo\n");
        return EXIT_FAILURE;
    }

    rc = pthread_create(&t2, NULL, f2, NULL);
    if (rc != 0) {
        fprintf(stderr, "pthread f2 fallo\n");
        return EXIT_FAILURE;
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    puts("Todos lo pthreads terminados.");
    return 0;
}