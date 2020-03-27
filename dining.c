#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#define N 3
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + 2) % N
#define RIGHT (phnum + 1) % N

//global variables
int index,limit;
int state[N];
int phil[N] = { 0, 1, 2 };
char name[15][10];
pthread_t thread_id[N];

//semaphores
sem_t mutex;
sem_t S[N];
sem_t app,main_course,desert;

//functions
void test(int phnum);
void take_fork(int phnum);
void put_fork(int phnum);
void* philospher(void* num);
void serve_n_philosophers(int n);

int main()
{
	int i;
	int number;

	printf("\nEnter no. of philosophers: ");
	scanf("%d",&number);

	limit=number;

	//taking names of philosophers
	printf("\nEnter names of philosophers: ");
	for(i=0;i<number;i++)
	{
		scanf("%s ",name[i]);

	}

	printf("\nWe can only serve three people at a time");
	printf("\n\t\tPhilosophers will be served 3 dishes(Appetiser, Main Course, Desert)\n");
	while(number>=N)
	{
		index=limit-number;
		//Serve N philosophers
		serve_n_philosophers(N);
		number-=N;
	}


	if (number>0)
	{
		index=limit-number;
		serve_n_philosophers(number);
	}

	return 0;
}
//end of main

//function to serve n number of philosophers
void serve_n_philosophers(int n)
{
	//printf("\n\nIndex changed to: %d\n\n",index);
	printf("\nCurrently serving philosophers: ");
	for (i=0;i<n;i++)
		printf("%s",name[index+i]);
	printf("\n");
	// initialize the semaphores
	sem_init(&mutex, 0, 1);
	for (i = 0; i < n; i++)

		sem_init(&S[index+i], 0, 0);

	for (i = 0; i < n; i++) {

		// create philosopher processes
		pthread_create(&thread_id[i], NULL,
					philospher, &phil[i]);

		printf("Philosopher %s is siting\n", name[index+i]);
	}
	//join the threads together
	for (i = 0; i < n; i++)
		pthread_join(thread_id[i], NULL);
}

//standard algorithm for dining philospher
void* philospher(void* num)
{

	while (1) {

		int* i = num;

		sleep(1);
		//Appetiser
		printf("Appetiser is served to philosopher %s\n",name[*i]);
		take_fork(*i);
		sleep(0);
		put_fork(*i);



		sleep(1);
		//Main Course
		printf("Main Course is served to philosopher %s\n",name[*i]);
		take_fork(*i);
		sleep(0);
		put_fork(*i);



		sleep(1);
		//Desert
		printf("Desert is served to philosopher %s\n",name[*i]);
		take_fork(*i);
		sleep(0);
		put_fork(*i);

		//End the thread
		pthread_exit(NULL);
	}
}


void test(int phnum)
{
	if (state[phnum] == HUNGRY
		&& state[LEFT] != EATING
		&& state[RIGHT] != EATING) {
		// state that eating

		state[phnum] = EATING;

		//sleep(2);

		printf("Philosopher %s takes fork %d and %d\n",
					name[index+phnum], LEFT + 1, phnum + 1);

		printf("Philosopher %s is Eating\n", name[index+phnum] );

		// sem_post(&S[phnum]) has no effect
		// during takefork
		// used to wake up hungry philosophers
		// during putfork
		sem_post(&S[phnum]);
	}
}

// take up forks
void take_fork(int phnum)
{

	sem_wait(&mutex);

	// state that hungry
	state[phnum] = HUNGRY;

	printf("Philosopher %s is Hungry\n", name[index+phnum]);

	// eat if neighbours are not eating
	test(phnum);

	sem_post(&mutex);

	// if unable to eat wait to be signalled
	sem_wait(&S[phnum]);

	sleep(1);
}

// put down forks
void put_fork(int phnum)
{

	sem_wait(&mutex);

	// state that thinking
	state[phnum] = THINKING;

	printf("Philosopher %s putting fork %d and %d down\n",
		name[index+phnum], LEFT + 1, phnum + 1);
	printf("Philosopher %s is thinking\n", name[index+phnum]);

	test(LEFT);
	test(RIGHT);

	sem_post(&mutex);
}
