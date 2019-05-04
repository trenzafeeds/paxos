/*
 * node.c
 */

#include "../headers/paxos.h"

int paxos(proc_info self)
{
  message recd_m;
  int moacc = FALSE; /* moacc = Majority Of ACCeptances */
  if ((recd_m = receive_m(self->listen)) != -1) {
    self->received++;
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
          if (acc_prom(self, recd_m)) {
            #ifdef DEBUG
            printf("Proposer received majority of promises. Proposing with number %d\n",
                   self->curr);
            #endif
            propose(self, self->prom_data[1], self->promises, self->prom_data[2]);
            wipe(self->prom_data, 3);
            wipe(self->promises, (MAXNODES/2) + 1);
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
      #ifdef DEBUG
      printf("Proposer issuing prepare with number %d\n", self->curr);
      #endif
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
  me->received = 0;
  char *mypath = nid(id);
  me->listen = init_queue(mypath, 0, MAXMSGS, M_SIZE);
  int accu;

  #ifdef DEBUG
    printf("Process %d setup, going to sleep\n", me->id);
  #endif
  
  raise(SIGTSTP);
  
  while ((accu = paxos(me)) != 1) {
    sleep(1);
  }
  
  printf("Process %d learned %d in %d messages.\n", me->id, me->order[0], me->received);
  
  close_queue(me->listen);
  mq_unlink(mypath);
  free(me);
  return 0;
}
