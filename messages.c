/*
 * messages.c
 */

#include "headers/paxos.h"

message new_message(long type, int number, int value)
{
  message new_m = malloc(M_SIZE);
  new_m.m_type = type;
  new_m.m_num = number;
  new_m.m_val = value;
  return new_m;
};

mqd_t init_queue(char *desc, long m_flags, long m_max, long m_size)
{
  struct mq_attr attr;
  attr.mq_maxmsg = m_max;
  attr.mq_msgsize = m_size;
  attr.mq_flags = m_flags;

  if ((mqd_t mq_des = mq_open(&desc, O_RDWR | O_CREAT, 0664, &attr)) == -1) {
    perror("Error at init_queue\n");
    exit(1);
  }

  return mq_des;
}

mqd_t open_queue(char *desc, int permissions)
{
  if ((mqd_t mq_des = mq_open(&desc, permissions)) == -1) {
    perror("Error at open_queue\n");
    exit(1);
  }

  return mq_des;
}



