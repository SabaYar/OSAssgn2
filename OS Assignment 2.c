#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5 // Number of philosophers
typedef enum { THINKING, EATING } state_t; // Philosopher states
state_t state[NUM_PHILOSOPHERS]; // State of each philosopher

pthread_mutex_t mutex; // Global lock
pthread_cond_t condition[NUM_PHILOSOPHERS]; // Condition variables

void pickup_forks(int p_num);
void return_forks(int p_num);

// Get the left and right neighbors
int left(int i) { return (i + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS; }
int right(int i) { return (i + 1) % NUM_PHILOSOPHERS; }

// Function that each thread will loop
void *philosopher_thread(void *arg) {
	// Converting cmd line arg to int
    int p_num = *(int *)arg;
    free(arg);
	
    while (1) {
		// Wait for 1 to 3 seconds before attempting to pick up forks
        sleep(rand() % 3 + 1); 
        pickup_forks(p_num);
        // Wait for 1 to 3 seconds before attempting to return forks
        sleep(rand() % 3 + 1);
        return_forks(p_num);
    }
}

// Function to pick up forks
void pickup_forks(int p_num) {
	// Lock critical section
    pthread_mutex_lock(&mutex);
	// Wait until neighbours have stopped eating before picking up forks
    while (state[left(p_num)] == EATING || state[right(p_num)] == EATING) {
        pthread_cond_wait(&condition[p_num], &mutex);
    }
	// Change current philosopher state to EATING
    state[p_num] = EATING;
	// Notify others that current philosopher state has changed to EATING
    printf("Philosopher %d picks up forks.\n", p_num);
	printf("Philosopher %d is eating.\n", p_num);
	// Unlock critical section
    pthread_mutex_unlock(&mutex);
}

// Function to return forks
void return_forks(int p_num) {
	// Lock critical section
    pthread_mutex_lock(&mutex);
	// Change current philosopher state to THINKING
    state[p_num] = THINKING;
	// Notify others that current philosopher state has changed to THINKING
    printf("Philosopher %d has stopped eating and returns forks.\n", p_num);
    printf("Philosopher %d is thinking.\n", p_num);
    // Signal neighbors that the current philosopher has stopped eating 
    pthread_cond_signal(&condition[left(p_num)]);
    pthread_cond_signal(&condition[right(p_num)]);
	// Unlock critical section
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