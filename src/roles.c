/*
 * roles.c
 */

#include "../headers/paxos.h"

int prepare(proc_info self)
{
  if (self->curr <= self->prep) {
    self->curr = self->curr + self->inc;
  }
  message prep = new_message(MSG_PREP, self->curr, 0, self->id);

  int acceptors[self->inc - 1];
  int j = 0;
  for (int i = 0; i < (self->inc - 1); i++) {
    if ((FIRSTID + j) != self->id) {
      acceptors[i] = FIRSTID + j;
      j++;
    } else {
      j++;
      acceptors[i] = FIRSTID + j;
    }
  }

  int retval = send_m(prep, acceptors, self->inc);

  return retval;
}

int propose(proc_info self, int value, int *acceptors, int majority)
{
  if (value == NULL) {
    value = rand() % MAXVAL;
  }

  message prop = new_message(MSG_PROP, self->curr, value, self->id);
  int retval = send_m(prop, acceptors, majority);

  return retval;
}
  
