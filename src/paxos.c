/*
 * paxos.c 
 */

#include "../headers/paxos.h"
#define TESTNODES 5

int main(int argc, char* argv[])
{
  int nodes;
  int i;
  pid_t pid;
  
  if (argc <= 1) {
    nodes = TESTNODES;
  } else {
    nodes = atoi(argv[1]);
    if (nodes > MAXNODES) {
      printf("Error: MAXNODES is currently %d. Recompile with a MAXNODES larger than %d.",
             MAXNODES, nodes);
      exit(1);
    } else if (nodes < 5) {
      printf("Error: Sorry, Paxos only likes to be run with 5 or more nodes at the moment.\n");
      exit(1);
    }
  }

  #ifdef DEBUG
  printf("Toy Paxos running in VERBOSE mode with %d nodes\n", nodes);
  #endif
  
  for (i = 0; i < nodes; i++) {
    if ((pid = fork()) == -1) {
      perror("Fork error\n");
      exit(1);
    } else if (pid == 0) {
      node(FIRSTID + i, nodes);
      exit(0);
    } else {
      printf("Forked: %d with id: %d\n", (int) pid, FIRSTID + i);
    }
  }

  for (i = 0; i < nodes; i++) {
    waitpid(-1, NULL, WUNTRACED);
  }
  
  printf("\n");
  #ifdef DEBUG
  printf("Waking all processes up\n");
  #endif
  kill(0, SIGCONT);

  for (i = 0; i < nodes; i++) {
    wait(NULL);
  }
  
  return 1;
}
