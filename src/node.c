/*
 * node.c
 */

#include "../headers/paxos.h"

int paxos(proc_info self)
{
  message recd_m;
  int moacc = FALSE; /* moacc = Majority Of ACCeptances */
  if ((recd_m = receive_m(self->listen)) != -1) {

    switch(recd_m->m_type) {
      case MSG_PREP:
        acc_prep(self, recd_m); /* TODO: Handle T/F (FOR ALL(?) OF THEM) */
        break;
      case MSG_NPROM:
        acc_nprom(self, recd_m);
        break;
      case MSG_PROP:
        accept(self, recd_m);
        break;
      case MSG_ACC:
        moacc = count_acc(self, recd_m);
        break;
      default:
        if (recd_m->m_type >= MSG_PROM) {
          printf("Rec prom: %d\n", self->curr);
          if (acc_prom(self, recd_m)) {
            printf("Acc_prom returned true\n");
            /* majority of promises */
          }
        } else {
          perror("Message type error.\n");
          exit(1);
        }
        break;
    }

    if (moacc) {
      learn(self);
      roundend(self);
      return 1;
    }
  } else {
    if (self->id == 97) {
      prepare(self);
    }
  }

  return 0;
}
  
int node(int id, int inc)
{
  /* INIT */
  proc_info me = malloc(PROCINF_SIZE);
  me->id = id;
  me->curr = id - FIRSTID + 1;
  me->inc = inc;
  char *mypath = nid(id);
  me->listen = init_queue(mypath, 0, MAXMSGS, M_SIZE);
  int accu;
  
  while ((accu = paxos(me)) != 1) {
    sleep(1);
  }
  
  printf("%d\n", accu);
  
  close_queue(me->listen);
  mq_unlink(mypath);
  free(me);
  return 0;
}
