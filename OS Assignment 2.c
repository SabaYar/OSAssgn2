#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5 // Number of the philosophers
typedef enum { THINKING, HUNGRY, EATING } state_t; // State of the philosopher
state_t state[NUM_PHILOSOPHERS];

pthread_mutex_t mutex; // Global lock to control access
pthread_cond_t condition[NUM_PHILOSOPHERS]; // condition variable for the philosopher

void *philosopher_thread(){

}

void pickup_forks(){

}

void return_forks(){

}

// Checks if the philosopher can eat or not
void test(){

}


int main(){
    return 0;
}