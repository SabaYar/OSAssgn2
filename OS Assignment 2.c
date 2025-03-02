#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5 // Number of the philosophers
typedef enum { THINKING, HUNGRY, EATING } state_t; // State of the philosopher
state_t state[NUM_PHILOSOPHERS];

pthread_mutex_t mutex; // Global lock to control access
pthread_cond_t condition[NUM_PHILOSOPHERS]; // condition variable for the philosopher

// Defining left and right forks
int left(int i) { return (i + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS; }
int right(int i) { return (i + 1) % NUM_PHILOSOPHERS; }


void *philosopher_thread(){

}

void pickup_forks(int p_num){
    pthread_mutex_lock(&mutex);
    state[p_num] = HUNGRY;
    test(p_num);
    if (state[p_num] != EATING) {
        pthread_cond_wait(&condition[p_num], &mutex); // Wait if forks are not available
    }
    pthread_mutex_unlock(&mutex);
}

void return_forks(){

}

// Checks if the philosopher can eat or not
void test(int p_num){
    // The if condition checks if the philosopher is HUNGRY and checks if both left and right neighbors are not eating
    // If that is true only then it allows the philosophers to eat 
    if (state[p_num] == HUNGRY && state[left(p_num)] != EATING && state[right(p_num)] != EATING) {
        state[p_num] = EATING;
        printf("The philosopher %d picks up forks and starts eating.\n", p_num);

        pthread_cond_signal(&condition[p_num]); // Notify waiting philosopher
    }
}


int main(){
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    // Initialize mutex and condition variables
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_cond_init(&condition[i], NULL);
        state[i] = THINKING;
        ids[i] = i;
    }

    // Cleaning up the condition variables and mutex locks
    pthread_mutex_destroy(&mutex);
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_cond_destroy(&condition[i]);
    }
    return 0;
}