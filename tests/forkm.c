/*
 * Tests
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define ONEPATH "/hello"
#define DESCPREF "/_"
#define DESCSIZE 4
#define MSGSIZE sizeof(int)

char *nid(int id)
{
  char *desc = malloc(sizeof(char) * DESCSIZE);
  char c = id;
  snprintf(desc, DESCSIZE, "%s%c", DESCPREF, c);
  return desc;
}

int individual(int id)
{
  mqd_t mq_des;
  struct mq_attr attr;
  mode_t mode = 0664;
  attr.mq_maxmsg = 20;
  attr.mq_msgsize = MSGSIZE;
  attr.mq_flags = O_NONBLOCK;
  
  if ((mq_des = mq_open(ONEPATH, O_CREAT | O_RDWR, mode, attr)) == -1) {
    perror("Error at mq_open");
    exit(1);
  }

  mq_close(mq_des);
  mq_unlink(ONEPATH);

  return 0;
}
  
#define TESTNODES 5

int main(){

  pid_t pid;
  int nodes = TESTNODES;

  for (int i = 0; i < nodes; i++) {
    if ((pid = fork()) == -1) {
      perror("Fork error\n");
      exit(1);
    } else if (pid == 0) {
      individual(2);
      return 0;
    } else {
      printf("Forked: %d\n", (int) pid);
    }
  }
  return 1;
}
