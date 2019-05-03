/*
 * paxos.c 
 */

#include "../headers/paxos.h"
#define TESTNODES 5

int main(){

  int i;
  pid_t pid;
  int nodes = TESTNODES;

  for (i = 0; i < nodes; i++) {
    if ((pid = fork()) == -1) {
      perror("Fork error\n");
      exit(1);
    } else if (pid == 0) {
      node(FIRSTID + i, nodes);
      exit(0);
    } else {
      printf("Forked: %d\n", (int) pid);
    }
  }

  for (i = 0; i < nodes; i++) {
    waitpid(-1, NULL, WUNTRACED);
  }

  kill(0, SIGCONT);

  for (i = 0; i < nodes; i++) {
    wait(NULL);
  }
  
  return 1;
}
