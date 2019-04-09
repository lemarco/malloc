#include "malloc.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void *malloc(size_t size)
{
	void* ptr;

	ptr = NULL;
	pthread_mutex_lock(&g_mmutex);
	if (size < 1 || size > MAX_SIZE)
	{
		pthread_mutex_unlock(&g_mmutex);
		return (NULL);
	}
	if (size < TINY_ALLOC)
		ptr = allocator(size, false);
	else if (size < SMALL_ALLOC)
		ptr = allocator(size, true);
	else
		ptr = large_allocator(size);
	if (ptr == NULL)
	{
		pthread_mutex_unlock(&g_mmutex);
		return (NULL);
	}
	pthread_mutex_unlock(&g_mmutex);
	return ((void*)ptr + CHUNK_SZ);
}

