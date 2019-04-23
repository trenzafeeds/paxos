/****************************************
 * messages.c
 * 
 * Implementation of message-manipulation
 * functions/wrappers
 * 
 * Kat Cannon-MacMartin
 ****************************************/

#include "headers/paxos.h"

message new_message(int dest, long type, int number, int value)
{
  message new_m = malloc(M_SIZE);
  new_m.m_dest = dest;
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

int close_queue(mqd_t mq_des)
{
  if ((int retval = mq_close(mq_des)) == -1) {
    perror("Error at close_queue\n");
    exit(1);
  }
  return retval; 
}

int c_send(message m_content)
{
  mqd_t server_queue = open_queue(S_QUEUE, O_WRONLY);
  if ((int retval = mq_send(server_queue, &m_content, M_SIZE, 0)) == -1) {
    perror("Error sending message\n");
  }
  close_queue(server_queue);
  return retval;
}

int *parse_dest(int dest)
{
  int *a = calloc(sizeof(int)*10);
  a[0] = 1;
  a[1] = 5;
  return a;
}

int s_send(message m_content, char *pathtable)
{
}
  
  
