/********************************
 * paxos.h
 * 
 * Include statements and macros
 * for basic Paxos implementation 
 *
 * Kat Cannon-MacMartin
 *
 * TODO: Vote tally in learner should be [current num, val, tally]
 ********************************/

/* Standards */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <string.h>

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
#define MAXMSGS 10
#define MAXNODES 10

/* Modify these for size of run */
#define MAXACC 20
#define MAXVAL 20


/* Message types */
/* Prepare statement */
#define MSG_PREP 1
/* Proposal AKA Accept Request */
#define MSG_PROP 2
/* Accept (acceptance of proposal) */
#define MSG_ACC 3
/* Teach accepted val (from learner) */
#define MSG_TCH 4
/* Ack or nAck (misc) */
#define MSG_NPROM 5
/* Promise (or denial) */
#define MSG_PROM 6

typedef struct _message *message;
struct _message {
  long m_type;
  int m_num;
  int m_val;
  int m_auth;
};
#define M_SIZE sizeof(struct _proc_info)

typedef struct _proc_info *proc_info;
struct _proc_info {
  int id;
  mqd_t listen;
  /* For proposer */
  int curr;
  int inc;
  /* For acceptor */
  int prep;
  int acc;
  int val;
  /* Universal */
  int tally[3];
  int promises[(MAXNODES / 2) + 1];
  int prom_data[3]; /* Highest num, val, # of promises */
  int round;
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

/* In utils.c */
int roundend(proc_info self);
int clear_data(proc_info self);
int clear_inbox(proc_info self);

/* In roles.c */
int prepare(proc_info self);
int propose(proc_info self, int value, int *acceptors, int majority);
int acc_prom(proc_info self, message m_content);
int acc_nprom(proc_info self, message m_content);
int acc_prep(proc_info self, message m_content);
int deny_prep(proc_info self, int num, int dest_id);
int promise(proc_info self, int num, int value, int dest_id);
int teach(proc_info self, int step, int value);
int accept(proc_info self, message m_content);
int p_learn(proc_info self, message m_content);
int count_acc(proc_info self, message m_content);
int learn(proc_info self);

/* In node.c */
int paxos(proc_info self);
int node(int id, int inc);

/* In paxos.c */
int main();

#endif /* PAXHEADER */

