#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

#define NUM_CHILD  3

#ifdef WITH_SIGNALS

    char interrupt_flag = 0;

    void create_sigaction(int sig_type, void(*sig_handler)(int)){
        struct sigaction sig_action;

        sig_action.sa_handler = sig_handler;

        sigemptyset(&sig_action.sa_mask);

        sig_action.sa_flags = SA_NODEFER;

        sigaction(sig_type, &sig_action, NULL);
    }

    void parent_signal_handler(int sig_type){
        if(sig_type == SIGINT){
            printf("PARENT [PID: %i] received terminate signal \n", getpid());
            interrupt_flag = 1;
        }
        else{
            printf("PARENT [PID: %i] received unsupported signal type %i \n", getpid(), sig_type);
        }
    }

    void child_signal_handler(int sig_type){
        if(sig_type == SIGTERM){
            printf("Child [PID: %i] received terminate signal \n", getpid());
            interrupt_flag = 1;
        }
        else{
            printf("Child [PID: %i] received unsupported signal type %i \n", getpid(), sig_type);
        }
    }

#endif

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
    printf("[ Signals handling is ");

    #ifdef WITH_SIGNALS
        for(int i=0; i < NSIG; i++){
            sigignore(i);
        }
        // setting up handler
        create_sigaction(SIGINT, parent_signal_handler);
        create_sigaction(SIGCHLD, SIG_DFL);
        printf("ON]\n");
    #else
        printf("OFF]\n");
    #endif

    printf("\n\n\n");

    pid_t list[NUM_CHILD];

    for(int i=0; i <  NUM_CHILD; i++){
        #ifdef WITH_SIGNALS
            if(interrupt_flag){
                printf("PARENT [%i]: \t", getpid());
                printf("Interrupt signal received. Killing all child processes: \n");
                kill_all(list, i);
                break;
            }
        #endif

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
            #ifdef WITH_SIGNALS
                create_sigaction(SIGINT, SIG_IGN);
                create_sigaction(SIGTERM, child_signal_handler);
            #endif
            printf("\t Child created with PID: %i \n", getpid());
            sleep(10); // make it 10 later. TODO: make it ten
            printf("Child with PID: %i completed the job. \n", getpid());
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
            printf("\t Child[%i] says: The process was completed. \n", temp);
            j++;
        }
    }
    printf("\nSuccess: %i child processes finished.\n", j);

    #ifdef WITH_SIGNALS
        for(int i = 0; i < NSIG; i++)  signal(i, SIG_DFL); // restore signal handling.
    #endif

    return 0;
}
