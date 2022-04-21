#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

#define NUM_CHILD  3

void kill_all(pid_t *list, int size){
    printf("\t PARENT: \n");
    for(int i=0; i<size; i++){
        printf("\t Sent SIGTERM to Child %i \n", list[i]);
        kill(list[i], SIGTERM);
    }
}

int main(){
    pid_t parent_pid = getpid();
    int pcounter = 0;

    printf("PARENT: \t Main process started with PID: %i \n", parent_pid);

    pid_t list[NUM_CHILD];

    for(int i=0; i <  NUM_CHILD; i++){
        printf("PARENT [%i]: \t", getpid());
        printf("Trying to create child #%i\n", i);

        pid_t child = fork();

        if(child == -1){ // checking if it is correctly created.
            printf("PARENT[%i]: \t", getpid());
            printf("Failed to create child. Killing all processes. \n");
            kill_all(list, i);
            exit(1);
        }

        if(child == 0){ // printing on child created.
            printf("\t Child created with PID: %i \n", getpid());
            sleep(2); // make it 10 later. TODO: make it ten
            printf("Worker with PID: %i completed the job. \n", getpid());
            return 0;
        }

        list[i] = child;
        pcounter = i;
        sleep(1); // one second delays between fork call.
    }

    // waiting for processes to finish

    printf("\n PARENT: [%i] has created %i processes. \n", parent_pid, pcounter +1);

    int child, j=0;
    while(1){
        int temp = wait(&child);

        if(temp == -1) break;
        else{
            printf("\t Worker[%i] says: The process was completed. \n", temp);
            j++;
        }
    }
    printf("\nSuccess: %i child processes finished.\n", j);
    return 0;
}
