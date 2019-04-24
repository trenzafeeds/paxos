/********************************
 * paxos.h
 * 
 * Include statements and macros
 * for basic Paxos implementation 
 *
 * Kat Cannon-MacMartin
 ********************************/

/* Standards */
#include <stdio.h>
#include <stlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

/* POSIX Messages */
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#ifndef PAXHEADER
#define PAXHEADER 1

/* Definitions */
#define TRUE 1
#define FALSE 0
#define DESCPREF "/_"
#define DESCSIZE 4

typedef struct _message *message;
struct _message {
  long m_type;
  int m_num;
  int m_val;
  int m_auth;
};

#define M_SIZE sizeof(_message)

/* In messages.c */
message new_message(long type, int number, int value, int author);
mqd_t init_queue(char *desc, long m_flags, long m_max, long m_size);
mqd_t open_queue(char *desc, int permissions);
int close_queue(mqd_t mq_des);
char *nid(int id);
int send_m(message m_content, int *dests, int ind);

#endif /* PAXHEADER */

