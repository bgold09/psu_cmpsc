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

#define FCFS     0
#define SRTF     1
#define PBS      2
#define MLFQ     3
#define MAX_SIZE 16
#define NUM_PRIO 5
#define TQ1      5
#define TQ2      10
#define TQ3      15
#define TQ4      20
#define TQ5      25

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
	float current_time;
	pthread_cond_t cond;
};
typedef struct _node node_t; 

struct _thread_queue
{
	node_t *q[MAX_SIZE];
	int head;
	int tail;  /* tail points to element after the last element */
};
typedef struct _thread_queue pqueue_t;

struct _node_linked
{
	_thread_info_t *tinfo;
	float current_time;
	pthread_cond_t cond;
	struct _node_linked *next;
};
typedef struct _node_linked node;

struct _link_queue
{
	node *head;
	node *tail;
};
typedef struct _link_queue link_queue;

extern char *prog;
pqueue_t *ready;
link_queue *l_ready[NUM_PRIO];
static int type;
pthread_mutex_t m_sched;
pthread_mutex_t m_globtime;
pthread_mutex_t m_signal;
int last_tid;
int last_tid_prio;
int counter;
float globtime;
int quantum[] = {TQ1, TQ2, TQ3, TQ4, TQ5};

link_queue *link_queue_allocate(void) 
{
	link_queue *p;
	
	p = (link_queue *) malloc(sizeof(*p));
	p->head = NULL;
	p->tail = NULL;

	return p;
}

node *node_alloc(int tid, int remaining_time, float current_time, int priority)
{
	node *p;
	_thread_info_t *tinfo;

	if ((tinfo = (_thread_info_t *) malloc(sizeof(*tinfo))) == NULL) {
		return NULL;
	}

	if ((p = (node *) malloc(sizeof(*p))) == NULL) {
		return NULL;
	}

	tinfo->id = tid;
	tinfo->required_time = remaining_time;
	tinfo->arrival_time = globtime /* current_time */;
	/* printf("arrival_time for T%d: %f\n", tid, tinfo->arrival_time); */
	tinfo->priority = priority;

	p->tinfo = tinfo;
	p->current_time = current_time;
	p->next = NULL;
	pthread_cond_init(&(p)->cond, NULL);

	return p;
}

void node_dealloc(node *p) 
{
	free(p->tinfo);
	p->tinfo = NULL;
	pthread_cond_destroy(&(p->cond));
	free(p);
	p = NULL;
}

void l_enqueue(link_queue *queue, node *p)
{
	if (queue->head == NULL) {
		queue->head = queue->tail = p;
	} else {
		queue->tail->next = p;
		queue->tail = p;
	}
}

node *l_dequeue(link_queue *queue) 
{
	node *t;

	t = queue->head;
	queue->head = t->next;
	t->next = NULL;

	return t;
}


node *l_remove(link_queue *queue, int tid)
{
	node *p;
	node *ret;

	ret = NULL;

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

int l_is_empty(link_queue *queue) 
{
	return queue->head == NULL;
}

int l_are_queues_empty()
{
	int i;

	for (i = 0; i < NUM_PRIO; i++) {
		if (!l_is_empty(l_ready[i])) {
			return 0;
		}
	}

	return 1;
}

int l_is_member(link_queue *queue, int tid)
{
	node *p;

	for (p = queue->head; p != NULL; p = p->next) {
		if (p->tinfo->id == tid) {
			return 1;
		}
	}

	return 0;
}

node *l_find(link_queue *queue, int tid)
{
	node *p;

	for (p = queue->head; p != NULL; p = p->next) {
		if (p->tinfo->id == tid) {
			return p;
		}
	}

	return NULL;
}

node *l_find_hp(void) 
{
	node *high;
	node *p;
	link_queue *queue;
	int i;

	for (i = 0; i < NUM_PRIO && l_is_empty(l_ready[i]); i++);
	queue = l_ready[i];

	high = queue->head;

	for (p = queue->head->next; p != NULL; p = p->next) {
		if (p->current_time < high->current_time) {
			high = p;
		}
	}

	return high;
}

void l_signal_hp(void)
{
	node *t;

	t = l_find_hp();
	pthread_cond_signal(&(t->cond));
}

node *find_mlfq(void)
{
	int i;

	for (i = 0; i < NUM_PRIO && l_is_empty(l_ready[i]); i++);

	return l_ready[i]->head;
}

void signal_mlfq(void)
{
	node *t;

	t = find_mlfq();
	last_tid = t->tinfo->id;
	pthread_cond_signal(&(t->cond));
}

void reset_counter(int index)
{
	if (index <= NUM_PRIO - 2)
		counter = quantum[index + 1];
	else 
		counter = quantum[index];
}

void mlfq_age(int index)
{
	node *t;

	if (index <= NUM_PRIO - 2) {
		t = l_dequeue(l_ready[index]);
		l_enqueue(l_ready[index + 1], t);
	} else {
		t = l_dequeue(l_ready[index]);
		l_enqueue(l_ready[index], t);
	}
}

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

int are_queues_empty(void)
{
	int i;

	for (i = 0; i < NUM_PRIO; i++) {
		if (is_empty(&ready[i])) {
			return 0;
		}
	}

	return 1;
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
	queue->q[queue->tail] = t;
	queue->tail = (queue->tail + 1) % MAX_SIZE;
}

node_t *dequeue(pqueue_t *queue)
{
	node_t *ret;


	ret = queue->q[queue->head];
	queue->head = (queue->head + 1) % MAX_SIZE;

	return ret;
}

node_t *alloc_node(int tid, int remaining_time, float current_time, int priority)
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
	tinfo->arrival_time = globtime /* current_time */;
	tinfo->priority = priority;

	p->tinfo = tinfo;
	p->current_time = current_time;
	pthread_cond_init(&(p)->cond, NULL);

	return p;
}


