#include "malloc.h"


void 

void free(void *ptr)
{
	pthread_mutex_lock(&g_mutex);
	
	free_chunk(find_chunk(ptr);

	pthread_mutex_unlock(&g_mutex);
}
