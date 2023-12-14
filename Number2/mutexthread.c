/* mutextrhead.c */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>

#define NUM_THREADS 3

pthread_mutex_t mutex;
int sum;

void *mutex_thread(void *arg) {
    intptr_t thread_arg = (intptr_t)arg; 
    pthread_mutex_lock(&mutex);
    sum += thread_arg;
    pthread_mutex_unlock(&mutex);

    return (void*)thread_arg;  
}

int main(int argc, char *argv[]) {
    pthread_t tid[NUM_THREADS];
    intptr_t arg[NUM_THREADS];  
    void *result;
    int status;

    if (argc < 4) {
        fprintf(stderr, "Usage: mutexthread number1 number2 number3\n");
        exit(1);
    }

    for (int i = 0; i < NUM_THREADS; i++)
        arg[i] = atoi(argv[i + 1]);

    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
        status = pthread_create(&tid[i], NULL, mutex_thread, (void *)arg[i]);
        if (status != 0) {
            fprintf(stderr, "Error creating thread %d: %d\n", i, status);
            exit(1);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        status = pthread_join(tid[i], &result);
        if (status != 0) {
            fprintf(stderr, "Error joining thread %d: %d\n", i, status);
            exit(1);
        }
    }

    status = pthread_mutex_destroy(&mutex);
    if (status != 0)
        perror("Destroy mutex");

    printf("Sum is %d\n", sum);

    pthread_exit(result);
}

