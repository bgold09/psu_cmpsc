/**
 * @file my_memory.h
 * @brief prototypes for memory allocation/deallocation schemes
 * @date 04/01/13
 * @author Mike Fitz, Brian Golden
 *
 */

#define FF          0       /* First Fit */
#define BF          1       /* Best Fit */
#define WF          2       /* Worst Fit */
#define BS          3       /* Buddy System */
#define HEADER_SIZE 4       /* size of size header for an allocated block */
#define MAXRAMSIZE  1048576 /* max starting RAM size */

/**
 * struct _free_block - information about a free segment of heap memory
 *
 */
struct _free_block
{
	void *addr;                /* starting address of this memory segment */
	int size;                  /* size of this memory segment */
	struct _free_block *next;  /* pointer to next block in free list */
	struct _free_block *prev;  /* pointer to previous block in free list */
};
typedef struct _free_block block_t;

/*
 * struct _free_list - doubly-linked list of free blocks
 *
 */
struct _free_list
{
	block_t *head;  /* head of the list, NULL indicates empty list */
	block_t *tail;  /* tail of the list */
};
typedef struct _free_list free_list_t;

/**
 * my_malloc - allocates memory segments from start_of_memory 
 *
 * @param size: size in bytes of memory to allocate
 * @return pointer to newly allocated memory on success, NULL failure
 */
void *my_malloc(int size);

/**
 * my_free - dellocates a memory segment 
 *
 * @param ptr: pointer to the memory segment to deallocate
 * @return void
 */
void my_free(void *ptr);

/**
 * setup - 
 *
 * @param malloc_type: indicator of which memory allocation type to use
 * @param mem_size: total memory space available in the system (initially 1 MB)
 * @param start_of_memory: pointer to start of memory segement used for 
 *                         allocation and deallocation
 * @return void
 */
void setup(int malloc_type, int mem_size, void *start_of_memory);

/**
 * block_allocate - allocate a new block node for the free_list
 *
 * @param addr: starting address for the new free block
 * @param size: size of the new free block
 * @return pointer to allocated block_t
 */
block_t *block_allocate(void *addr, int size);

/**
 * block_deallocate - free the memory allocated to a node
 *
 * @param p: pointer to block info to deallocate
 * @return void
 */
void block_deallocate(block_t *p);

/**
 * free_list_remove - remove a block from a free list
 *
 * @param list: list to remove from
 * @param block: block to remove from the list
 * @return pointer to the removed block
 */
block_t *free_list_remove(free_list_t *list, block_t *block);

/**
 * find_and_remove_block_ff - find a block on the free list using First Fit 
 *                            allocation and remove it
 *
 * @param size: size of memory segment to allocate
 * @return pointer to block to use for allocation on success, 
 *         NULL no block large enough was found
 */
block_t *find_and_remove_block_ff(int size);

/**
 * find_and_remove_block_bf - find a block on the free list using Best Fit
 *                            allocation and remove it
 *
 * @param size: size of memory segment to allocate
 * @return pointer to block to use for allocation on success, 
 *         NULL no block large enough was found
 */
block_t *find_and_remove_block_bf(int size);

/**
 * find_and_remove_block_wf - find a block on the free list using Worst Fit 
 *                            allocation and remove it
 *
 * @param size: size of memory segment to allocate
 * @return pointer to block to use for allocation on success, 
 *         NULL no block large enough was found
 */
block_t *find_and_remove_block_wf(int size);

/**
 * find_and_remove_block_bs - find a block on the free list using Buddy System 
 *                            allocation and remove it
 *
 * @param size: size of memory segment to allocate
 * @return pointer to block to use for allocation on success, 
 *         NULL no block large enough was found
 */
block_t *find_and_remove_block_bs(int size);

/**
 * find_and_remove_block - find a block on the free list to use for 
 *                         allocation and remove it
 *
 * @param size: size of memory segment to allocate
 * @return pointer to block to use for allocation on success, 
 *         NULL no block large enough was found
 */
block_t *find_and_remove_block(int size);


/**
 * coalesce - merge free blocks adjacent to a newly freed block
 *
 * @param list: free list to coalesce in
 * @param ptr: pointer to newly added free block to the free_list
 * @return void
 */
void coalesce(free_list_t *list, block_t *ptr);

/**
 * free_list_allocate - allocate memory for a free list
 * 
 * @return pointer to new free_list
 */
free_list_t *free_list_allocate(void);

/**
 * free_list_enqueue - enqueue a new block to a free list
 *
 * @param block: new block to enqueue on the list
 * @return pointer to free list the block was added to
 */
free_list_t *free_list_enqueue(block_t *block);
