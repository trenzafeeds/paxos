/* 
 * utils.c
 */

#include "../headers/paxos.h"


int roundend(proc_info self)
{
  clear_inbox(self);
  clear_data(self);
  return 0;
}

int clear_data(proc_info self)
{
  self->prep = 0;
  self->acc = 0;
  self->val = 0;
  wipe(self->tally, 3);
  wipe(self->prom_data, 3);
  wipe(self->promises, (MAXNODES/2) + 1);
  return 0;
}

int clear_inbox(proc_info self)
{
  message clear;
  while((clear = receive_m(self->listen)) != -1) {
    if (clear->m_num > self->prep) {
      if (mq_send(self->listen, (char *) clear, M_SIZE, 5) == -1) {
        perror("Error at clear_inbox send\n");
      }
      free(clear);
      break;
    }
    self->received++;
    free(clear);
  }
  return 0;
}

int wipe(int *arr, int len)
{
  for (int i = 0; i < len; i++) {
    arr[i] = 0;
  }
  return 0;
}
