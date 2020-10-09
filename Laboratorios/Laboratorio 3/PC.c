#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <math.h>
#include <unistd.h>
#include <sys/time.h>
#include <semaphore.h>
#define limit 8

pthread_t hilos[limit]; 
long long thread_count=limit;
sem_t semaphores[limit];
char* messages[limit];
int MSG_MAX=limit;

void* Send_msg(void* rank) {
	long my_rank = (long) rank;
	long dest = (my_rank + 1) % thread_count;
	char* my_msg = malloc(MSG_MAX*sizeof(char));
	sprintf(my_msg, "Hello to %ld from %ld\n", dest, my_rank);
	messages[dest] = my_msg;
	sem_post(&semaphores[dest]);

	sem_wait(&semaphores[my_rank]);
	printf("Thread %ld > %s\n", my_rank, messages[my_rank]);
	return NULL;
}

int main(){
	int error;
	for (int i = 0; i < limit; ++i)
	 {
	 	error = pthread_create(&(hilos[i]), NULL, &Send_msg, (void*)i);
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