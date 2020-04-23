#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#define N 5
#define EMPTY 7
#define SITTING 5
#define DONE 3
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N

//global variables
int _index,limit;
int state[N];
int phil[N] = { 0, 1, 2, 3, 4};
char name[15][10];
pthread_t thread_id[N];
int status[N][10];

//semaphores
sem_t mutex;
sem_t S[N];
sem_t noodles;

//functions
void test(int phnum);
void take_fork(int phnum);
void put_fork(int phnum);
void* philospher(void* num);
void serve_n_philosophers(int n);
void printTable();
void clean_table();

//MAIN METHOD
int main()
{
	int number,i;

	printf("\nEnter no. of philosophers: ");
	scanf("%d",&number);

	limit=number;

	//taking names of philosophers
	printf("\nEnter names of philosophers: ");
	for(i=0;i<number;i++)
	{
		scanf("%s ",name[i]);

	}

	printf("\nWe can only serve five people at a time");
	printf("\n\t\tPhilosophers will be served Noodles\n");
	while(number>=N)
	{
		_index=limit-number;
		//Serve N philosophers
		clean_table();
		printf("\nThis is served table*************************************\n");
		printTable();
		serve_n_philosophers(N);
		number-=N;
		clean_table();
		printf("\nThis is AFTER served table*************************************\n");
		printTable();

	}


	if (number>0)
	{
		_index=limit-number;

		printf("\nThis is served table***************************\n");
		printTable();
		serve_n_philosophers(number);

	}

	return 0;
}
//end of main

//function to serve n number of philosophers
void serve_n_philosophers(int n)
{int i;

	//printf("\n\n_index changed to: %d\n\n",_index);
	printf("\nCurrently serving philosophers: ");
	for (i=0;i<n;i++)
		printf(" %d |%s|",i+1,name[_index+i]);
	printf(" 1\n");
	// initialize the semaphores
	sem_init(&mutex, 0, 1);

	for (i = 0; i < n; i++)
		sem_init(&S[i], 0, 0);

	for (i = 0; i < n; i++) {
		// create philosopher processes
		pthread_create(&thread_id[i], NULL,
					philospher, &phil[i]);
		state[i]=SITTING;
		printf("Philosopher %s is siting\n", name[_index+i]);
	}
	printf("\n\n" );

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
		//Noodles
		printf("Noodle is served to philosopher %s\n",name[_index+*i]);
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

		printTable();
		//sleep(2);

		// printf("Philosopher %s takes fork %d and %d\n",
		// 			name[_index+phnum], LEFT + 1, phnum + 1);

		// printf("Philosopher %s is Eating\n", name[_index+phnum] );
		//status[phnum]=DONE;
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

	// printf("Philosopher %s is Hungry\n", name[_index+phnum]);

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
	state[phnum] = DONE;

	// printf("Philosopher %s putting fork %d and %d down\n",
	// 	name[_index+phnum], LEFT + 1, phnum + 1);
	//printf("Philosopher %s is thinking\n", name[_index+phnum]);

	test(LEFT);
	test(RIGHT);
	sem_post(&mutex);
}

void printTable()
{
	for(int i=0; i<N ; i++ ){

		printf("|%s|\t",name[_index+i]);
	}
	printf("\n");
	for(int i=0; i<N ; i++ )
		if (state[i]==EMPTY)
				printf("EMPTY  ");
		else if(state[i]==HUNGRY)
			printf("HUNGRY\t");
		else if (state[i]==DONE)
			printf("DONE\t");
		else if (state[i]==EATING)
			printf("EATING  ");
		else if (state[i]==SITTING)
			printf("SITTING  ");
	printf("\n\n\n\n");
}

void clean_table()
{
	for(int i; i<N; i++)
		{state[i]=EMPTY;
	printf("%d",state[i]);
}
	// for(int i; i<N; i++)
	// 	printf("%d",state[i]);

}
