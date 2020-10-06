#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <math.h>
#include <unistd.h>
#include <sys/time.h>
#include <semaphore.h>
#define limit 16

pthread_t hilos[limit]; 
int counter = 0;
int thread_count=limit;
pthread_mutex_t barrier_mutex;
pthread_mutex_t mutex;
pthread_cond_t cond_var;
sem_t hugs[2];
sem_t count_sem;
sem_t barrier_sem;

void* Thread_work_BW(void* rank) {
	long my_rank = (long) rank;
	while(my_rank!=counter);
	counter++;
	printf("Paso el hilo %ld\n",my_rank );
	while (counter < thread_count);
}

void* Thread_work_M(void* rank) {
	long my_rank = (long) rank;
	pthread_mutex_lock(&barrier_mutex);
	counter++;
	pthread_mutex_unlock(&barrier_mutex);
	printf("Paso el hilo %ld\n",my_rank );
	while (counter < thread_count);
}

void* Thread_work_S(void* rank) {
	long my_rank = (long) rank;
	sem_wait(&count_sem);
	printf("Paso el hilo %ld\n",my_rank );
	if (counter == thread_count-1) {
		counter = 0;
		sem_post(&count_sem);
		for (int j = 0; j < thread_count-1; j++)
			sem_post(&barrier_sem);
	} else {
		counter++;
		sem_post(&count_sem);
		sem_wait(&barrier_sem);
	}
}

void* Thread_work_CV(void* rank) {
	long my_rank = (long) rank;
	pthread_mutex_lock(&mutex);
	printf("Paso el hilo %ld\n",my_rank );
	counter++;
	if (counter == thread_count) {
		counter = 0;
		pthread_cond_broadcast(&cond_var);
	} else {
		while (pthread_cond_wait(&cond_var, &mutex) != 0);
	}
	pthread_mutex_unlock(&mutex);
}


int main(int argc, char const *argv[]){
	int ret1 = sem_init(&count_sem, 1, limit); 
	int ret2 = sem_init(&barrier_sem, 0, limit); 
	int error;
	for (int i = 0; i < limit; ++i)
	 {
	 	error = pthread_create(&(hilos[i]), NULL, &Thread_work_CV, (void*)i);
	 	if (error!=0)
	 	{
	 		printf("pthread %d no puede ser creado: [%s]\n", i, strerror(error));
	 	}
	 } 
	
	pthread_join(hilos[0],NULL);
	for (int i = 0; i < limit-1; ++i)
	{
		pthread_join(hilos[i], NULL);
	}
	return 0;
}