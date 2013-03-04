/**
 * @file scheduler.c
 * @brief simulate scheduling of processes using FCFS, SRTF, 
 *        PBS and MLFQ scheduling
 * @date 03/03/13
 * @author Mike Fitz, Brian Golden
 *
 */

#include <stdio.h>
#include <stdlib.h>  /* malloc(), free() */
#include <math.h>    /* ceil()           */
#include <pthread.h> /* pthread_mutex_init(), pthread_mutex_lock(), 
		      * pthread_mutex_unlock(), pthread_cond_init(), 
		      * pthread_cond_wait(), pthread_cond_signal(), 
		      * pthread_cond_destroy()
		      */

#define FCFS     0
#define SRTF     1
#define PBS      2
#define MLFQ     3
#define NUM_PRIO 5   /* number of priority levels for PBS, MLFQ scheduling */
#define TQ1      5   /* time quantum for priority 1, MLFQ scheduling */
#define TQ2      10  /* time quantum for priority 2, MLFQ scheduling */
#define TQ3      15  /* time quantum for priority 3, MLFQ scheduling */
#define TQ4      20  /* time quantum for priority 4, MLFQ scheduling */
#define TQ5      25  /* time quantum for priority 5, MLFQ scheduling */


/**
 * struct _thread_info - information about a currently running thread
 *
 */
struct _thread_info 
{
	int id;              /* id number of the thread, from input file */
	float arrival_time;  /* time the thread arrived in the system */
	int required_time;   /* CPU time requested */
	int priority;        /* thread priority, 1 is highest, 5 is lowest */
};
typedef struct _thread_info _thread_info_t;

/**
 * struct _node - node of a thread queue
 *
 */
struct _node
{
	_thread_info_t *tinfo;  /* information about this thread */
	float current_time;     /* time the thread called scheduleme() */
	pthread_cond_t cond;    /* 
				 * thread waits on this condition variable to 
				 * be scheduled 
				 */
	struct _node *next;     /* next element of the queue */
};
typedef struct _node node;

/*
 * struct _link_queue - linked list of threads/nodes
 *
 */
struct _link_queue
{
	node *head;  /* head of the list, NULL indicates empty list */
	node *tail;  /* tail of the list */
};
typedef struct _link_queue pqueue_t;

static pqueue_t *ready[NUM_PRIO];    /* list of reday queues */
static int type;                     /* scheduling type being used */
float globtime;                      /* global system time for simulation */
static pthread_mutex_t m_sched;      /* lock scheduling operations */
static pthread_mutex_t m_globtime;   /* lock updating the global time */
static int last_tid;                 /* id of last running thread, for MLFQ */
static int counter;                  /* 
				      * remaining time of last running 
				      * thread's time quantum 
				      */
static int quantum[] =               /* time quanta for MLFQ scheduling */
	{TQ1, TQ2, TQ3, TQ4, TQ5};

/**
 * pqueue_allocate - allocate a new linked list queue of threads 
 *
 * @return pointer to allocated pqueue_t
 */
pqueue_t *pqueue_allocate(void) 
{
	pqueue_t *p;   /* pointer to new pqueue_t to allocate */
	
	p = (pqueue_t *) malloc(sizeof(*p));
	p->head = NULL;
	p->tail = NULL;

	return p;
}

/**
 * node_alloc - allocate a new thread node
 *
 * @param tid: thread id of the new node
 * @param remaining_time: CPU time requested by the thread
 * @param current_time: time at which the thread was created
 * @param priority: priority of this thread, for PBS scheduling
 */
node *node_alloc(int tid, int remaining_time, float current_time, int priority)
{
	node *p;                /* pointer to new node to allocate */
	_thread_info_t *tinfo;  /* pointer to new thread info to allocate */

	if ((tinfo = (_thread_info_t *) malloc(sizeof(*tinfo))) == NULL) {
		return NULL;
	} else if ((p = (node *) malloc(sizeof(*p))) == NULL) {
		return NULL;
	}

	tinfo->id = tid;
	tinfo->required_time = remaining_time;
	tinfo->arrival_time = globtime;
	tinfo->priority = priority;

	p->tinfo = tinfo;
	p->current_time = current_time;
	p->next = NULL;
	pthread_cond_init(&(p)->cond, NULL);

	return p;
}

