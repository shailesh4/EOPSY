#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>

#define NUMBER_OF_PHILOSOHPERS 5 // number of philosophers
#define FOOD_LIMIT 3		// number of meals for each philosopher
#define EATING_TIME 1		// eating time
#define THINKING_TIME 2		// thinking time

void grab_forks(int left_fork_id);
void put_away_forks(int left_fork_id);
void forkop(int left_fork_id, short sem_op);
void inc_total_eat();
int get_total_eat();

void phil();
int eat(int meals_left);
void think();

char* philosophers_list[5] = {"0","1","2","3","4"};
int philosopher_id,semaphor_id,pid,status, total_eating;

int main() {
    printf("=========================================================\n");
    printf("Main process has started.\n");
    printf("Number of philosophers in dining session:\t %i\n", NUMBER_OF_PHILOSOHPERS);
    printf("Eating period for each philosopher:\t\t %i\n", EATING_TIME);
    printf("Thinking period for each philosopher:\t\t %i\n", THINKING_TIME);
    printf("The food limit of a philosopher is\t\t\t %is\n", FOOD_LIMIT);
    printf("=========================================================\n");
    printf("\n\n");
	
	// creating the set of semaphores
	semaphor_id = semget(IPC_PRIVATE, NUMBER_OF_PHILOSOHPERS, 0644 | IPC_CREAT);
	
	// creating one more semaphore (total number of meals eaten)	
	total_eating = semget(IPC_PRIVATE, 1, 0644 | IPC_CREAT);
	// initialize 
	if(semctl(total_eating, 0, SETVAL, 1) == -1){printf("semctl failed");}

	if(semaphor_id == -1) {
		printf("\nERROR: Allocation of semaphor set unsuccessfull.\n");
		exit(1);
	}
	
	// initializing the semaphores
	for (int i = 0; i < NUMBER_OF_PHILOSOHPERS; ++i)
    {
        if (semctl(semaphor_id, i, SETVAL, 1) == -1)
        {
            printf("semctl failed");
            return 1;
        }
    }

	for (int i = 0; i < NUMBER_OF_PHILOSOHPERS; ++i) {
		pid = fork();
		if(pid == 0) {
			philosopher_id = i;
			phil();
			printf ("%s has left the table.\n",philosophers_list[philosopher_id]);
			return 0;
		}
		else if(pid < 0) {
			kill(-2,SIGTERM);
			printf("\nERROR: can't create a processes\n");
			exit(1);
		}
	}

	while(1) {
		pid = wait(&status);
		if(pid < 0)
			break;
	}
	
	// Deallocation of semaphores:
	if (semctl (semaphor_id, 0, IPC_RMID, 1)<0)
		printf("ERROR deallocationg semaphores.\n");

	return 0;
}

void grab_forks(int left_fork_id) { return forkop(left_fork_id, -1); }

void put_away_forks(int left_fork_id) { return forkop(left_fork_id, 1); }

void forkop(int left_fork_id, short sem_op)
{
    int semcapacity = 2;
    struct sembuf op[semcapacity];

    if(philosopher_id % 2 == 0){
    	op[0].sem_num = left_fork_id;
    	op[1].sem_num = (left_fork_id + 1) % NUMBER_OF_PHILOSOHPERS;
	}else{
		op[0].sem_num = (left_fork_id + 1) % NUMBER_OF_PHILOSOHPERS;
    	op[1].sem_num = left_fork_id;
	}

    for(int counter = 0; counter < semcapacity; ++counter)
    {
        op[counter].sem_op = sem_op;
        op[counter].sem_flg = 0;
    }

    if (semop(semaphor_id, op, 2) == -1)
        printf("> \t\t %s_forks semop failed. \n", sem_op == -1 ? "grab" : "put_away");
}

void inc_total_eat()
{
	struct sembuf op[1];
	op[0].sem_num = 0;
	op[0].sem_op = 1;
	op[0].sem_flg = 0;
	
	if(semop(total_eating, op, 1) == -1)
		printf("forks semop failed.");
}

int get_total_eat()
{
	return semctl(total_eating,0, GETVAL, 1 );
}

void phil() {
	printf("%s came to the table.\n", philosophers_list[philosopher_id]);
	int no_of_meals = FOOD_LIMIT;
	char hungry = 0;
	while(no_of_meals) {
		if(hungry) {
			int val = eat(--no_of_meals);
			if(val == 1){
				no_of_meals++;
			}else{
				inc_total_eat();
			}
			hungry = 0;
		}
		else {
			think();
			hungry = 1;
		}
	}
}

int eat(int meals_left) {
	int current_meal = FOOD_LIMIT-meals_left;
	int total_meals = get_total_eat();
	if(total_meals/NUMBER_OF_PHILOSOHPERS + 1 >= current_meal){
		grab_forks(philosopher_id);
		printf("%s is eating\n", philosophers_list[philosopher_id]);
		sleep(EATING_TIME);
		printf("%s ate his %d",philosophers_list[philosopher_id],(FOOD_LIMIT-meals_left));
			switch (FOOD_LIMIT-meals_left){
				case 1:
					printf("st");
					break;
				case 2:
					printf("nd");
					break;
				case 3:
					printf("rd");
					break;
				default:
					printf("th");
			}
			printf(" meal.\n");
		put_away_forks(philosopher_id);
		return 0;
	}
	return 1;
}

void think() {
	printf("%s is thinking\n", philosophers_list[philosopher_id]);
	sleep(THINKING_TIME);
}
