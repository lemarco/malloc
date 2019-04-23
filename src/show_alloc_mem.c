#include "malloc.h"

static inline void		show_alloc_tiny_small(t_alloc *mem, \
						size_t *total, const char *size_str)
{
	t_chunk		*chunk;

	while (mem)
	{
		printf("%s : %p\n", size_str, mem);
		chunk = mem->alloc;
		while (chunk)
		{
			printf("%p - %p : %lu bytes\n", (void *)chunk + \
			sizeof(t_chunk), (void *)chunk + CHUNK_SZ + \
			chunk->size, chunk->size);
			*total += chunk->size;
			chunk = chunk->next;
		}
		mem = mem->next;
	}
}

static inline void		show_alloc_large(size_t *total)
{
	t_chunk	*chunk;

	printf("LARGE : %p\n", g_page.large);
	chunk = g_page.large;
	while (chunk)
	{
		printf("%p - %p : %lu bytes\n", (void *)chunk + \
		CHUNK_SZ, (void *)chunk + CHUNK_SZ + \
		chunk->size, chunk->size);
		*total += chunk->size;
		chunk = chunk->next;
	}
}


void					show_alloc_mem(void)
{
	size_t				total;

	total = 0;
	pthread_mutex_lock(&g_mmutex);
	if (g_page.tiny)
		show_alloc_tiny_small(g_page.tiny, &total, "TINY");
	if (g_page.small)
		show_alloc_tiny_small(g_page.small, &total, "SMALL");
	if (g_page.large)
		show_alloc_large(&total);
	printf("Total : %lu bytes\n", total);
	pthread_mutex_unlock(&g_mmutex);
}

