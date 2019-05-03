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

int acc_prom(proc_info self, message m_content)
{
  if ((m_content->m_type - MSG_PROM) == self->curr) {
    self->promises[self->prom_data[2]] = m_content->m_auth;
    self->prom_data[2]++;
    
    if (m_content->m_num > self->prom_data[0]) {
      self->prom_data[0] = m_content->m_num;
      self->prom_data[1] = m_content->m_val;
    }

    printf("Prom_maj: %d vs. %d\n", self->prom_data[2], self->inc / 2);
    if (self->prom_data[2] > (self->inc / 2)) {
      free(m_content);
      return TRUE;
    }
  }
  free(m_content);
  return FALSE;
}

int acc_nprom(proc_info self, message m_content)
{
  if (m_content->m_num == self->curr) {
    for (int i = 0; i < self->inc; i++) {
      self->promises[i] = 0;
    }
    for (int i = 0; i < 3; i++) {
      self->prom_data[i] = 0;
    }
    self->curr += self->inc;
  }
  free(m_content);
  return 0;
}

int acc_prep(proc_info self, message m_content)
{
  int retval;
  if (m_content->m_num > self->prep) {
    self->prep = m_content->m_num;
    promise(self, self->acc, self->val, m_content->m_auth);
    retval = TRUE;
  } else {
    deny_prep(self, m_content->m_num, m_content->m_auth);
    retval = FALSE;
  }
  free(m_content);
  return retval;
}

int deny_prep(proc_info self, int num, int dest_id)
{
  int dest[1];
  dest[0] = dest_id;
  message nprom = new_message(MSG_NPROM, num, self->prep, self->id);
  int retval = send_m(nprom, dest, 1);
  return retval;
} 

int promise(proc_info self, int num, int value, int dest_id)
{
  /* NOTE: in this case, `num` is the highest number previously accepted,
     whereas self->prep is the number of the prepare/promise. */
  int dest[1];
  dest[0] = dest_id;
  message prom = new_message(MSG_PROM + self->prep, num, value, self->id);
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
  int accd;
  if (m_content->m_num >= self->prep) {
    self->acc = m_content->m_num;
    self->val = m_content->m_val;

    message acc = new_message(MSG_ACC, m_content->m_num, m_content->m_val, self->id);

    int dests[self->inc];
    for (int i = 0; i < self->inc; i++) {
      dests[i] = FIRSTID + i;
    }
  
    send_m(acc, dests, self->inc);
    accd = TRUE;
  } else {
    /* SEND DENIAL HERE? */
    accd = FALSE;
  }

  free(m_content);
  return accd;
}

int p_learn(proc_info self, message m_content)
{
  self->order[m_content->m_num] = m_content->m_val;
  self->round = m_content->m_num + 1;
  free(m_content);
  return 0;
}

int count_acc(proc_info self, message m_content)
{
  if (m_content->m_num > self->tally[0]) {
    self->tally[0] = m_content->m_num;
    self->tally[1] = m_content->m_val;
    self->tally[2] = 1;
  } else if (m_content->m_num == self->tally[0]) {
    self->tally[2]++;
  }

  free(m_content);

  if (self->tally[2] > (self->inc / 2)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

int learn(proc_info self)
{
  self->order[self->round] = self->tally[1];
  self->round++;
  return 0;
}
  
  
  
                        
