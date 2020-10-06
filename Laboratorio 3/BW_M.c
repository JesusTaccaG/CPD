#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <math.h>
#include <unistd.h>
#include <sys/time.h>
#define temp 1
pthread_t hilos[temp]; 
pthread_mutex_t mutex;
int flag=0;
float tiempo=0;
long long sum=0;
long thread_count=temp;
long n=pow(10,8);

void* Thread_sum(void* rank){
	//printf("%ld\n",pthread_self() );
	long my_rank = (long) rank;
	double factor;
	long long i;
	long long my_n = n/thread_count;
	long long my_first_i = my_n*my_rank;
	long long my_last_i = my_first_i + my_n;
	double my_sum = 0.0;

	if (my_first_i % 2 == 0)
		factor = 1.0;
	else
		factor = -1.0;
	printf("%lld, %lld\n",my_first_i,my_last_i );
	clock_t start, finish;
	start= clock();

	for (i = my_first_i; i < my_last_i; i++, factor = -factor){
		my_sum += factor/(2*i+1);
	}
	pthread_mutex_lock(&mutex);
	sum += my_sum;
	pthread_mutex_unlock(&mutex);

	finish= clock();
	float current=(float)(finish-start)/CLOCKS_PER_SEC;
	tiempo+=current;

	//printf("Con %d hilos: %lf segundos\n",temp , current);

	return NULL;
}


static void *Thread_sum2(void *rank)
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
	double my_sum = 0.0;

	clock_t start, finish;
	start= clock();
	
	for (i = my_first_i; i < my_last_i; i++, factor = -factor){
		my_sum += factor/(2*i+1);
	}
	while (hilos[flag] != pthread_self());
	sum+=my_sum;

	finish= clock();
	float current=(float)(finish-start)/CLOCKS_PER_SEC;
	tiempo+=current;
	
	flag++;
	return NULL;
}


int main(){
	int error;
	for (int i = 0; i < temp; ++i)
	 {
	 	error = pthread_create(&(hilos[i]), NULL, &Thread_sum2, (void*)i);
	 	if (error!=0)
	 	{
	 		printf("pthread %d no puede ser creado: [%s]\n", i, strerror(error));
	 	}
	 } 
	
	for (int i = 0; i < temp; ++i)
	{
		pthread_join(hilos[i], NULL);
	}
	printf("Con %d hilos: %lf segundos\n",temp , tiempo/temp);
	return 0;
}