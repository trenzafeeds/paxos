/*
 * node.c
 */

#include "../headers/paxos.h"

int paxos(proc_info self)
{
  message recd_m;
  if ((recd_m = receive_m(self->listen)) != -1) {

    switch(recd_m->m_type) {
      case MSG_PREP:
        acc_prep(self, recd_m); /* TODO: Handle T/F (FOR ALL(?) OF THEM) */
        break;
      case MSG_PROM:
        acc_prom(self, recd_m);
        break;
      case MSG_NPROM:
        break;
      case MSG_PROP:
        accept(self, recd_m);
        break;
      case MSG_ACC:
        count_acc(self, recd_m);
        break;
      case default:
        perror("Message recieved with incorrect type.\n");
        exit(1);
    }
  } else {
    /* NO MESSAGE RECIEVED */
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
  me->listen = init_queue(mypath, O_NONBLOCK, MAXMSGS, M_SIZE);

  /* REMEMBER TO INCREMENT me->round SOMEWHERE -- UNLESS it gets incremented in learn function */
  
  close_queue(me->listen);
  mq_unlink(mypath);
  free(me);
  return 0;
}