/**
 * node_deallocate - free the memory allocated to a node
 *
 * @param p: pointer to node to deallocate
 * @return void
 */
void node_dealloc(node *p) 
{
	free(p->tinfo);
	p->tinfo = NULL;
	pthread_cond_destroy(&(p->cond));
	free(p);
	p = NULL;
}

/**
 * enqueue - add a node to the end of a queue
 *
 * @param queue: queue to append to
 * @param p: node to append to queue
 * @return void
 */
void enqueue(pqueue_t *queue, node *p)
{
	if (queue->head == NULL) {              /* empty queue */
		queue->head = queue->tail = p;
	} else {
		queue->tail->next = p;
		queue->tail = p;
	}
}

/**
 * dequeue - remove the node at the front of a queue
 *
 * @param queue: queue to remove node from
 * @return pointer to removed node
 */
node *dequeue(pqueue_t *queue) 
{
	node *t;              /* pointer to node to remove */

	t = queue->head;
	queue->head = t->next;
	t->next = NULL;

	return t;
}

/**
 * pqueue_remove - remove a node from a queue, search by its tid
 *
 * @param queue: queue to remove from
 * @param tid: thread id of node to remove from queue
 * @return pointer to removed node
 */
node *pqueue_remove(pqueue_t *queue, int tid)
{
	node *p;     /* check this node */
	node *ret;   /* node to remove */

	ret = NULL;

	/* head of queue is the thread we are searching for */
	if (queue->head->tinfo->id == tid) {  
		ret = queue->head;
		queue->head = queue->head->next;
	} else {
		for (p = queue->head; p->next != NULL; p = p->next) {
			if (p->next->tinfo->id == tid) {
				if (p->next == queue->tail) {
					queue->tail = p;
				}
				ret = p->next;
				p->next = p->next->next;
				ret->next = NULL;
				return ret;
			}
		}
	}

	return ret;
}

/**
 * is_empty - check if a queue has no nodes on it
 *
 * @param queue: queue of threads to check if empty
 * @return 0 queue is not empty, 1 queue is empty
 */
int is_empty(pqueue_t *queue) 
{
	return queue->head == NULL;
}

/**
 * are_queues_empty - check if all the ready queues are empty
 *
 * @return 0 some ready queue is not empty, 1 all ready queue are empty
 */
int are_queues_empty()
{
	int i;

	for (i = 0; i < NUM_PRIO; i++) {       /* check each ready queue */
		if (!is_empty(ready[i])) {
			return 0;
		}
	}

	return 1;
}

/**
 * is_member - check if a queue contains a thread with thread id tid
 *
 * @param queue: queue of threads to check
 * @param tid: thread id to search for on queue
 * @return 0 queue does not have a thread with id tid, 1 queue contains a 
 *         thread with thread id tid
 */
int is_member(pqueue_t *queue, int tid)
{
	node *p;

	/* check each thread on queue */
	for (p = queue->head; p != NULL; p = p->next) {
		if (p->tinfo->id == tid) {
			return 1;
		}
	}

	return 0;
}

/**
 * queue_find - find the node on queue with thread id tid
 *
 * @param queue - queue to search for thread
 * @param tid: thread if to look for
 * @return pointer to node if match found, NULL if not found
 */
node *queue_find(pqueue_t *queue, int tid)
{
	node *p;

	for (p = queue->head; p != NULL; p = p->next) {
		if (p->tinfo->id == tid) {
			return p;
		}
	}

	return NULL;
}

/**
 * find_hp - find the thread with the highest priority on the first non-null 
 *           queue with earliest arrival time
 *
 * @return pointer to node of thread with highest priority
 */
node *find_hp(void) 
{
	node *high;       /* highest priority node, earliest arrival time */
	node *p;          /* check node on queue */
	pqueue_t *queue;  /* queue to check */
	int i;

	/* find first non-null queue */
	for (i = 0; i < NUM_PRIO && is_empty(ready[i]); i++);
	queue = ready[i];

	high = queue->head;

	/* find thread with earliest arrival time */
	for (p = queue->head->next; p != NULL; p = p->next) {  
		if (p->current_time < high->current_time) {
			high = p;
		}
	}

	return high;
}

/**
 * signal_hp - signal the thread with the highest priority
 *
 * @return void
 */
