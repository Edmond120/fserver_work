#include "pipe_networking.h"
#include <signal.h>
#include <string.h>

void process(char *s);
void subserver(int from_client);

static void sighandler(int signo) {
  if (signo == SIGINT) {
    remove("luigi");
    exit(0);
  }
}

int main() {
  signal(SIGINT,sighandler);
  while(1){
    //Blocks Until Client Connects
    int from_client = server_setup();

    //We now have a client, time to fork
    int f = fork();
    if(!f){
      printf("subserver created\n");
      subserver(from_client);
    }
    else{
      close(from_client);
      //remove(from_client);
    }
  }
}

void subserver(int from_client) {
  int to_client = server_connect(from_client);
  char* buff = (char*) calloc((BUFFER_SIZE / sizeof(char)), sizeof(char));
  while(read(from_client, buff, BUFFER_SIZE)){
    printf("[subserver] recieved: [%s]\n", buff);
    process(buff);
    write(to_client, buff, BUFFER_SIZE);
  }
}

void process(char * s) {//reverses string
	int len = strlen(s);
	int start = 0;
	int end = len - 1;
	char tmp;
	printf("len: %d\n",len);
	while(start < end){
		tmp = s[start];
		s[start++] = s[end];
		s[end--] = tmp;
	}
}
