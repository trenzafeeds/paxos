/*
 * paxos.c 
 */

#include "../headers/paxos.h"
#define TESTNODES 5

int main(){

  pid_t pid;
  int nodes = TESTNODES;

  for (int i = 0; i < nodes; i++) {
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
  return 1;
}
