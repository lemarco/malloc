#ifndef MALLOC_H
# define MALLOC_H

# include <unistd.h>
# include <sys/mman.h>
# include <pthread.h>
#include <stdbool.h>


#define MAX_SIZE (~(size_t)0)

/********************************************/
/* define of sizes of memory allocation.    */ 
/* AREA defines are for choising chunk size */
/* SIZE defines max size of chunk alloc     */
/********************************************/
# define PAGE getpagesize()
# define TINY_AREA (size_t)(64 * PAGE)
# define TINY_ALLOC 2048

# define SMALL_AREA (size_t)(256 * PAGE)
# define SMALL_ALLOC 8192
/********************************************/
/********************************************/
/********************************************/
/* PROT definition of protection flags 		*/
/* MAP non shariable displaying		        */
/* SIZE defines max size of chunk alloc     */
/********************************************/
# define FLAGS (MAP_ANON | MAP_PRIVATE)
# define PROTECT (PROT_READ | PROT_WRITE)
/********************************************/

# define CHUNK_SZ sizeof(t_chunk)
# define PAGE_SZ sizeof(t_page)
	
typedef struct	s_chunk 
{
	size_t		size;
	bool		is_free;
	struct s_chunk 	 *next;

}				t_chunk;

typedef struct	s_page
{
	t_chunk		*tiny;
	t_chunk		*small;
	t_chunk		*large;
}				t_page;

t_page			g_page;
pthread_mutex_t 	g_mmutex;

void			*malloc(size_t size);
// void			*realloc(void *ptr, size_t size);
// void			*free(void *ptr);
// void			show_alloc_mem();

#endif