#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_EVENTS 5

pthread_mutex_t *mutex = new PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t *condition = new PTHREAD_COND_INITIALIZER;

volatile int events[NUM_EVENTS] = { 0 };  

void *producerThread(void *arg) {
    for (int i = 0; i < NUM_EVENTS; ++i) {
        sleep(1);  // Задержка в одну секунду

        pthread_mutex_lock(mutex);
        events[i] = 1; 
        printf("Event %d Produced.\n", i + 1);
        pthread_cond_signal(condition);  
        sleep(1);
        pthread_mutex_unlock(mutex);
    }

    return nullptr;
}

void *consumerThread(void *arg) {
    for (int i = 0; i < NUM_EVENTS; ++i) {
        pthread_mutex_lock(mutex);
        while (!events[i]) {
            pthread_cond_wait(condition, mutex);
	        printf("Event %d Consumed.\n\n", i + 1);
        }
        pthread_mutex_unlock(mutex);
    }

    return nullptr;
}

int main() {
    pthread_t producer, consumer;

    pthread_create(&producer, NULL, producerThread, NULL);
    pthread_create(&consumer, NULL, consumerThread, NULL);

    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);

    return 0;
}

