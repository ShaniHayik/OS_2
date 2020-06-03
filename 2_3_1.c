#include <pthread.h> 
#include <semaphore.h> 
#include <stdio.h> 
#include <unistd.h>

#define N 5 
#define THINKING 2 
#define HUNGRY 1 
#define EATING 0 
#define FREE -1
#define LEFT (phnum + 4) % N 
#define RIGHT (phnum + 1) % N 


//Explanation - livelock:
//all philosophers take a fork and 
//than no one has two forks to eat with.


int state[N]; 

int phil[N] = { 0, 1, 2, 3, 4 }; 
int forks[N] = {-1, -1,-1,-1,-1};

sem_t mutex; 
sem_t S[N]; 


// change state
void changer(int phnum, int s){

	sem_wait(&mutex);
	state[phnum] = s; 
	sem_post(&mutex);

	if(s == THINKING) printf("Philosopher %d is Thinking\n", phnum + 1);

	else if(s == EATING) printf("Philosopher %d is Eating\n", phnum + 1);

	else if(s == HUNGRY) printf("Philosopher %d is Hungry\n", phnum + 1);  
}


// take up chopsticks
void take_leftF(int phnum) 
{ 
	sem_wait(&mutex); 

	if (forks[phnum] == FREE) { 

		forks[phnum] = phnum;

		printf("Philosopher %d takes fork %d (left fork)\n", 
					phnum + 1, phnum +1); 

	} 

	sem_post(&mutex); 
} 



void take_rightF(int phnum) 
{ 
	sem_wait(&mutex); 

	if (forks[phnum+1] == FREE) { 
		forks[phnum+1] = phnum;

		printf("Philosopher %d takes fork %d (right fork)\n", 
					phnum + 1, phnum +2); 

	} 
	sem_post(&mutex);
} 


// put down chopsticks 
void put_leftF(int phnum) 
{ 
	sem_wait(&mutex); 

	printf("Philosopher %d putting fork %d down (left fork)\n", 
		phnum + 1, phnum + 1); 
	
	forks[phnum] = FREE;

	sem_post(&mutex); 
} 


void put_rightF(int phnum) 
{ 
	sem_wait(&mutex);

	printf("Philosopher %d putting fork %d down (right fork)\n", 
		phnum + 1, phnum + 2); 
	
	forks[phnum+1] = FREE;

	sem_post(&mutex); 
} 


void* philospher(void* num) 
{ 
	while (1) { 
		int* i = num;

		changer(*i, HUNGRY);

		while(forks[*i] != *i){
			take_leftF(*i); 
			sleep(1);
		}

		take_rightF(*i); 
		sleep(1);
		if(forks[*i+1] == *i){
			changer(*i, EATING);
			sleep(2);
			put_leftF(*i);
			put_rightF(*i);
            changer(*i, THINKING);
            sleep(1); 
        }
        // if not put left fork back down
        else{
            put_leftF(*i);
            sleep(1);
        }

	} 
} 


int main() 
{ 
	int i; 
	pthread_t thread_id[N]; 

	// initialize the mutexes 
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

