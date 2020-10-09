
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <math.h>
#include <unistd.h>
#include "timer.h"
#include <semaphore.h>
#define limit 4
const int MAX=1000;
pthread_t HILOS[limit];
long long thread_count=limit;
sem_t* sems;

void* Tokenize(void* rank) {
	long my_rank = (long) rank;
	int count;
	int next = (my_rank + 1) % thread_count;
	char *fg_rv;
	char my_line[MAX];
	char *my_string;
	sem_wait(&sems[my_rank]);
	fg_rv = fgets(my_line, MAX, stdin);
	sem_post(&sems[next]);
	while (fg_rv != NULL) {
		printf("Thread %ld > my_line = %s", my_rank, my_line);
		count = 0;
		my_string = strtok(my_line, " \t\n");
		while ( my_string != NULL ) {
			count++;
			printf("Thread %ld > string %d = %s\n", my_rank, count, my_string);
			my_string = strtok(NULL, " \t\n");
		}
		sem_wait(&sems[my_rank]);
		fg_rv = fgets(my_line, MAX, stdin);
		sem_post(&sems[next]);
	}
	return NULL;
}

int main(int argc, char** argv){
	sems = malloc(thread_count*sizeof(sem_t));
	for (int i = 0; i < limit; ++i){
		sem_init(&sems[i], 1, limit); 
	}
		
    int error;
    double start, finish;
    GET_TIME(start);
    for (int i = 0; i < limit; ++i){
        error = pthread_create(&(HILOS[i]), NULL, &Tokenize, (void*)i);
        if (error!=0){
            printf("pthread %d no puede ser creado: [%s]\n", i, strerror(error));
        }
    } 
    for (int i = 0; i < limit; ++i){
        pthread_join(HILOS[i], NULL);
    }
    GET_TIME(finish);
    printf("Tiempo de demora = %lf segundos\n", finish - start);
    return 0;
}