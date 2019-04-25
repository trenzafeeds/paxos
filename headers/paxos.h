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
#include <time.h>

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
#define FIRSTID 97

/* Modify these for size of run */
#define MAXACC 20
#define MAXVAL 20

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

typedef struct _proc_info *proc_info;
struct _proc_info {
  int id;
  int leader;  //TODO: Initialize to NULL
  /* For proposer */
  int curr;
  int inc;
  /* For acceptor */
  int prep;
  int acc;
  int val;
  int order[MAXACC];
};
#define PROCINF_SIZE sizeof(struct _proc_info)

/* In messages.c */
message new_message(long type, int number, int value, int author);
mqd_t init_queue(char *desc, long m_flags, long m_max, long m_size);
mqd_t open_queue(char *desc, int permissions);
int close_queue(mqd_t mq_des);
char *nid(int id);
int send_m(message m_content, int *dests, int ind);
message receive_m(mqd_t mqdes);

/* In roles.c */
int prepare(proc_info self);
int propose(proc_info self, int value, int *acceptors, int majority);
int promise(proc_info self, int num, int value, int dest_id);
int accept(proc_info self, message m_content);

/* In paxos.c */
int main();

#endif /* PAXHEADER */

