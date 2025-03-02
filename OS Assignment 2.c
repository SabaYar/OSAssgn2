#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5 // Number of the philosophers
typedef enum { THINKING, EATING } state_t; // State of the philosopher
state_t state[NUM_PHILOSOPHERS];

pthread_mutex_t mutex; // Global lock to control access
pthread_cond_t condition[NUM_PHILOSOPHERS]; // condition variable for the philosopher

void *philosopher_thread();  /* threads call this function */
void pickup_forks();
void return_forks();

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

void pickup_forks(){

}

void return_forks(){

}

// Checks if the philosopher can eat or not
void test(){

}


int main(int argc, char *argv[]){
	pthread_t tid; /* the thread identifier */
	pthread_attr_t attr; /* set of thread attributes */
	/* set the default attributes of the thread */
	pthread_attr_init(&attr);
	/* create the thread */
	pthread_create(&tid, &attr, philosopher_thread, argv[1]);
	/* wait for the thread to exit */
	pthread_join(tid,NULL);
    return 0;
}