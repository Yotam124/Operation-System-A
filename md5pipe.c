/*
 * Pipes.c
 *
 *  Created on: 22 May 2019
 *      Author: yotam dafna, matti stern, tomer hazan
 */


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
#include "md5.h"

#define MSGSIZE 20
#define ANSSIZE 32
char* ans;
int size;

void catchSig(int msgLen){
	if (size == 32){
		printf("%s", ans);
		exit(0);
	}
	signal(SIGINT, catchSig);
}

int main(int argc, char **argv){

	if (argc < 2) {
		printf("usage: %s 'string'\n", argv[0]);
		return 1;
	}

	int p[2];
	signal(SIGINT, catchSig);

	if (pipe(p) < 0){ //Opening pipes.
		exit(1);
	}
	int pid;

	if ((pid = fork() < 0)){ //creating child process.
		exit(1);
	}

	if (pid > 0){
		char* msg = argv[1];
		write(p[1], msg, strlen(msg)+1);
		close(p[1]);
		sleep(1);
		char* recans;
		read(p[0], recans , 255);
		size = strlen(recans);
		ans = recans;
		kill(pid,SIGINT);
		exit(0);
	}

	else {

		wait(NULL);
		char rec[32];
		read(p[0], rec, 32);
		std::string str;
		str = md5(rec);
		char send[str.length() + 1];
		strcpy(send, str.c_str());
		write(p[1], send, strlen(send)+1);
		close(p[1]);
	}

	return 0;
}

