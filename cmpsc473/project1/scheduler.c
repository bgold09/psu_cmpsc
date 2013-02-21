#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <time.h>
#include <sched.h>
#include <math.h>

#include <pthread.h>
#include <semaphore.h>

#define FCFS 0
#define SRTF 1
#define PBS  2
#define MLFQ 3
#define MAX_SIZE 32
#define NUM_PRIO 5

struct _thread_info 
{
	int id;
	float arrival_time;
	int required_time;
	int priority;
};
typedef struct _thread_info _thread_info_t;

struct _node
{
	_thread_info_t *tinfo;
	pthread_cond_t cond;
};
typedef struct _node node_t; 

struct _thread_queue
{
	int head;
	int tail;  /* tail points to element after the last element */
	node_t *q[MAX_SIZE];
};
typedef struct _thread_queue pqueue_t;

extern char *prog;
pqueue_t *ready;
static int type;
pthread_mutex_t m_queue;
pthread_mutex_t m_sched;
pthread_mutex_t m_globtime;
float globtime;

int init_pqueue(pqueue_t *queue)
{
	queue->head = 0;
	queue->tail = 0;

	return 0;
}

int is_empty(pqueue_t *q)
{
	return q->head == q->tail;
}

/*  */
int is_member(pqueue_t *queue, int tid)
{
	int i; 
	
	for (i = queue->head; i != queue->tail; i = (i + 1) % MAX_SIZE) {
		if (queue->q[i]->tinfo->id == tid) {
			return 1;
		}
	}

	return 0;
}

void dealloc_node(node_t *t)
{
	free(t->tinfo);
	pthread_cond_destroy(&(t->cond));
	free(t);
	t = NULL;
}

void enqueue(pqueue_t *queue, node_t *t)
{
	pthread_mutex_lock(&m_queue);
	queue->q[queue->tail] = t;
	queue->tail = (queue->tail + 1) % MAX_SIZE;
	pthread_mutex_unlock(&m_queue);
}

void dequeue(pqueue_t *queue)
{
	pthread_mutex_lock(&m_queue);
	dealloc_node(queue->q[queue->head]);
	queue->head = (queue->head + 1) % MAX_SIZE;
	pthread_mutex_unlock(&m_queue);
}

node_t *alloc_node(int tid, int remaining_time, int priority)
{
	_thread_info_t *tinfo;
	node_t *p;

	if ((tinfo = (_thread_info_t *) malloc(sizeof(*tinfo))) == NULL) {
		return NULL;
	}

	if ((p = (node_t *) malloc(sizeof(node_t))) == NULL) {
		return NULL;
	}

	tinfo->id = tid;
	tinfo->required_time = remaining_time;
	tinfo->arrival_time = globtime;
	tinfo->priority = priority;

	p->tinfo = tinfo;
	pthread_cond_init(&(p)->cond, NULL);

	return p;
}

int indexof(pqueue_t *queue, int tid)
{
	int i;

	for (i = queue->head; i != queue->tail; i = (i + 1) % MAX_SIZE) {
		if (queue->q[i]->tinfo->id == tid) {
			return i;
		}
	}

	return -1;
}

/**
 * schedule_fcfs - schedule the calling thread according to 
 *   First-Come-First-Served (FCFS) scheduling (no preemption)
 * @param tid: thread identification number of the calling thread
 * @param remaining_time: amount of CPU time requested by the thread
 * @return the global current time
 */
int schedule_fcfs(int tid, int remaining_time)
{
	pqueue_t *curr;
	node_t *t;
	int index;
	int ret;

	curr = &ready[0];
	ret = -1;

	pthread_mutex_lock(&m_sched);

	if (is_empty(curr)) {
		t = alloc_node(tid, remaining_time, 0);  /* bogus priority */
		enqueue(curr, t); 
		ret = ceil(globtime);
	} else if (remaining_time == 0) {   /* thread is finished with the CPU */
		dequeue(curr);  /* remove calling thread from the queue */
		if (!is_empty(curr)) {  /* signal the head if there are threads still on the queue */
			pthread_cond_signal(&(curr->q[curr->head]->cond)); /* signal head */
		}
		ret = globtime;
	} else {
		if (!is_member(curr, tid)) {   /* if thread not on the queue */
			t = alloc_node(tid, remaining_time, 0);  /* bogus priority */
			enqueue(curr, t);
		}

		if (curr->q[curr->head]->tinfo->id == tid) {  /* if this thread at the head */
			ret = globtime;
		} else {
			pthread_cond_signal(&(curr->q[curr->head]->cond)); /* signal head */
			if ((index = indexof(curr, tid)) != -1) {  /* find index of this thread */
				pthread_cond_wait(&(curr->q[index]->cond), &m_sched); /* wait until signaled */
				ret = globtime;
			}
		}
	}

	pthread_mutex_unlock(&m_sched);

	return ret;
}

