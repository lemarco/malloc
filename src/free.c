#include "malloc.h"

void free_large(t_chunk *chunk, size_t size)
{
	
	if (chunk->prev)
		chunk->prev->next = chunk->next;
	else
		g_page.large = chunk->next;
	if (chunk->next)
		chunk->next->prev = chunk->prev;

	munmap(chunk, ALIGN(size + CHUNK_SZ));
}

void unmap_zone(t_alloc *list, int flag)
{
	size_t size;
	
	if (flag == 0)
		size = TINY_CHUNK_SZ;
	else
		size = SMALL_CHUNK_SZ;
	if (list->prev)
		list->prev->next = list->next;
	else if (flag)
		g_page.small = list->next;
	else
		g_page.tiny = list->next;
	munmap(list, NB_BLOCKS * size);
}

void free_chunk(t_chunk *chunk, int flag )
{
	t_alloc *temp;
	
	if (flag == 2)
		free_large(chunk, chunk->size);
	if (flag == 1)
		temp = g_page.small;
	else
		temp = g_page.tiny;
	 
	if (chunk->prev)
		chunk->prev->next = chunk->next;
	else
		temp->alloc = chunk->next;
	if (chunk->next)
		chunk->next->prev = chunk->prev;
	chunk->prev = NULL;
	chunk->next = temp->free;
	if (temp->free)
		temp->free->prev = chunk;
	temp->free = chunk;
	if ( !temp->free)
		unmap_zone(temp, flag);
}

t_chunk *check_seq(void *ptr, t_chunk *seq)
{
	while (seq != NULL)
	{
		if (((void *)seq + CHUNK_SZ) == ptr)
			return (seq);
	}	
	return (NULL);
}

t_chunk *check_list(void *ptr, t_alloc *list)
{
	t_chunk *temp;

	while (list)
	{
		if ((temp = check_seq(ptr, list->alloc)) != NULL)
			return temp;
		list = list->next;
	}
	return (NULL);
}

t_chunk *check_tiny_and_small(void *ptr, int *flag)
{
	t_chunk *temp;
	
	temp = NULL;
	if ((temp = check_list(ptr, g_page.tiny)) != NULL)
	{
		*flag = 0;
		return (temp);
	}
	if ((temp = check_list(ptr, g_page.small)) != NULL)
	{
		*flag = 1;
		return (temp);
	}
	return (NULL);
}

void free(void *ptr)
{
	pthread_mutex_lock(&g_mmutex);
	t_chunk *temp;
	int flag;
	
	temp = NULL;
	if ((temp = check_seq(ptr, g_page.large)) != NULL)
		flag  = 2;
	else
		temp = check_tiny_and_small(ptr, &flag);
	if (temp != NULL)
		free_chunk(temp, flag);

	pthread_mutex_unlock(&g_mmutex);
}

