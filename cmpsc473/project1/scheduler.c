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
#define MAX_SIZE 16
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
pthread_mutex_t m_queue;
pthread_mutex_t m_sched;
pthread_mutex_t m_globtime;
pthread_mutex_t m_signal;
pthread_cond_t cond_time;
float globtime;


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
	free(p);
	p = NULL;
}

void l_enqueue(link_queue *queue, node *p)
{
	pthread_mutex_lock(&m_queue);

	if (queue->head == NULL) {
		queue->head = queue->tail = p;
	} else {
		queue->tail->next = p;
		queue->tail = p;
	}

	pthread_mutex_unlock(&m_queue);
}


node *l_remove(link_queue *queue, int tid)
{
	node *p;
	node *ret;

	pthread_mutex_lock(&m_queue);

	ret = NULL;

	if (queue->head->tinfo->id == tid) {
		ret = queue->head;
		queue->head = queue->head->next;
	} else {
		for (p = queue->head->next; p->next != NULL; p = p->next) {
			if (p->next->tinfo->id == tid) {
				ret = p->next;
				p->next = p->next->next;
				break;
			}
		}
	}

	pthread_mutex_unlock(&m_queue);

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

node *l_find_hp() 
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

void l_signal_hp(int tid)
{
	node *t;

	(void) tid;
	t = l_find_hp();
	/*printf("\t\tsignal T%d with prio = %d\n", t->tinfo->id, t->tinfo->priority);*/
	pthread_cond_signal(&(t->cond));
	/*sem_post(&(t->sem));*/
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
	pthread_mutex_lock(&m_queue);
	queue->q[queue->tail] = t;
	queue->tail = (queue->tail + 1) % MAX_SIZE;
	pthread_mutex_unlock(&m_queue);
}

node_t *dequeue(pqueue_t *queue)
{
	node_t *ret;

	pthread_mutex_lock(&m_queue);

	ret = queue->q[queue->head];
	queue->head = (queue->head + 1) % MAX_SIZE;
	pthread_mutex_unlock(&m_queue);

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
	/* printf("arrival_time for T%d: %f\n", tid, tinfo->arrival_time);*/
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


node_t *find_srt(pqueue_t *queue, int tid)
{
	node_t *min;
	node_t *p;
	int tail;
	int head;
	/* int integral; */
	int i;

	(void) tid;

	pthread_mutex_lock(&m_queue);

	tail = queue->tail;
	head = queue->head;
	/*  integral = (globtime == ceil(globtime)); */
	min = NULL;

	/* if (integral) {
		if (head != tail) {
			min = queue->q[head];
		}
	} else {
		if (head != tail && queue->q[head]->tinfo->id != tid) {
			min = queue->q[head];
		} 
	} */

	if (head != tail)
		min = queue->q[head];

	for (i = head; i != tail; i = (i + 1) % MAX_SIZE) {
		p = queue->q[i];

		/*if (integral) {
			if (p->tinfo->required_time < min->tinfo->required_time 
				|| (p->tinfo->required_time == min->tinfo->required_time && 
					p->current_time > min->current_time)) {

				min = p;
			}
		} else { 
			if ((min == NULL && p->tinfo->id != tid) || 
				(p->tinfo->required_time < min->tinfo->required_time && tid != p->tinfo->id) ||
				(p->tinfo->required_time == min->tinfo->required_time && 
					p->current_time > min->current_time &&
					tid != p->tinfo->id)) {
				min = p;
			}
		} */

		if (p->tinfo->required_time < min->tinfo->required_time 
				|| (p->tinfo->required_time == min->tinfo->required_time && 
					p->current_time > min->current_time)) {
			min = p;
		}

	}

	pthread_mutex_unlock(&m_queue);

	return min;
}

node_t *find_hp(int tid)
{
	node_t *hp;
	node_t *curr;
	pqueue_t *queue;
	int tail;
	int head;
	int i;

	(void) tid;

	pthread_mutex_lock(&m_queue);

	hp = NULL;

	for (i = 0; i < NUM_PRIO && is_empty(&ready[i]); i++);
	queue = &ready[i];

	tail = queue->tail;
	head = queue->head;
	
	if (head == tail) {
		pthread_mutex_unlock(&m_queue);
		return NULL;
	}

	hp = queue->q[head];

	for (i = (head + 1) % MAX_SIZE; i != tail; i = (i + 1) % MAX_SIZE) {
		curr = queue->q[i];

		if (curr->current_time < hp->current_time) {
			hp = curr;
		}
	}

	pthread_mutex_unlock(&m_queue);

	return hp;
}

int signal_hp(int tid)
{
	node_t *t;

	if ((t = find_hp(tid)) == NULL) {
		fprintf(stderr, "find_hp failed, call from T%d\n", tid);
		return 0;
	}
	pthread_cond_signal(&(t->cond));
	return 0;
}

int round_robin(pqueue_t *queue)
{
	(void) queue;
	return 0;
}

/**
 * signal_srt - 
 * @param queue:
 * @return success/failure return value from pthread_cond_signal
 */
int signal_srt(pqueue_t *queue, int tid)
{
	node_t *t;

	t = find_srt(queue, tid);
	/* printf("\t\tsignal T%d from T%d\n", t->tinfo->id, tid); */
	return pthread_cond_signal(&(t->cond));
}

node_t *remove_by_index(pqueue_t *queue, int index)
{
	node_t *ret;
	int i;
	int tail;

	pthread_mutex_lock(&m_queue);

	ret = queue->q[index];
	tail = queue->tail;

	for (i = index; i != tail; i = (i + 1) % MAX_SIZE) { /* shift elements to the right */
		queue->q[i] = queue->q[(i + 1) % MAX_SIZE];
	}

	queue->tail = (tail - 1) % MAX_SIZE;

	pthread_mutex_unlock(&m_queue);

	return ret;
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
	pqueue_t *curr;
	node_t *t;
	int index;
	int empty;
	int ret;

	ret = -1;
	curr = &ready[0];

	pthread_mutex_lock(&m_globtime);
		globtime = current_time;
	pthread_mutex_unlock(&m_globtime);

	/* pthtread_cond_signal(&cond_time);  */

	pthread_mutex_lock(&m_sched);	

	if ((empty = is_empty(curr))) {
		t = alloc_node(tid, remaining_time, current_time, 0);  /* bogus priority */
		enqueue(curr, t);
		ret = ceil(globtime);
	}

	pthread_mutex_unlock(&m_sched);	

	if (empty)
		return ret;

	pthread_mutex_lock(&m_sched);	

	if (remaining_time == 0) {
		t = dequeue(curr);  /* remove calling thread from the queue */
		dealloc_node(t);
		if (!is_empty(curr)) {      /* signal the head if there are threads still on the queue */
			pthread_cond_signal(&(curr->q[curr->head]->cond)); /* signal head */
		}
		ret = globtime;
	}

	pthread_mutex_unlock(&m_sched);	

	if (remaining_time == 0)
		return ret;

	pthread_mutex_lock(&m_sched);	

	if (!is_member(curr, tid)) {   /* if thread not on the queue */
		t = alloc_node(tid, remaining_time, current_time, 0);  /* bogus priority */
		enqueue(curr, t);
	}

	pthread_mutex_unlock(&m_sched);	

	pthread_mutex_lock(&m_sched);	

	if (curr->q[curr->head]->tinfo->id == tid) {  /* if this thread at the head */
		ret = globtime;
	} else {
		pthread_cond_signal(&(curr->q[curr->head]->cond)); /* signal head */
		if ((index = indexof(curr, tid)) != -1) {  /* find index of this thread */
			pthread_cond_wait(&(curr->q[index]->cond), &m_sched); /* wait until signaled */
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
	pqueue_t *curr;
	node_t *t;
	int index;
	int ret;

	ret = -1;
	curr = &ready[0];

	pthread_mutex_lock(&m_globtime);
		globtime = current_time; 
	pthread_mutex_unlock(&m_globtime); 

	pthread_cond_signal(&cond_time);

	if (is_empty(curr)) {
		t = alloc_node(tid, remaining_time, current_time, 0);  /* bogus priority */
		enqueue(curr, t); 
		ret = ceil(globtime); 
	} else if (remaining_time == 0) {   /* thread is finished with the CPU */
		t = remove_by_index(curr, indexof(curr, tid));   /* remove calling thread from the queue */
		dealloc_node(t);
		if (!is_empty(curr)) {      /* signal thread with srt if there are threads still on the queue */
			signal_srt(curr, tid);
		}
		ret = globtime;  
	} else {
		if (!is_member(curr, tid)) {   /* if thread not on the queue */
			if (current_time != ceil(current_time)) {
				pthread_cond_wait(&cond_time, &m_sched);
			}

			t = alloc_node(tid, remaining_time, globtime, 0);  /* bogus priority */
			enqueue(curr, t);
		} else {
			t = curr->q[indexof(curr, tid)];
			t->current_time = current_time;
			t->tinfo->required_time = remaining_time; /* update remaining time for the calling thread */
		}

		if (find_srt(curr, tid)->tinfo->id == tid) {  /* if this thread is the one with the shortest remaining time */
			ret = globtime; 
		} else {
			pthread_mutex_lock(&m_signal);
			signal_srt(curr, tid);   /* signal thread with srt */ 
			pthread_mutex_unlock(&m_signal);
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

	pthread_cond_signal(&cond_time);
	
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
		pthread_mutex_lock(&m_queue);
		if (!l_are_queues_empty()) {      /* signal thread with hp if there are threads still on some queue */
			l_signal_hp(tid);
		}
		pthread_mutex_unlock(&m_queue);
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

	if (l_find_hp(tid)->tinfo->id == tid) {  
		ret = globtime;
	} else {
		pthread_mutex_lock(&m_queue);
		l_signal_hp(tid);    
		pthread_mutex_unlock(&m_queue);
		if ((t = l_find(curr, tid)) != NULL) { 
			pthread_cond_wait(&(t->cond), &m_sched); 
			ret = globtime;
		}
	}

	pthread_mutex_unlock(&m_sched);

	return ret;
}

/**
 * schedule_fcfs - schedule the calling thread according to 
 *   Multi-Level Feedback Queue (MLFQ) scheduling (with preemption)
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
	int ret;

/*	pthread_mutex_lock(&m_sched);
	pthread_mutex_lock(&m_globtime);*/
		/*globtime = time; *//* hack - will this work? */
	/* pthread_mutex_unlock(&m_globtime); */
	/* pthread_mutex_unlock(&m_sched);*/


	/* if (time != ceil(time))
		pthread_cond_wait(&cond_time, &m_sched);
	else 
		pthread_cond_signal(&cond_time); */
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
		return schedule_mlfq(time, tid, remaining_time, tprio);
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
	pthread_mutex_init(&m_queue, NULL);
	pthread_mutex_init(&m_sched, NULL);
	pthread_mutex_init(&m_globtime, NULL);
	pthread_mutex_init(&m_signal, NULL);
	pthread_cond_init(&cond_time, NULL);

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
		/*if ((ready = (pqueue_t *) malloc(sizeof(*ready) * NUM_PRIO)) == NULL) {
			return; 
		}

		for (i = 0; i < NUM_PRIO; i++) {
			if (init_pqueue(&ready[i]) == -1) {
				return;
			}
		}*/
		/*if ((l_ready = (link_queue *) malloc(sizeof(*l_ready) * NUM_PRIO)) == NULL) {
			return;
		}*/

		for (i = 0; i < NUM_PRIO; i++) {
			l_ready[i] = link_queue_allocate();
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

