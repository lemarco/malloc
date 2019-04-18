#include "malloc.h"

void *malloc(size_t size)
{
	void *(*m_size[3])(size_t) = {&tiny_alloc, &small_alloc, &large_alloc};
	void *ptr;

	if (size == 0 || size > MAX_SIZE - CHUNK_SZ)
		return (NULL);
	pthread_mutex_lock(&g_mmutex);
	ptr = m_size[MSIZE(size)](size); 
	pthread_mutex_unlock(&g_mmutex);
	return (ptr);
}

