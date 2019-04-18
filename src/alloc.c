#include "malloc.h"


void *put_chunk(t_chunk **free, t_chunk **alloc, size_t size)
{
	t_chunk *chunk;
	
	chunk  = *free;
	(*free) = chunk->next;
	if ((*free))
		(*free)->prev = NULL;
	chunk->next = *alloc;
	if ((*alloc))
		(*alloc)->prev = chunk;
	(*alloc) = chunk;
	chunk->size = size;
	
	return (chunk + 1);
}

void set_chunk(t_alloc **alloc_list, t_alloc *memory, size_t size)
{
	t_chunk	*free_chunk = (void*)memory + sizeof(t_alloc);

	memory->prev = NULL;
	memory->next = *alloc_list;
	*alloc_list = memory;
	if (memory->next)
		memory->next->prev = memory;
	memory->alloc = NULL;
	memory->free = free_chunk;
	while((void*)free_chunk + (size + sizeof(t_chunk)) * 2 < \
		(void*)memory + size * NB_BLOCKS)
	{
		free_chunk->next = (void*)free_chunk + CHUNK_SZ + size;
		free_chunk->next->prev = free_chunk;
		free_chunk = free_chunk->next;	
	}	
	free_chunk->next = NULL;
}

void *tiny_alloc(size_t size)
{
	t_alloc *alloc;

	alloc = g_page.tiny;

	while(alloc && !alloc->free)
		alloc = alloc->next;
	if (!alloc)
	{
		alloc = mmap(0, NB_BLOCKS * TINY_CHUNK_SZ, PROTECT, FLAGS, -1, 0);
		if (alloc == MAP_FAILED)
			return (NULL);
		set_chunk(&g_page.tiny, alloc, TINY_CHUNK_SZ);
	}
	return (put_chunk(&g_page.tiny->free,&g_page.tiny->alloc, size));
}

void *small_alloc(size_t size)
{
	t_alloc *alloc;

	alloc = g_page.tiny;

	while(alloc && !alloc->free)
		alloc = alloc->next;
	if (!alloc)
	{
		alloc = mmap(0, NB_BLOCKS * SMALL_CHUNK_SZ, PROTECT, FLAGS, -1, 0);
		if (alloc == MAP_FAILED)
			return (NULL);
		set_chunk(&g_page.small, alloc, SMALL_CHUNK_SZ);
	}

	return (put_chunk(&g_page.small->free, &g_page.small->alloc, size));
}

void *large_alloc(size_t size)
{
	t_chunk *chunk;
	size_t aligned_size;

	aligned_size = ALIGN(size);
	chunk = mmap(0, aligned_size, PROTECT, FLAGS, -1, 0);
	if (chunk == MAP_FAILED)
		return (NULL);
	chunk->next = g_page.large;
	chunk->prev = NULL;
	chunk->size = size;
	
	if (g_page.large)
		g_page.large->prev = chunk;
	g_page.large = chunk;
	return ((void*)chunk + CHUNK_SZ);
	
}
