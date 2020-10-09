#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <math.h>
#include <unistd.h>
#include "timer.h"
#define OP 100000
#define limit 8
struct list_node_s {
    int data;
    struct list_node_s* next;
    pthread_mutex_t mutex;
};

struct list_node_s* start;
pthread_t HILOS[limit];
pthread_mutex_t head_p_mutex;
pthread_mutex_t list_mutex;
pthread_rwlock_t rwlock;

int Member1(int value, struct list_node_s** head_p) {
    struct list_node_s* curr_p = *head_p;
    while (curr_p != NULL && curr_p->data < value)
        curr_p = curr_p->next;

    if (curr_p == NULL || curr_p->data > value) {
        return 0;
    } else {
        return 1;
    }
}

int Member2(int value, struct list_node_s** head_p) {
    struct list_node_s* temp_p;
    pthread_mutex_lock(&head_p_mutex);
    temp_p = *head_p;
    while (temp_p != NULL && temp_p->data < value) {
        if (temp_p->next != NULL)
            pthread_mutex_lock(&(temp_p->next->mutex));
        if (temp_p == *head_p)
            pthread_mutex_unlock(&head_p_mutex);
        pthread_mutex_unlock(&(temp_p->mutex));
        temp_p = temp_p->next;
    }
    if (temp_p == NULL || temp_p->data > value) {
        if (temp_p == *head_p)
            pthread_mutex_unlock(&head_p_mutex);
        if (temp_p != NULL)
            pthread_mutex_unlock(&(temp_p->mutex));
        return 0;
    } else {
        if (temp_p == *head_p)
            pthread_mutex_unlock(&head_p_mutex);
        pthread_mutex_unlock(&(temp_p->mutex));
        return 1;
    }
}



int Insert(int value, struct list_node_s** head_p) {
    struct list_node_s* curr_p = *head_p;
    struct list_node_s* pred_p = NULL;
    struct list_node_s* temp_p;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value) {
        temp_p = malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = curr_p;
        if (pred_p == NULL) 
            *head_p = temp_p;
        else
            pred_p->next = temp_p;
        return 1;
    } else {
        return 0;
    }
}

int Insert2(int value, struct list_node_s** head_p) {
    struct list_node_s* curr_p = *head_p;
    struct list_node_s* pred_p = NULL;
    struct list_node_s* temp_p;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value) {
        temp_p = malloc(sizeof(struct list_node_s));
        pthread_mutex_init(&(temp_p->mutex), NULL);
        temp_p->data = value;
        temp_p->next = curr_p;
        if (curr_p != NULL) 
            pthread_mutex_unlock(&(curr_p->mutex));
        if (pred_p == NULL) 
            *head_p = temp_p;
        else{
            pred_p->next = temp_p;
            pthread_mutex_unlock(&(pred_p->mutex));
        }
        return 1;
    } else {
        return 0;
    }
}

int Delete(int value, struct list_node_s** head_p) {
    struct list_node_s* curr_p = *head_p;
    struct list_node_s* pred_p = NULL;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data == value) {
        if (pred_p == NULL) { 
            *head_p = curr_p->next;
            free(curr_p);
        } else {
            pred_p->next = curr_p->next;
            free(curr_p);
        }
        return 1;
    } else { 
        return 0;
    }
}
int Delete2(int value, struct list_node_s** head_p) {
    struct list_node_s* curr_p = *head_p;
    struct list_node_s* pred_p = NULL;  

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data == value) {
        if (pred_p == NULL) { 
            *head_p = curr_p->next;
            pthread_mutex_unlock(&head_p_mutex);
            pthread_mutex_unlock(&(curr_p->mutex));
            pthread_mutex_destroy(&(curr_p->mutex));
            free(curr_p);
        } else {
            pred_p->next = curr_p->next;
            pthread_mutex_unlock(&(pred_p->mutex));
            pthread_mutex_destroy(&(curr_p->mutex));
            free(curr_p);
        }
        return 1;
    } else { 
        if (pred_p != NULL)
            pthread_mutex_unlock(&(pred_p->mutex));
        if (curr_p != NULL)
            pthread_mutex_unlock(&(curr_p->mutex));
        if (curr_p == *head_p)
            pthread_mutex_unlock(&head_p_mutex);
        return 0;
    }
}

void* RWL(void* rank) {
    long my_rank = (long) rank;
    int OPP = OP/limit;
    int ME = 99.9*OPP/100;
    int DE = 0.05*OPP/100;
    int IN = 0.05*OPP/100;
    for (int i = 0; i < ME; ++i){
        int numero = rand() % 1000;
        pthread_rwlock_wrlock(&rwlock);
        Member1(numero,&start);
        pthread_rwlock_unlock(&rwlock);
    }
    for (int i = 0; i < DE; ++i){
        pthread_rwlock_wrlock(&rwlock);
        Insert(i,&start);
        pthread_rwlock_unlock(&rwlock);
    }
    for (int i = 0; i < IN; ++i){
        pthread_rwlock_wrlock(&rwlock);
        Delete(i,&start);
        pthread_rwlock_unlock(&rwlock);
    }
    printf("Corrio el hilo %ld\n",my_rank);
    return NULL;
}

void* OMEL(void* rank) {
    long my_rank = (long) rank;
    int OPP = OP/limit;
    int ME = 99.9*OPP/100;
    int DE = 0.05*OPP/100;
    int IN = 0.05*OPP/100;
    for (int i = 0; i < ME; ++i){
        int numero = rand() % 1000;
        pthread_mutex_lock(&list_mutex);
        Member1(numero,&start);
        pthread_mutex_unlock(&list_mutex);
    }
    for (int i = 0; i < DE; ++i){
        pthread_mutex_lock(&list_mutex);
        Insert(i,&start);
        pthread_mutex_unlock(&list_mutex);
    }
    for (int i = 0; i < IN; ++i){
        pthread_mutex_lock(&list_mutex);
        Delete(i,&start);
        pthread_mutex_unlock(&list_mutex);
    }
    printf("Corrio el hilo %ld\n",my_rank);
    return NULL;
}

void* OMN(void* rank) {
    long my_rank = (long) rank;
    int OPP = OP/limit;
    int ME = 99.9*OPP/100;
    int DE = 0.05*OPP/100;
    int IN = 0.05*OPP/100;
    for (int i = 0; i < ME; ++i){
        int numero = rand() % 1000;
        Member2(numero,&start);
    }
    for (int i = 0; i < DE; ++i){
        Insert2(i,&start);
    }
    for (int i = 0; i < IN; ++i){
        Delete2(i,&start);
    }
    printf("Corrio el hilo %ld\n",my_rank);
    return NULL;
}

int main(){
    start =  (struct list_node_s *) malloc(sizeof(struct list_node_s));
    for (int i = 0; i < 1000; ++i){
        Insert(i,&start);
    }
    double inicio, finish;
    //Read-Write Locks
    //One Mutex for Entire List
    //One Mutex per Node
    int error;
    GET_TIME(inicio);
    for (int i = 0; i < limit; ++i){
        error = pthread_create(&(HILOS[i]), NULL, &OMEL, (void*)i);
        if (error!=0){
            printf("pthread %d no puede ser creado: [%s]\n", i, strerror(error));
        }
    } 
    for (int i = 0; i < limit; ++i){
        pthread_join(HILOS[i], NULL);
    }
    GET_TIME(finish);
    printf("Tiempo de demora = %lf segundos\n", finish - inicio);
    return 0;
}