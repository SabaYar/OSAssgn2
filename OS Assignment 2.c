#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5 // Number of philosophers
typedef enum { THINKING, EATING } state_t; // States
state_t state[NUM_PHILOSOPHERS];

pthread_mutex_t mutex; // Global lock
pthread_cond_t condition[NUM_PHILOSOPHERS]; // Condition variables

void pickup_forks(int p_num);
void return_forks(int p_num);

// Get the left and right neighbors
int left(int i) { return (i + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS; }
int right(int i) { return (i + 1) % NUM_PHILOSOPHERS; }

void *philosopher_thread(void *arg) {
    int p_num = *(int *)arg;
    free(arg);

    while (1) {       
        sleep(rand() % 3 + 1); // Simulate thinking

        pickup_forks(p_num);

        printf("Philosopher %d is eating.\n", p_num);
        sleep(rand() % 3 + 1); // Simulate eating

        return_forks(p_num);
    }
}

// Function to pick up forks
void pickup_forks(int p_num) {
    pthread_mutex_lock(&mutex);

    while (state[left(p_num)] == EATING || state[right(p_num)] == EATING) {
        pthread_cond_wait(&condition[p_num], &mutex);
    }

    state[p_num] = EATING;
    printf("Philosopher %d picks up forks and starts eating.\n", p_num);

    pthread_mutex_unlock(&mutex);
}


void return_forks(int p_num) {
    pthread_mutex_lock(&mutex);

    state[p_num] = THINKING;
    printf("Philosopher %d has stopped eating and returns forks.\n", p_num);
    printf("Philosopher %d is thinking.\n", p_num);
    // Notifying neighbors that the current philosopher has stopped eating 
    pthread_cond_signal(&condition[left(p_num)]);
    pthread_cond_signal(&condition[right(p_num)]);

    pthread_mutex_unlock(&mutex);
}


int main() {
    int i;
    pthread_t philosophers[NUM_PHILOSOPHERS];

    // Initializing the mutex and condition variables
    pthread_mutex_init(&mutex, NULL);
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_cond_init(&condition[i], NULL);
        state[i] = THINKING;
    }

    // Creating the philosopher threads
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&philosophers[i], NULL, philosopher_thread, id);
        printf("Philosopher %d is thinking.\n", *id);
    }

    // Join threads
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy mutex and condition variables
    pthread_mutex_destroy(&mutex);
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_cond_destroy(&condition[i]);
    }

    return 0;
}