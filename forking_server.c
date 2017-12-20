#include "pipe_networking.h"
#include <signal.h>

void process(char *s);
void subserver(int from_client);

static void sighandler(int signo) {
  if (signo == SIGINT) {
    remove("luigi");
    exit(0);
  }
}

int main() {
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
    /* process(buff); */
    write(to_client, buff, BUFFER_SIZE);
  }
}

void process(char * s) {

}
