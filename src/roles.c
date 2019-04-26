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

  int retval = send_m(prep, acceptors, self->inc - 1);

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

int promise(proc_info self, int num, int value, int dest_id)
{
  self->prep = num;

  int dest[1];
  dest[0] = dest_id;
  message prom = new_message(MSG_PROM, num, value, self->id);
  int retval = send_m(prom, dest, 1);

  return retval;
}

int teach(proc_info self, int step, int value)
{
  message tch = new_message(MSG_TCH, step, value, self->id);

  int pupils[self->inc - 1];
  int j = 0;
  for (int i = 0; i < (self->inc - 1); i++) {
    if ((FIRSTID + j) != self->id) {
      pupils[i] = FIRSTID + j;
      j++;
    } else {
      j++;
      pupils[i] = FIRSTID + j;
    }
  }

  int retval = send_m(tch, pupils, self->inc - 1);

  return retval;
}

int accept(proc_info self, message m_content)
{
  self->acc = m_content->m_num;
  self->val = m_content->m_val;

  message acc = new_message(MSG_ACC, m_content->m_num, m_content->m_val, self->id);
  int dest_count;
  int dest[2];
  if (self->leader != NULL) {
    dest[1] = self->leader;
    dest_count = 2;
  } else {
    dest_count = 1;
  }
  dest[0] = m_content->m_auth;
  
  int retval = send_m(acc, dest, dest_count);
  free(m_content);
  return retval;
}

int p_learn(proc_info self, message m_content)
{
  self->order[m_content->m_num] = m_content->m_val;
  self->round = m_content->m_num + 1;
  free(m_content);
  return 0;
}

int count_acc(proc_info self, int *tally, message m_content)
{
  if (m_content->m_num > tally[0]) {
    tally[0] = m_content->m_num;
    tally[1] = m_content->m_val;
    tally[2] = 1;
  } else if (m_content->m_num == tally[0]) {
    tally[2]++;
  }

  free(m_content);

  if (tally[2] > (self->inc / 2)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

int learn(proc_info self, int *tally)
{
  self->order[self->round] = tally[1];
  teach(self, self->round, tally[1]);
  self->round++;
  return 0;
}
  
  
  
                        
