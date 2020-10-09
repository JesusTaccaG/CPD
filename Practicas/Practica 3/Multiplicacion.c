#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <math.h>
#include <unistd.h>
#include "timer.h"
#define limit 1
#define m 8000000
#define n 8
pthread_t HILOS[limit];
int x[n];
int y[m];
int A[m][n];
long long thread_count=limit;


void *Pth_mat_vect(void* rank) {
	long my_rank = (long) rank;
	int i, j;
	int local_m = m/thread_count;
	int my_first_row = my_rank*local_m;
	int my_last_row = (my_rank+1)*local_m - 1;

	for (i = my_first_row; i <= my_last_row; i++) {
		y[i] = 0;
		for (j = 0; j < n; j++)
			y[i] += A[i][j]*x[j];
	}
	return NULL;
}

int main(int argc, char** argv){
    int error;
    double start, finish;
    GET_TIME(start);
    for (int i = 0; i < limit; ++i){
        error = pthread_create(&(HILOS[i]), NULL, &Pth_mat_vect, (void*)i);
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