int indexof(pqueue_t *queue, int tid)
{
	int i;
	int tail;

	tail = queue->tail;

	for (i = queue->head; i != tail; i = (i + 1) % MAX_SIZE) {
		if (queue->q[i]->tinfo->id == tid) {
			return i;
		}
	}

	return -1;
}


node_t *find_srt(pqueue_t *queue)
{
	node_t *min;
	node_t *p;
	int tail;
	int head;
	int i;


	tail = queue->tail;
	head = queue->head;
	min = NULL;

	if (head != tail)
		min = queue->q[head];

	for (i = head; i != tail; i = (i + 1) % MAX_SIZE) {
		p = queue->q[i];

		if (p->tinfo->required_time < min->tinfo->required_time 
				|| (p->tinfo->required_time == min->tinfo->required_time && 
					p->current_time < min->current_time)) {
			min = p;
		}
	}


	return min;
}

/**
 * signal_srt - 
 * @param queue:
 * @return success/failure return value from pthread_cond_signal
 */
int signal_srt(pqueue_t *queue)
{
	node_t *t;

	t = find_srt(queue);
	return pthread_cond_signal(&(t->cond));
}

node *l_find_srt(link_queue *queue)
{
	node *min;
	node *p;


	min = NULL;

	if (!l_is_empty(queue))
		min = queue->head;

	for (p = queue->head; p != NULL; p = p->next) {

		if (p->tinfo->required_time < min->tinfo->required_time 
				|| (p->tinfo->required_time == min->tinfo->required_time && 
					p->current_time < min->current_time)) {
			min = p;
		}
	}


	return min;
}

int l_signal_srt(link_queue *queue)
{
	node *t;
	t = l_find_srt(queue);
	return pthread_cond_signal(&(t->cond));
}

node *remove_by_tid(link_queue *queue, int tid)
{
	node *p;
	node *ret;


	ret = NULL;

	if (queue->head->tinfo->id == tid) {
		ret = queue->head;
		queue->head = queue->head->next;
		ret->next = NULL;
		return ret;
	}

	for (p = queue->head; p->next != NULL; p = p->next) {
		if (p->next->tinfo->id == tid) {
			ret = p->next;
			p->next = p->next->next;
			ret->next = NULL;
			return ret;
		}
	}


	return ret;
}


node_t *remove_by_index(pqueue_t *queue, int index)
{
	node_t *ret;
	int i;
	int tail;


	ret = queue->q[index];
	tail = queue->tail;

	for (i = index; i != tail; i = (i + 1) % MAX_SIZE) { /* shift elements to the right */
		queue->q[i] = queue->q[(i + 1) % MAX_SIZE];
	}

	queue->tail = (tail - 1) % MAX_SIZE;


	return ret;
}