void signal_hp(void)
{
	node *t;  /* thread with the highest priority in the system */

	t = find_hp();  /* find the thread with the higest priority */
	pthread_cond_signal(&(t->cond)); /* signal that thread */
}

/**
 * find_mlfq - find the thread to schedule for MLFQ scheduling
 *
 * @return void
 */
node *find_mlfq(void)
{
	int i;
	
	/* find first non-null queue */
	for (i = 0; i < NUM_PRIO && is_empty(ready[i]); i++);

	return ready[i]->head;
}

/**
 * singal_mlfq - signal the thread to schedule using MLFQ scheduling
 *
 * @return void
 */
void signal_mlfq(void)
{
	node *t;                          /* thread to schedule */

	t = find_mlfq();                  /* find the thread to schedule */
	last_tid = t->tinfo->id;
	pthread_cond_signal(&(t->cond));
}

/**
 * reset_counter - reset the counter for time quantum of the scheduled thread
 *
 * @param index: index of ready queue the scheduled thread was on previously
 * @return void
 */
void reset_counter(int index)
{
	if (index <= NUM_PRIO - 2) /* thread was not on the last queue, aged */
		counter = quantum[index + 1];
	else                       /* thread was on the last queue, still is */
		counter = quantum[index];
}

/**
 * mlfq_age - age a thread according to MLFQ scheduling
 *
 * @param index: index of ready queue to age its head
 * @return void
 */
void mlfq_age(int index)
{
	node *t;   /* thread to age */

	if (index <= NUM_PRIO - 2) {  /* thread not on last queue, age it */
		t = dequeue(ready[index]);
		enqueue(ready[index + 1], t);
	} else {                     /* thread on last queue, move to end */
		t = dequeue(ready[index]);
		enqueue(ready[index], t);
	}
}

/**
 * find_srt - find the thread with the shortest remaining run time 
 *            and earliest arrival time
 *
 * @param queue: ready queue to search
 * @return void: pointer to node with shortest remaining time, earliest arrival
 */
node *find_srt(pqueue_t *queue)
{
	node *min;   /* thread with shortest remaining time, earliest arrival */
	node *p;

	min = NULL;

	if (!is_empty(queue))
		min = queue->head;

	/* check each thread on the queue */
	for (p = queue->head; p != NULL; p = p->next) {
		/* 
		 * if this thread has a shorter remaining, 
		 * or same remaining time but earlier arrival time 
		 */
		if (p->tinfo->required_time < min->tinfo->required_time 
				|| (p->tinfo->required_time == min->tinfo->required_time && 
					p->current_time < min->current_time)) {
			min = p;  /* found new candidate thread */
		}
	}

	return min;
}

/**
 * signal_srt - signal thread with shortest remaining time, earliest arrival
 *
 * @param queue: queue to search
 * @return 0 thread successfully signaled, -1 signal failed
 */
int signal_srt(pqueue_t *queue)
{
	node *t;
	t = find_srt(queue);
	return pthread_cond_signal(&(t->cond));
}

/**
 * find_which_queue - find the ready queue that has a thread with id tid on it
 */
int find_which_queue(int tid) 
{
	int i; 

	for (i = 0; i < NUM_PRIO; i++) {            /* check each ready queue */
		if (is_member(ready[i], tid)) {
			return i; 
		}
	}

	return -1;
}

/**
 * schedule_fcfs - schedule the calling thread according to 
 *   First-Come-First-Served (FCFS) scheduling (no preemption)
 * @param current_time: time at which the thread called the function 
 * @param tid: thread identification number of the calling thread
 * @param remaining_time: amount of CPU time requested by the thread
 * @return the global current time
 */
