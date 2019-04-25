/****************************************
 * messages.c
 * 
 * Implementation of message-manipulation
 * functions/wrappers
 * 
 * Kat Cannon-MacMartin
 ****************************************/

#include "../headers/paxos.h"

message new_message(long type, int number, int value, int author)
{
  message new_m = malloc(M_SIZE);
  new_m->m_type = type;
  new_m->m_num = number;
  new_m->m_val = value;
  new_m->m_auth = author;
  return new_m;
}

mqd_t init_queue(char *desc, long m_flags, long m_max, long m_size)
{
  mqd_t mq_des;
  struct mq_attr attr;
  attr.mq_maxmsg = m_max;
  attr.mq_msgsize = m_size;
  attr.mq_flags = m_flags;

  if ((mq_des = mq_open(desc, O_RDWR | O_CREAT, 0664, &attr)) == -1) {
    perror("Error at init_queue\n");
    exit(1);
  }

  return mq_des;
}

mqd_t open_queue(char *desc, int permissions)
{
  mqd_t mq_des;
  if ((mq_des = mq_open(desc, permissions)) == -1) {
    perror("Error at open_queue\n");
    exit(1);
  }

  return mq_des;
}

int close_queue(mqd_t mq_des)
{
  int retval;
  if ((retval = mq_close(mq_des)) == -1) {
    perror("Error at close_queue\n");
    exit(1);
  }
  return retval; 
}

char *nid(int id)
{
  char *desc = malloc(sizeof(char) * DESCSIZE);
  char c = id;
  snprintf(desc, DESCSIZE, "%s%c", DESCPREF, c);
  return desc;
}

int send_m(message m_content, int *dests, int ind)
{
  int retval = 0;
  for (int i = 0; i < ind; i++) {
    char *path = nid(dests[i]);
    mqd_t target = open_queue(path, O_WRONLY);
    free(path);
    if (mq_send(target, (char *) m_content, M_SIZE, 0) == -1) {
      perror("Error at send_m\n");
      retval = -1;
    }
    close_queue(target);
  }
  free(m_content);
  return retval; 
}

message receive_m(mqd_t mqdes)
{
  message m_content = malloc(M_SIZE);
  if (mq_receive(mqdes, (char *) m_content, M_SIZE, NULL) == -1) {
    return -1;
  } else {
    return m_content;
  }
}

  