int queue_find(int tid) 
{
	int i; 

	for (i = 0; i < NUM_PRIO; i++) {
		if (l_is_member(l_ready[i], tid)) {
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
int schedule_fcfs(float current_time, int tid, int remaining_time)
{
	link_queue *curr;
	node *t;
	int empty;
	int ret;

	ret = -1;
	curr = l_ready[0];

	pthread_mutex_lock(&m_globtime);
		globtime = current_time;
	pthread_mutex_unlock(&m_globtime);

	pthread_mutex_lock(&m_sched);	

	if ((empty = l_is_empty(curr))) {
		t = node_alloc(tid, remaining_time, current_time, 0);  /* bogus priority */
		l_enqueue(curr, t);
		ret = ceil(globtime);
	}

	pthread_mutex_unlock(&m_sched);	

	if (empty)
		return ret;

	pthread_mutex_lock(&m_sched);	

	if (remaining_time == 0) {
		t = l_remove(curr, tid);  /* remove calling thread from the queue */
		node_dealloc(t);
		ret = globtime;
		if (!l_is_empty(curr)) {      /* signal the head if there are threads still on the queue */
			pthread_cond_signal(&(curr->head->cond)); /* signal head */
		}
	}

	pthread_mutex_unlock(&m_sched);	

	if (remaining_time == 0)
		return ret;

	pthread_mutex_lock(&m_sched);	

	if (!l_is_member(curr, tid)) {   /* if thread not on the queue */
		t = node_alloc(tid, remaining_time, current_time, 0);  /* bogus priority */
		l_enqueue(curr, t);
	}

	pthread_mutex_unlock(&m_sched);	

	pthread_mutex_lock(&m_sched);	

	if (curr->head->tinfo->id == tid) {  /* if this thread at the head */
		ret = globtime;
	} else {
		pthread_cond_signal(&(curr->head->cond)); /* signal head */
		if ((t = l_find(curr, tid)) != NULL) {  /* find index of this thread */
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
	link_queue *curr;
	node *t;
	int empty;
	int ret;

	ret = -1;
	curr = l_ready[0];

	pthread_mutex_lock(&m_globtime);
		globtime = current_time; 
	pthread_mutex_unlock(&m_globtime); 

	pthread_mutex_lock(&m_sched);

	if ((empty = l_is_empty(curr))) {
		t = node_alloc(tid, remaining_time, current_time, 0);  /* bogus priority */
		l_enqueue(curr, t); 
		ret = ceil(globtime); 
	}

	pthread_mutex_unlock(&m_sched);

	if (empty)
		return ret;

	pthread_mutex_lock(&m_sched);

	if (remaining_time == 0) {
		t = l_remove(curr, tid);   /* remove calling thread from the queue */
		node_dealloc(t);
		if (!l_is_empty(curr)) {       /* signal thread with srt if there are threads still on the queue */
			l_signal_srt(curr);
		}
		ret = globtime;
	}

	pthread_mutex_unlock(&m_sched);

	if (remaining_time == 0) 
		return ret;

	pthread_mutex_lock(&m_sched);

	if (!l_is_member(curr, tid)) {   /* if thread not on the queue */
		t = node_alloc(tid, remaining_time, globtime, 0);  /* bogus priority */
		l_enqueue(curr, t);
		pthread_cond_wait(&(t->cond), &m_sched);
	} else {
		t = l_find(curr, tid);
		t->current_time = current_time;
		t->tinfo->required_time = remaining_time; /* update remaining time for the calling thread */
	}

	pthread_mutex_unlock(&m_sched);

	pthread_mutex_lock(&m_sched);

	if (l_find_srt(curr)->tinfo->id == tid) {  /* if this thread is the one with the shortest remaining time */
		ret = globtime; 
	} else {
		l_signal_srt(curr);   /* signal thread with srt */ 
		t = l_find(curr, tid);
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
	link_queue *curr;
	node *t;
	int empty;
	int ret;

	ret = -1;
	curr = l_ready[tprio - 1];

	pthread_mutex_lock(&m_globtime);
		globtime = current_time; 
	pthread_mutex_unlock(&m_globtime); 

	pthread_mutex_lock(&m_sched);
	if ((empty = l_are_queues_empty())) {
		t = node_alloc(tid, remaining_time, 0, tprio); 
		l_enqueue(curr, t);
		ret = ceil(globtime);
	}
	pthread_mutex_unlock(&m_sched);

	if (empty)
		return ret;

	pthread_mutex_lock(&m_sched);
	if (remaining_time == 0) {
		t = l_remove(curr, tid);    /* remove calling thread from the queue */
		node_dealloc(t);
		ret = globtime;
		if (!l_are_queues_empty()) {      /* signal thread with hp if there are threads still on some queue */
			l_signal_hp();
		}
	}
	pthread_mutex_unlock(&m_sched);

	if (remaining_time == 0) 
		return ret;

	pthread_mutex_lock(&m_sched);

	if (!l_is_member(curr, tid)) { 
		t = node_alloc(tid, remaining_time, 0, tprio); 
		l_enqueue(curr, t);
		pthread_cond_wait(&(t->cond), &m_sched);
	} else {
		t = l_find(curr, tid);
		t->current_time = current_time;
		t->tinfo->required_time = remaining_time; 
	}
	pthread_mutex_unlock(&m_sched);


	pthread_mutex_lock(&m_sched);

	if (l_find_hp()->tinfo->id == tid) {  
		ret = globtime;
	} else {
		l_signal_hp();    
		if ((t = l_find(curr, tid)) != NULL) { 
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
	link_queue *curr;
	node *t;
	int empty;
	int index;
	int preempt;
	int last_is_running;
	int last_finished;
	int new;
	int ret;

	ret = -1;
	new = 0;
	curr = NULL;

	pthread_mutex_lock(&m_globtime);
		globtime = current_time; 
	pthread_mutex_unlock(&m_globtime); 

	pthread_mutex_lock(&m_sched);

	if ((empty = l_are_queues_empty())) {
		t = node_alloc(tid, remaining_time, 0, 0); 
		l_enqueue(l_ready[0], t);
		ret = ceil(globtime);
		last_tid = tid;
		counter = TQ1 - 1;
		last_tid_prio = 1;
	}
	pthread_mutex_unlock(&m_sched);

	if (empty)
		return ret;

	if ((index = queue_find(tid)) != -1)
		curr = l_ready[index];

	pthread_mutex_lock(&m_sched);
	if (remaining_time == 0) {
		t = l_dequeue(curr);            /* remove calling thread from the queue */
		node_dealloc(t);
		ret = globtime;
		if (!l_are_queues_empty()) {    /* signal thread with hp if there are threads still on some queue */
			signal_mlfq();
		}
	}
	pthread_mutex_unlock(&m_sched);

	if (remaining_time == 0) 
		return ret;

	pthread_mutex_lock(&m_sched);

	if (curr == NULL) { 
		t = node_alloc(tid, remaining_time, 0, 0); 
		l_enqueue(l_ready[0], t);
		pthread_cond_wait(&(t->cond), &m_sched);
		reset_counter(0);
		ret = globtime;
		new = 1;
	} else {
		t = l_find(curr, tid);
		t->tinfo->required_time = remaining_time; 
	}
	pthread_mutex_unlock(&m_sched);


	pthread_mutex_lock(&m_sched);

	index = queue_find(tid);
	curr = l_ready[index];

	/* 
	 * if we are the last process to run, but some other process 
	 * was introduced during our time quantum  
	 */
	if ((preempt = (last_tid == tid && counter >= 0 && l_ready[0]->head != NULL 
					&& l_ready[0]->head->tinfo->id != tid))) {
		t = l_find(curr, tid);
		signal_mlfq();
		pthread_cond_wait(&(t->cond), &m_sched);
		ret = globtime;
		reset_counter(index - 1);
	}

	pthread_mutex_unlock(&m_sched);

	if (preempt)
		return ret;

	pthread_mutex_lock(&m_sched);

	index = queue_find(tid);
	curr = l_ready[index];

	/* this is the last running thread, but no higher threads have been introduced */
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

	index = queue_find(tid);
	curr = l_ready[index];

	if ((last_finished = (last_tid == tid && counter == 0))) {
		mlfq_age(index);
		t = find_mlfq();
		if (t->tinfo->id != tid) {
			signal_mlfq();
			curr = l_ready[queue_find(tid)];
			t = l_find(curr, tid);
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

	index = queue_find(tid);
	curr = l_ready[index];

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

int scheduleme(float time, int tid, int remaining_time, int tprio)
{
	int ret;

	ret = 0;

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

	return ret;   /* FIX THIS */
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
	pthread_mutex_init(&m_sched, NULL);
	pthread_mutex_init(&m_globtime, NULL);
	pthread_mutex_init(&m_signal, NULL);

	switch(sched_type) {
	case FCFS:
		l_ready[0] = link_queue_allocate();
		break;
	case SRTF:
		if ((ready = (pqueue_t *) malloc(sizeof(*ready))) == NULL) {
			/* fprintf  */
			return;  /* how should we return? */
		}
		if (init_pqueue(&ready[0]) == -1) {
			return;
		}

		l_ready[0] = link_queue_allocate();

		break;
	case PBS:
		for (i = 0; i < NUM_PRIO; i++) {
			l_ready[i] = link_queue_allocate();
		}

		break;
	case MLFQ:
		for (i = 0; i < NUM_PRIO; i++) {
			l_ready[i] = link_queue_allocate();
		}
		last_tid = 0;
		counter = 0;
		last_tid_prio = 0;
		break;
	default:
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