/**
 * schedule_srtf - schedule the calling thread according to 
 *   Shortest-Remaining-Time-First (SRTF) scheduling (with preemption)
 * @param current_time: the time when the call to schedule_fcfs is made 
 *   by a calling thread
 * @param tid: thread identification number of the calling thread
 * @param remaining_time: amount of CPU time requested by the thread
 * @return the global current time
 */
int schedule_srtf(float current_time, int tid, int remaining_time)
{
	(void) current_time;
	(void) tid;
	(void) remaining_time;

	return 0;
}

/**
 * schedule_pbs - schedule the calling thread according to 
 *   Priority-Based Scheduling (PBS) (with preemption)
 * @param current_time: the time when the call to schedule_fcfs is made 
 *   by a calling thread
 * @param tid: thread identification number of the calling thread
 * @param remaining_time: amount of CPU time requested by the thread
 * @param tprio: priority of the thread
 * @return the global current time
 */
int schedule_pbs(float current_time, int tid, int remaining_time, int tprio)
{
	(void) current_time;
	(void) tid;
	(void) remaining_time;
	(void) tprio;
	return 0;
}

/**
 * schedule_fcfs - schedule the calling thread according to 
 *   Mulit-Level Feedback Queue (MLFQ) scheduling (with preemption)
 * @param current_time: the time when the call to schedule_fcfs is made 
 *   by a calling thread
 * @param tid: thread identification number of the calling thread
 * @param remaining_time: amount of CPU time requested by the thread
 * @param tprio: priority of the thread
 * @return the global current time
 */
int schedule_mlfq(float current_time, int tid, int remaining_time, int tprio) 
{
	(void) current_time;
	(void) tid;
	(void) remaining_time;
	(void) tprio;
	return 0;
}

int scheduleme(float time, int tid, int remaining_time, int tprio)
{
	pthread_mutex_lock(&m_globtime);
		globtime = time;
	pthread_mutex_unlock(&m_globtime);

	switch(type) {
	case FCFS:
		return schedule_fcfs(tid, remaining_time);
		break;
	case SRTF: 
		return schedule_srtf(time, tid, remaining_time);
		break;
	case PBS:
		return schedule_pbs(time, tid, remaining_time, tprio);
		break;
	case MLFQ:
		return schedule_mlfq(time, tid, remaining_time, tprio);
		break;
	}

	return 0;   /* FIX THIS */
}

/**
 * init_scheduler - 
 * @param sched_type:
 * @return void
 */
void init_scheduler(int sched_type) 
{
	int i;

	type = sched_type;
	pthread_mutex_init(&m_queue, NULL);
	pthread_mutex_init(&m_sched, NULL);
	pthread_mutex_init(&m_globtime, NULL);

	switch(sched_type) {
	case FCFS:
		if ((ready = (pqueue_t *) malloc(sizeof(*ready))) == NULL) {
			/* fprintf  */
			return;  /* how should we return? */
		}
		if (init_pqueue(&ready[0]) == -1) {
			return;
		}
		break;
	case SRTF:
		if ((ready = (pqueue_t *) malloc(sizeof(*ready))) == NULL) {
			/* fprintf  */
			return;  /* how should we return? */
		}
		if (init_pqueue(&ready[0]) == -1) {
			return;
		}
		break;
	case PBS:
		if ((ready = (pqueue_t *) malloc(sizeof(*ready) * NUM_PRIO)) == NULL) {
			/* fprintf  */
			return;  /* how should we return? */
		}

		for (i = 0; i < NUM_PRIO; i++) {
			if (init_pqueue(&ready[i]) == -1) {
				return;
			}
		}
		break;
	case MLFQ:
		if ((ready = (pqueue_t *) malloc(sizeof(*ready) * NUM_PRIO)) == NULL) {
			/* fprintf  */
			return;  /* how should we return? */
		}
		
		for (i = 0; i < NUM_PRIO; i++) {
			if (init_pqueue(&ready[i]) == -1) {
				return;
			}
		}
		break;
	default:
		/* fprintf(stderr, "%s: invalid scheduler type %d\n", prog, sched_type);*/
		fprintf(stderr, "invalid scheduler type %d, defaulting to FCFS\n", sched_type);
		type = FCFS;
		if ((ready = (pqueue_t *) malloc(sizeof(*ready))) == NULL) {
			/* fprintf  */
			return;  /* how should we return? */
		}
		if (init_pqueue(&ready[0]) == -1) {
			return;
		}
		break;
	}
}

