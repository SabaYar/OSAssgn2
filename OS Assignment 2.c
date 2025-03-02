#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5 // Number of the philosophers
typedef enum { THINKING, HUNGRY, EATING } state_t; // State of the philosopher
state_t state[NUM_PHILOSOPHERS];

pthread_mutex_t mutex; // Global lock to control access
pthread_cond_t condition[NUM_PHILOSOPHERS]; // condition variable for the philosopher

int left(int i) { return (i + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS; }
int right(int i) { return (i + 1) % NUM_PHILOSOPHERS; }

void pickup_forks(int p_num);
void return_forks(int p_num);
void test(int p_num);

void *philosopher_thread(int p_num){
	while (1) {
		printf("The philosopher %d is thinking.\n", *id);
		/* sleeps between 1 to 3 seconds */
        sleep(rand() % 3 + 1);

        pickup_forks(p_num);
		/* sleeps between 1 to 3 seconds */
        sleep(rand() % 3 + 1);

        return_forks(p_num);
		/* exits the thread */
		pthread_exit(0);
    }
}

void pickup_forks(int p_num){
    pthread_mutex_lock(&mutex);
	
	/* reverts current philospher to thinking state*/
    state[p_num] = THINKING;
    test(p_num);
	
    if (state[p_num] != EATING) {
        pthread_cond_wait(&condition[p_num], &mutex); // Wait if forks are not available
    }
	
    pthread_mutex_unlock(&mutex);
}

void return_forks(int p_num){
	pthread_mutex_lock(&mutex);
    
	/* reverts current philospher to thinking state*/
    state[p_num] = THINKING;
    printf("The philosopher %d has stopped eating and returns forks.\n", p_num);
    
	/* */
    test(left(p_num));
    test(right(p_num));
    
    pthread_mutex_unlock(&mutex);
}

// Checks if the philosopher can eat or not
void test(int p_num){
    // The if condition checks if the philosopher is THINKING and checks if both left and right neighbors are not eating
    // If that is true only then it allows the philosophers to eat 
    if (state[p_num] == THINKING && state[left(p_num)] != EATING && state[right(p_num)] != EATING) {
        state[p_num] = EATING;
        printf("The philosopher %d picks up forks and starts eating.\n", p_num);

        pthread_cond_signal(&condition[p_num]); // Notify waiting philosopher
    }
}


int main() {
    // pthread_t tid; /* the thread identifier */
	// pthread_attr_t attr; /* set of thread attributes */
	// /* set the default attributes of the thread */
	// pthread_attr_init(&attr);
	// /* create the thread */
	// pthread_create(&tid, &attr, philosopher_thread, argv[1]);
	// /* wait for the thread to exit */
	// pthread_join(tid,NULL);
    int i;
    pthread_t philosophers[NUM_PHILOSOPHERS];

    // Initialize the mutex and condition variables
    pthread_mutex_init(&mutex, NULL);
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_cond_init(&condition[i], NULL);
        state[i] = THINKING;
    }

    // Create philosopher threads
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&philosophers[i], NULL, philosopher_thread(*id), id);
		printf("The philosopher %d is thinking.\n", *id);
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
