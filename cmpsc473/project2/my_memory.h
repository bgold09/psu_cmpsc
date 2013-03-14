/**
 * @file my_memory.h
 * @brief prototypes for memory allocation/deallocation schemes
 * @date 04/01/13
 * @author Mike Fitz, Brian Golden
 *
 */

#define FF  0  /* First Fit */
#define BF  1  /* Best Fit */
#define WF  2  /* Worst Fit */
#define BS  3  /* Buddy System */

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
