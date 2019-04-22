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

#define M_QUEUE "/m_queue"

/* Definitions */
#define TRUE 1
#define FALSE 0

#ifndef PAXHEADER
#define PAXHEADER 1

typedef struct _message *message;
struct _message {
  long m_type;
  int m_num;
  int m_val;
};

#define M_SIZE sizeof(_message)

message new_message(long type, int number, int value);
mqd_t init_queue(char *desc, long m_flags, long m_max, long m_size);
mqd_t open_queue(char *desc, int permissions);

#endif /* PAXHEADER */
