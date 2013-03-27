/**
 * @file my_memory.c
 * @brief implement memory allocation/deallocation schemes using
 *        First Fit, Best Fit, Worst Fit and Buddy System
 * @date 04/01/13
 * @author Mike Fitz, Brian Golden
 *
 */

#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>     /* malloc(), free() */
#include <math.h>       /* log2() */
#include "my_memory.h"

static int type;          /* memory allocation scheme to use */
static int num_buckets;   /* number of block orders needed for Buddy System */
static void *base_addr;   /* starting address of `RAM' */
free_list_t **free_list;  /* free_lists for allocation */
static int arch;          /*
			   * architecture of this machine (32 or 64-bit), 
			   * needed for finding buddies
			   */

void *my_malloc(int size)
{
	block_t *block;  /* pointer to a free block */
	void *ret;       /* pointer to newly allocated segment on the heap */

	/* find block to use for allocation and remove it from the free list */
	if ((block = find_and_remove_block(size + HEADER_SIZE)) == NULL) {
		return (void *) -1;
	}
	
	ret = block->addr;
	if (type != BS)
		*((int *) ret) = size;  /* add size of new block to its header */
	else
		*((int *) ret) = (int) block->size;

	/* split the block, if necessary */
	if (type != BS) {
		if (size + HEADER_SIZE == block->size) {
			block_deallocate(block);
		} else {
			block->size -= size + HEADER_SIZE;
			block->addr += size + HEADER_SIZE;
			/* readd the block to the correct free_list */
			free_list_enqueue(block);
		}
	}

	return ret + HEADER_SIZE;
}

block_t *find_and_remove_block(int size)
{
	switch(type) {
	case FF:
		return find_and_remove_block_ff(size);
		break;
	case BF:
		return find_and_remove_block_bf(size);
		break;
	case WF:
		return find_and_remove_block_wf(size);
		break;
	case BS:
		return find_and_remove_block_bs(size);
		break;
	default:
		/* default to First Fit allocation */
		return find_and_remove_block_ff(size);
		break;
	}
}

block_t *free_list_remove(free_list_t *list, block_t *block)
{
	block_t *p;  /* pointer to free block to remove from list */

	p = NULL;

	/* one element list, assuming non-null */
	if (list->head == list->tail) { 
		p = list->head;
		list->head = NULL;
		list->tail = NULL;
	} else if (list->head == block) {
		p = list->head;
		list->head = p->next;
		list->head->prev = NULL;
		p->next = NULL;
		p->prev = NULL;
	} else if (list->tail == block) {
		p = list->tail;
		list->tail = p->prev;
		list->tail->next = NULL;
		p->next = NULL;
		p->prev = NULL;
	} else {	
		p = block;
		p->prev->next = p->next;
		p->next->prev = p->prev;
		p->next = NULL;
		p->prev = NULL;
	}

	return p;
}

block_t *find_and_remove_block_ff(int size)
{
	free_list_t *list;  /* free list to remove from */
	block_t *p;         /* block removed from list */

	list = free_list[0];

	if (list->head->size >= size) {
		return free_list_remove(list, list->head);
	} 

	for (p = list->head->next; p != list->tail; p = p->next) {
		if (p->size >= size) {
			return free_list_remove(list, list->head);
		}
	}

	if (list->tail->size >= size) {
		return free_list_remove(list, list->tail);
	}
	
	return NULL;
}

block_t *find_and_remove_block_bf(int size)
{
	free_list_t *list;  /* free list to remove from */
	block_t *best;      /* best fit block for allocation */
	block_t *p;         /* temp block */
	int bestsize;       /* size of the current best fit block */

	list = free_list[0];
	bestsize = MAXRAMSIZE + 1;
	best = NULL;

	for (p = list->head; p != NULL; p = p->next) {
		if (p->size >= size && p->size < bestsize) {
			best = p;
			bestsize = p->size;			
		}
	}

	return best != NULL ? free_list_remove(list, best) : NULL;
}

block_t *find_and_remove_block_wf(int size)
{
	free_list_t *list;  /* free list to remove from */
	block_t *worst;     /* worst fit block for allocation */
	block_t *p;         /* temp block */
	int worstsize;      /* size of the current worst fit block */

	list = free_list[0];
	worstsize = -1;
	worst = NULL;

	for (p = list->head; p != NULL; p = p->next) {
		if (p->size >= size && p->size > worstsize) {
			worst = p;
			worstsize = p->size;			
		}
	}

	return worst != NULL ? free_list_remove(list, worst) : NULL;
}

block_t *find_and_remove_block_bs(int size)
{
	block_t *old;  /* block moved from one list to another */
	block_t *new;  /* newly allocated block, split from old */
	int index;     /* index of smallest free list large enough */
	int i;

	index = (int) ceilf(log2f((float) ((size + HEADER_SIZE) / MIN_REQUEST)));

	if (free_list_is_empty(free_list[index])) { /* no block at this order */
		for (i = index + 1; 
		     i < num_buckets && free_list_is_empty(free_list[i]);
		     i++);  /* find the first non-empty list for allocation */

		if (i == num_buckets) {  /* no block large enough was found */
			return NULL;
		} else {
			for (; i > index; i--) {  /* split blocks as necessary */
				old = free_list_remove(free_list[i], free_list[i]->head);
				old->size >>= 1;    /* divide by 2 */
				free_list_enqueue(old);
				/* allocate new free block adjacent to old */
				new = block_allocate(old->addr + old->size, old->size); 
				/* new = block_allocate((void *) ((unsigned long) old->addr ^ (unsigned long) old->size), old->size); */
				free_list_enqueue(new);
			}
		}
	}

	return free_list_remove(free_list[index], free_list[index]->head);
}

