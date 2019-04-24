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
#include <stdlib.h>
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

/* Message types */
/* Prepare statement */
#define MSG_PREP 1
/* Promise (or denial) */
#define MSG_PROM 2
/* Proposal AKA Accept Request */
#define MSG_PROP 3
/* Accept (acceptance of proposal) */
#define MSG_ACC 4
/* Ack or nAck (misc) */
#define MSG_ACK 5

typedef struct _message *message;
struct _message {
  long m_type;
  int m_num;
  int m_val;
  int m_auth;
};

#define M_SIZE sizeof(struct _message)

/* In messages.c */
message new_message(long type, int number, int value, int author);
mqd_t init_queue(char *desc, long m_flags, long m_max, long m_size);
mqd_t open_queue(char *desc, int permissions);
int close_queue(mqd_t mq_des);
char *nid(int id);
int send_m(message m_content, int *dests, int ind);

/* In paxos.c */
int main();

#endif /* PAXHEADER */

