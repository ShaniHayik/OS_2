#include <pthread.h> 
#include <semaphore.h> 
#include <stdio.h> 
#include <time.h>
#include <unistd.h>

#define N 5 
#define THINKING 2 
#define HUNGRY 1 
#define EATING 0 
#define LEFT (phnum + 4) % N 
#define RIGHT (phnum + 1) % N 
#define HUNGER_LIM 10.0



//Explanation - never livelock:
//we prevent from live lock as 
//we did in the starvation question



int state[N] = {2,2,2,2,2}; 
int phil[N] = { 0, 1, 2, 3, 4 }; 
clock_t last_time[N];

sem_t mutex; 
sem_t S[N]; 


double diff(clock_t time){
	return (clock() - time)/CLOCKS_PER_SEC;
}


void test(int phnum) 
{ 	
	if (state[phnum] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING && (diff(last_time[LEFT]) < HUNGER_LIM) && 		(diff(last_time[RIGHT]) < HUNGER_LIM)) { 
		// state that eating 
		state[phnum] = EATING; 

		printf("Philosopher %d takes fork %d and %d\n", 
					phnum + 1, LEFT + 1, phnum + 1); 

		printf("Philosopher %d is Eating\n", phnum + 1); 

		sleep(2); 

		last_time[phnum] = clock();

		sem_post(&S[phnum]); 
	} 
} 


// take up chopsticks 
void take_fork(int phnum) 
{ 

	sem_wait(&mutex); 

	// state that hungry 
	state[phnum] = HUNGRY; 

	printf("Philosopher %d is Hungry\n", phnum + 1); 

	// eat if neighbours are not eating 
	test(phnum); 

	sem_post(&mutex); 

	// if unable to eat wait to be signalled 
	sem_wait(&S[phnum]); 

	sleep(1); 
} 


// put down chopsticks 
void put_fork(int phnum) 
{ 

	sem_wait(&mutex); 

	// state that thinking 
	state[phnum] = THINKING; 

	printf("Philosopher %d putting fork %d and %d down\n", 
		phnum + 1, LEFT + 1, phnum + 1); 
	printf("Philosopher %d is thinking\n", phnum + 1); 

	test(RIGHT); 

	sem_post(&mutex); 
} 


void* philospher(void* num) 
{ 	

	int* i = num; 
	last_time[*i] = clock();

	while (1) { 

		sleep(1); 

		take_fork(*i); 

		sleep(0); 

		put_fork(*i); 
	} 
} 


int main() 
{ 
	int i; 
	pthread_t thread_id[N]; 

	sem_init(&mutex, 0, 1); 

	for (i = 0; i < N; i++) 

		sem_init(&S[i], 0, 0); 


	for (i = 0; i < N; i++) { 

		// create philosopher processes 
		pthread_create(&thread_id[i], NULL, 
					philospher, &phil[i]); 

		printf("Philosopher %d is thinking\n", i + 1); 
	} 

 
	for (i = 0; i < N; i++) 

		pthread_join(thread_id[i], NULL); 
} 