void my_free(void *ptr)
{
	int size;
	block_t *block;
	free_list_t *list;

	size = *((int *) (ptr - HEADER_SIZE));
	if (type != BS)
		block = block_allocate(ptr - HEADER_SIZE, size + HEADER_SIZE);
	else
		block = block_allocate(ptr - HEADER_SIZE, size);
	list = free_list_enqueue(block);   /* add to correct free_list */
	coalesce(list, block);
}

void coalesce(free_list_t *list, block_t *ptr)
{
	void *buddy;
	block_t *left;
	block_t *right;
	block_t *new;
	int buddy_num;
	int done;
	int i;

	if (type != BS) {
		if (ptr->next != NULL) {  /* not at tail of list */
			if (ptr->addr + ptr->size == ptr->next->addr) {
				ptr->size += ptr->next->size;
				free_list_remove(list, ptr->next); 
			}
		} 

		if (ptr->prev != NULL) {  /* not at head of the list */
			if (ptr->prev->addr + ptr->prev->size == ptr->addr) {
				ptr->prev->size += ptr->size;
				free_list_remove(list, ptr);
			}
		}
	} else {
		done = 0;
		i = (int) ceilf(log2f((float) (ptr->size) / MIN_REQUEST));
		new = ptr;
		while(!done) {  /* while still coalescing to do */
			/* do we need to conditionally cast based on the architecture / how it was compiled */
			buddy_num = (new->addr - base_addr) / new->size;

			if (buddy_num % 2 == 0) {  /* even buddy number */
				buddy = new->addr + new->size;
			} else {                   /* odd buddy number */
				buddy = new->addr - new->size;
			}

			/* buddy = (void *) ((unsigned long) new->addr ^ (unsigned long) new->size); */
			
			if (new->prev != NULL && new->prev->addr == buddy) {
				/* buddy is to the left */
				left = free_list_remove(free_list[i], new->prev);
				right = free_list_remove(free_list[i], new);
				block_deallocate(right);
				left->size <<= 1;  /* multiply by 2 */
				free_list_enqueue(left);
				new = left;
			} else if (new->next != NULL && new->next->addr == buddy) {
				right = free_list_remove(free_list[i], new->next);
				left = free_list_remove(free_list[i], new);
				right->addr = left->addr; 
				block_deallocate(left);
				right->size <<= 1;  /* multiply by 2 */
				free_list_enqueue(right);
				new = right;
			} else {
				done = 1;
			}
			i++;
		}
	}
}

void setup(int malloc_type, int mem_size, void *start_of_memory)
{
	int i;

	type = malloc_type;
	base_addr = start_of_memory;
	num_buckets = (int) log2f((float) (mem_size / MIN_REQUEST)) + 1;
	arch = (size_t) - 1 > 0xffffffffUL;  /* determine the architecture */

	if (malloc_type != BS) {
		free_list = (free_list_t **) malloc(sizeof(*free_list) * num_buckets);
		free_list[0] = free_list_allocate();
		free_list[0]->head = block_allocate(start_of_memory, mem_size);
		free_list[0]->tail = free_list[0]->head;
	} else {
		free_list = (free_list_t **) malloc(sizeof(*free_list) * num_buckets);
		for (i = 0; i < num_buckets; i++) {
			free_list[i] = free_list_allocate();
		}
		free_list[num_buckets - 1]->head = 
			block_allocate(start_of_memory, mem_size);
		free_list[num_buckets - 1]->tail = 
			free_list[num_buckets - 1]->head;
	}
}

block_t *block_allocate(void *addr, int size)
{
	block_t *p;   /* pointer to new free block to allocate */

	if ((p = malloc(sizeof(*p))) == NULL) {
		fprintf(stderr, "malloc() failed\n");
	} else {
		p->addr = addr;
		p->size = (size_t) size;
		p->prev = NULL;
		p->next = NULL;
	}

	return p;
}

void block_deallocate(block_t *p)
{
	free(p);
	p = NULL;
}

free_list_t *free_list_allocate(void)
{
	free_list_t *p;

	if ((p = (free_list_t *) malloc(sizeof(*p))) == NULL) {
		fprintf(stderr, "malloc failed, free_list_allocate()\n");
	}

	return p;
}

free_list_t *free_list_enqueue(block_t *block)
{
	free_list_t *list;
	block_t *p;

	if (type != BS)
		list = free_list[0];
	else 
		list = free_list[(int) ceilf(log2f((float) (block->size / MIN_REQUEST)))];

	if (list->head == NULL) {    /* empty list */
		block->next = NULL;
		block->prev = NULL;
		list->head = block;
		list->tail = block;
		return list;
	} else if (list->head->addr > block->addr) {  
		/* insert before head */
		p = list->head;
		list->head = block;
		list->head->next = p;
		list->head->prev = NULL;
		p->prev = list->head;
		return list;
	} else if (list->head == list->tail) {  
		/* single element list, insert at tail */
		list->head->next = block;
		list->tail = block;
		block->prev = list->head;
		block->next = NULL;
		return list;
	}

	for (p = list->head->next; 
	     p->addr < block->addr && p->next != NULL; p = p->next);

	if (p->addr >= block->addr && p->next == NULL) {   /* not enqueuing at the tail */
		p->prev->next = block;
		block->prev = p->prev;
		block->next = p;
		p->prev = block;
	} else if (p->next != NULL) {  /* not enqueuing at the tail */
		p->prev->next = block;
		block->prev = p->prev;
		block->next = p;
		p->prev = block;
	} else {                /* inserting after p, new tail */
		p->next = block;
		block->prev = p;
		block->next = NULL;
		list->tail = block;
	}

	return list;
}

int free_list_is_empty(free_list_t *list)
{
	return list->head == NULL;
}

int get_bucket_index(int size)
{
	return (int) log2f((float) size);
}
