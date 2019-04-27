/*
 * node.c
 */

#include "../headers/paxos.h"

int paxos(proc_info self)
{
  if ((recd_m = receive_m(self->listen)) != -1) {

    switch(recd_m->type) {
      case MSG_PREP:
        acc_prep(self, recd_m);
      case MSG_PROM:

      case MSG_PROP:
        
      case MSG_ACC:
        count_acc(self, recd_m);
      case MSG_TCH:

    }
  } else {
    /* NO MESSAGE RECIEVED */
  }
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


  
  close_queue(me->listen);
  mq_unlink(mypath);
  return 0;
}
