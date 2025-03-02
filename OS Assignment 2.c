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

void *philosopher_thread(){
	while (1) {
		/* sleeps between 1 to 3 seconds */
        sleep(rand() % 3 + 1);

        pickup_forks();
		/* sleeps between 1 to 3 seconds */
        sleep(rand() % 3 + 1);

        return_forks();
		/* exits the thread */
		pthread_exit(0);
    }
}

// Function to pick up forks
void pickup_forks(int p_num) {
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