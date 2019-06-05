#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/fcntl.h>
#include <sys/wait.h>

#define SIGREADY 1
#define SIGCATCH 2
#define SIGDEAD 3

void sigReady(){
	printf("PID %d ready \n", getpid());
}
void sigCatch(){
	printf("PID %d caught one \n", getpid());
}
void sigDead(){
	printf("PID %d is dead \n", getpid());
}

int main(){
	pid_t pid,help;
	int sons[5];

	signal(SIGREADY,sigReady);
	signal(SIGCATCH,sigCatch);
	signal(SIGDEAD , sigDead);
	for(int i=0 ; i<5 ; i++) { // loop will run n times (n=5)
		pid = fork();
		if(!pid){
			break;
		}
	}

	for(int i=0 ; i < 5 ; i++){
		sleep(1);
		kill(getpid(),SIGREADY);
		break;
	}

	for(int i=0 ; i < 5 ; i++){
		sleep(1);
		kill(getpid(),SIGCATCH);
		break;
	}

	for(int i=0 ; i < 5 ; i++){
		sleep(1);
		help = getpid();
		sons[i] = help;
		kill(help,SIGDEAD);
		break;
	}
	exit(0);
	//return 0;
}
