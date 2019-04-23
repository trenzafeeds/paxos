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
#define S_QUEUE "/s_queue"
#define TRUE 1
#define FALSE 0

typedef struct _message *message;
struct _message {
  int m_dest;
  long m_type;
  int m_num;
  int m_val;
};

#define M_SIZE sizeof(_message)

/* In messages.c */
message new_message(int dest, long type, int number, int value);
mqd_t init_queue(char *desc, long m_flags, long m_max, long m_size);
mqd_t open_queue(char *desc, int permissions);
int close_queue(mqd_t mq_des);
int c_send(message m_content);
int *parse_dest(int dest);
int s_send(message m_content, char *pathtable);

#endif /* PAXHEADER */