int schedule_fcfs(float current_time, int tid, int remaining_time)
{
	pqueue_t *curr;   /* queue to use for scheduling */
	node *t;          /* general purpose pointer */
	int empty;        /* true if curr is empty when scheduling a thread */
	int ret;          /* scheduled time to return */

	ret = -1;
	curr = ready[0];

	pthread_mutex_lock(&m_globtime);
		globtime = current_time;    /* update global time */
	pthread_mutex_unlock(&m_globtime);

	pthread_mutex_lock(&m_sched);	
	/* queue is empty, append and schedule thread */
	if ((empty = is_empty(curr))) {     
		t = node_alloc(tid, remaining_time, current_time, 0);  /* bogus priority */
		enqueue(curr, t);
		ret = ceil(globtime);
	}
	pthread_mutex_unlock(&m_sched);	

	if (empty)
		return ret;

	pthread_mutex_lock(&m_sched);	
	if (remaining_time == 0) {        /* this thread is done running */
		t = pqueue_remove(curr, tid);  /* remove calling thread from queue */
		node_dealloc(t);
		ret = globtime;
		/* if there are threads still on the queue */
		if (!is_empty(curr)) {  
			pthread_cond_signal(&(curr->head->cond)); /* signal head */
		}
	}
	pthread_mutex_unlock(&m_sched);

	if (remaining_time == 0)
		return ret;

	pthread_mutex_lock(&m_sched);	
	if (!is_member(curr, tid)) {   /* if thread is not on the queue */
		t = node_alloc(tid, remaining_time, current_time, 0);  /* bogus priority */
		enqueue(curr, t);
	}
	pthread_mutex_unlock(&m_sched);	

	pthread_mutex_lock(&m_sched);	
	if (curr->head->tinfo->id == tid) {  /* if this thread at the head */
		ret = globtime;
	} else {
		pthread_cond_signal(&(curr->head->cond)); /* signal head */
		if ((t = queue_find(curr, tid)) != NULL) {  /* find index of this thread */
			pthread_cond_wait(&(t->cond), &m_sched); /* wait until signaled */
			ret = globtime;
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
	pqueue_t *curr;  /* queue to use for scheduling */
	node *t;         /* general purpose node pointer */
	int empty;       /* true if curr is empty when scheduling a thread */
	int ret;         /* scheduled time to return */

	ret = -1;
	curr = ready[0];

	pthread_mutex_lock(&m_globtime);
		globtime = current_time;   /* update global time */
	pthread_mutex_unlock(&m_globtime); 

	pthread_mutex_lock(&m_sched);
	if ((empty = is_empty(curr))) {   /* curr is empty  */
		t = node_alloc(tid, remaining_time, current_time, 0);  /* bogus priority */
		enqueue(curr, t); 
		ret = ceil(globtime);
	}
	pthread_mutex_unlock(&m_sched);

	if (empty)
		return ret;

	pthread_mutex_lock(&m_sched);
	if (remaining_time == 0) {             /* thread is finished with CPU */
		t = pqueue_remove(curr, tid);  /* remove thread from the queue */
		node_dealloc(t);
		if (!is_empty(curr)) {         /* signal thread with srt if there are threads still on the queue */
			signal_srt(curr);
		}
		ret = globtime;
	}
	pthread_mutex_unlock(&m_sched);

	if (remaining_time == 0) 
		return ret;

	pthread_mutex_lock(&m_sched);
	if (!is_member(curr, tid)) {   /* if thread not on the queue */
		t = node_alloc(tid, remaining_time, globtime, 0);  /* bogus priority */
		enqueue(curr, t);
		pthread_cond_wait(&(t->cond), &m_sched);
	} else {
		t = queue_find(curr, tid);
		t->current_time = current_time;
		t->tinfo->required_time = remaining_time; /* update remaining time for the calling thread */
	}
	pthread_mutex_unlock(&m_sched);

	pthread_mutex_lock(&m_sched);
	if (find_srt(curr)->tinfo->id == tid) {  /* if this thread is the one with the shortest remaining time */
		ret = globtime; 
	} else {
		signal_srt(curr);   /* signal thread with srt */ 
		t = queue_find(curr, tid);
		pthread_cond_wait(&(t->cond), &m_sched); /* wait until signaled */
		ret = globtime; 
	}
	pthread_mutex_unlock(&m_sched);

	return ret;
}

/**
 * schedule_pbs - schedule the calling thread according to 
 *   Priority-Based Scheduling (PBS) (with preemption)
 * @param tid: thread identification number of the calling thread
 * @param remaining_time: amount of CPU time requested by the thread
 * @param tprio: priority of the thread
 * @return the global current time
 */
int schedule_pbs(float current_time, int tid, int remaining_time, int tprio)
{
	pqueue_t *curr;  /* queue which alling thread is on  */
	node *t;         /* general purpose node pointer  */
	int empty;       /* true if curr is empty when scheduling a thread */
	int ret;         /* scheduled time to return */

	ret = -1;
	curr = ready[tprio - 1]; 

	pthread_mutex_lock(&m_globtime);
		globtime = current_time;    /* update global time */
	pthread_mutex_unlock(&m_globtime); 

	pthread_mutex_lock(&m_sched);
	if ((empty = are_queues_empty())) {  /* all ready queues are empty */
		t = node_alloc(tid, remaining_time, 0, tprio); 
		enqueue(curr, t);
		ret = ceil(globtime);
	}
	pthread_mutex_unlock(&m_sched);

	if (empty)
		return ret;

	pthread_mutex_lock(&m_sched);
	if (remaining_time == 0) {            /* thread finished with the CPU */
		t = pqueue_remove(curr, tid); /* remove calling thread from the queue */
		node_dealloc(t);
		ret = globtime;
		if (!are_queues_empty()) {    /* signal thread with hp if there are threads still on some queue */
			signal_hp();
		}
	}
	pthread_mutex_unlock(&m_sched);

	if (remaining_time == 0) 
		return ret;

	pthread_mutex_lock(&m_sched);
	if (!is_member(curr, tid)) {  /* if this thread is not on its queue */
		t = node_alloc(tid, remaining_time, 0, tprio); 
		enqueue(curr, t);
		pthread_cond_wait(&(t->cond), &m_sched); /* wait until next integral time */
	} else {                      /* update this thread's running time */
		t = queue_find(curr, tid);
		t->current_time = current_time;
		t->tinfo->required_time = remaining_time; 
	}
	pthread_mutex_unlock(&m_sched);


	pthread_mutex_lock(&m_sched);
	if (find_hp()->tinfo->id == tid) {  /* this thread is the one to be scheduled */
		ret = globtime;
	} else { 
		signal_hp();       /* signal the thread with highest priority */
		if ((t = queue_find(curr, tid)) != NULL) { 
			pthread_cond_wait(&(t->cond), &m_sched); 
			ret = globtime;
		}
	}
	pthread_mutex_unlock(&m_sched);

	return ret;
}

/**
 * schedule_mlfq - schedule the calling thread according to 
 *   Multi-Level Feedback Queue (MLFQ) scheduling (with preemption)
 * @param current_time: the time when the call to schedule_fcfs is made 
 *   by a calling thread
 * @param tid: thread identification number of the calling thread
 * @param remaining_time: amount of CPU time requested by the thread
 * @return the global current time
 */
int schedule_mlfq(float current_time, int tid, int remaining_time) 
{
	pqueue_t *curr;  
	node *t;              /* general purpose node pointer */
	int empty;            /*
			       * true if all queues are empty when 
			       * scheduling a thread 
			       */
	int index;            /* index of ready queue to use */
	int preempt;          /* true if preempting this thread */
	int last_is_running;  /* true if thread is still in its time quantum */
	int last_finished;    /* true if thread finished its time quantum */
	int new;              /* true if thread has just arrived */
	int ret;              /* scheduled time to return */

	ret = -1;
	new = 0;
	curr = NULL;

	pthread_mutex_lock(&m_globtime);
		globtime = current_time;   /* update global time */
	pthread_mutex_unlock(&m_globtime); 

	pthread_mutex_lock(&m_sched);
	if ((empty = are_queues_empty())) {           /* all queues are empty */
		t = node_alloc(tid, remaining_time, 0, 0); 
		enqueue(ready[0], t);
		ret = ceil(globtime);
		last_tid = tid;
		counter = TQ1 - 1;
	}
	pthread_mutex_unlock(&m_sched);

	if (empty)
		return ret;

	if ((index = find_which_queue(tid)) != -1)  /* find ready queue with this thread */
		curr = ready[index];

	pthread_mutex_lock(&m_sched);
	if (remaining_time == 0) {
		t = dequeue(curr);            /* remove calling thread from the queue */
		node_dealloc(t);
		ret = globtime;
		if (!are_queues_empty()) {    /* signal thread with hp if there are threads still on some queue */
			signal_mlfq();
		}
	}
	pthread_mutex_unlock(&m_sched);

	if (remaining_time == 0) 
		return ret;

	pthread_mutex_lock(&m_sched);
	if (curr == NULL) {                   /* this thread is just arriving */
		t = node_alloc(tid, remaining_time, 0, 0); 
		enqueue(ready[0], t);
		pthread_cond_wait(&(t->cond), &m_sched);
		reset_counter(0);
		ret = globtime;
		new = 1;
	} else {
		t = queue_find(curr, tid);
		t->tinfo->required_time = remaining_time; 
	}
	pthread_mutex_unlock(&m_sched);

	pthread_mutex_lock(&m_sched);
	index = find_which_queue(tid);
	curr = ready[index];

	/* 
	 * if we are the last process to run, but some other process 
	 * was introduced during our time quantum  
	 */
	if ((preempt = (last_tid == tid && counter >= 0 && ready[0]->head != NULL 
					&& ready[0]->head->tinfo->id != tid))) {
		t = queue_find(curr, tid);
		signal_mlfq();
		pthread_cond_wait(&(t->cond), &m_sched);
		ret = globtime;
		reset_counter(index - 1);
	}
	pthread_mutex_unlock(&m_sched);

	if (preempt)
		return ret;

	pthread_mutex_lock(&m_sched);
	index = find_which_queue(tid);
	curr = ready[index];

	/* 
	 * this is the last running thread, 
	 * but no higher threads have been introduced 
	 */
	if ((last_is_running = (last_tid == tid && counter > 0))) {
		if (!new) {
			ret = globtime;
		} else {
			counter = TQ1;
		}
		counter--;
	}
	pthread_mutex_unlock(&m_sched);

	if (last_is_running)
		return ret;

	pthread_mutex_lock(&m_sched);
	index = find_which_queue(tid);
	curr = ready[index];

	/* this thread is the last one to run, finished with its time quantum */
	if ((last_finished = (last_tid == tid && counter == 0))) {
		mlfq_age(index);                           /* age this thread */
		t = find_mlfq();
		/* this thread is not the next to run */
		if (t->tinfo->id != tid) {    
			signal_mlfq();
			curr = ready[find_which_queue(tid)];
			t = queue_find(curr, tid);
			pthread_cond_wait(&(t->cond), &m_sched);
		}
		ret = globtime;
		reset_counter(index);
		counter--;
	}
	pthread_mutex_unlock(&m_sched);
	
	if (last_finished)
		return ret;

	pthread_mutex_lock(&m_sched);
	index = find_which_queue(tid);
	curr = ready[index];

	t = find_mlfq();
	if (t->tinfo->id != tid) {
		signal_mlfq();
		pthread_cond_wait(&(t->cond), &m_sched);
	}
	ret = globtime;
	reset_counter(index);
	pthread_mutex_unlock(&m_sched);

	return ret;
}

/**
 * scheduleme - schedule the calling thread according to 
 *              the scheduling type being used
 *
 * @param time: time at which the calling thread calls the function 
 * @param tid: calling thread's id 
 * @param remaining_time: amount of CPU time requested by the thread
 * @param tprio: priority of the calling thread
 * @return time to schedule the calling thread
 */
int scheduleme(float time, int tid, int remaining_time, int tprio)
{
	switch(type) {
	case FCFS:
		return schedule_fcfs(time, tid, remaining_time);
		break;
	case SRTF: 
		return schedule_srtf(time, tid, remaining_time);
		break;
	case PBS:
		return schedule_pbs(time, tid, remaining_time, tprio);
		break;
	case MLFQ:
		return schedule_mlfq(time, tid, remaining_time);
		break;
	}

	return 0;
}

/**
 * init_scheduler - initialize ready queues, mutexes needed 
 *                  for the appropriate scheduling time
 * @param sched_type: indicator of which scheduling type to use
 * @return void
 */
void init_scheduler(int sched_type) 
{
	int i;

	type = sched_type;
	pthread_mutex_init(&m_sched, NULL);
	pthread_mutex_init(&m_globtime, NULL);

	switch(sched_type) {
	case FCFS:
		ready[0] = pqueue_allocate();
		break;
	case SRTF:
		ready[0] = pqueue_allocate();
		break;
	case PBS:
		for (i = 0; i < NUM_PRIO; i++) {
			ready[i] = pqueue_allocate();
		}

		break;
	case MLFQ:
		for (i = 0; i < NUM_PRIO; i++) {
			ready[i] = pqueue_allocate();
		}
		last_tid = 0;
		counter = 0;
		break;
	default:
		fprintf(stderr, "invalid scheduler type %d, defaulting to FCFS\n", sched_type);
		type = FCFS;
		ready[0] = pqueue_allocate();
		break;
	}
}

