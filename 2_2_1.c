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
#define SUCCESS 1
#define FAIL 0


//Explanation - starvation:
//the three philosophers that joined last wants 
//to eat at the same time and beacause of that they 
//starve all the time.
	

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
int take_fork(int phnum) 
{ 
	int f = FAIL;
	sem_wait(&mutex); 

	if (forks[phnum] == FREE && forks[phnum+1] == FREE) { 
		forks[phnum] = phnum;
		printf("Philosopher %d takes forks %d and %d\n", 
					phnum + 1, phnum +1, phnum +2); 
		f = SUCCESS;
	} 
	sem_post(&mutex); 

	return f;
} 


// put down chopsticks
void put_fork(int phnum) 
{ 
	sem_wait(&mutex); 

	printf("Philosopher %d putting forks %d and %d down\n", 
		phnum + 1, phnum + 1, phnum + 2); 
	
	forks[phnum] = FREE;
	forks[phnum+1] = FREE;

	sem_post(&mutex); 
} 



void* philospher(void* num) 
{ 	
	int* i = num; 
	changer(*i, THINKING);
	sleep(1);
	changer(*i, HUNGRY);

	while (1) { 
		if(take_fork(*i) == SUCCESS){
			changer(*i, EATING);
			sleep(3);
			put_fork(*i);
			changer(*i, THINKING);
			sleep(1);
			changer(*i, HUNGRY);
		}

        else sleep(4);

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

	// create 2 philosophers
	pthread_create(&thread_id[2], NULL, 
					philospher, &phil[2]); 
	pthread_create(&thread_id[4], NULL, 
					philospher, &phil[4]); 

	// create the other philosophers
	sleep(2);
	pthread_create(&thread_id[0], NULL, 
					philospher, &phil[0]); 				
	pthread_create(&thread_id[1], NULL, 
					philospher, &phil[1]); 	
	pthread_create(&thread_id[4], NULL, 
					philospher, &phil[4]); 

	for (i = 0; i < N; i++) 

		pthread_join(thread_id[i], NULL); 
} 

