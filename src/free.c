#include "malloc.h"

void	munmap_chunk(t_chunk *to_free)
{
	munmap(to_free, to_free->size);
	to_free->is_free = 1;
}

t_chunk *find_chunk(t_chunk *to_find, t_chunk **to_save, void* ptr)
{
	*to_save = to_find;
	while(to_find->next)
	{
		*to_save = to_find;
		if ((to_find->next + CHUNK_SZ) == ptr)
			return (to_find->next);
		to_find->next = to_find->next->next;
	}
	return NULL;
}

void	free(void *ptr)
{
	size_t size;
	t_chunk *chunk_to_free;
	t_chunk *cur;
	t_chunk *to_save;
	if (ptr == NULL)
		return ;
	pthread_mutex_lock(&g_mmutex);
	chunk_to_free = (void*)ptr - CHUNK_SZ;
	size = chunk_to_free->size;
	if (size <= TINY_AREA)
		cur = g_page.tiny;
	else if (size <= SMALL_AREA)
		cur = g_page.small;
	else
		cur = g_page.large;
	munmap_chunk(find_chunk(cur, &to_save, ptr));
	pthread_mutex_unlock(&g_mmutex);
}

// void		free(void *ptr)
// {
// 	t_block	*free_ptr;

// 	free_ptr = NULL;

	
// 	if ((free_ptr = find_prev_block(g_page.tiny, ptr)) != NULL)
// 	{
// 		concat_free_next(free_ptr, free_ptr->next, ptr);
// 		munmap_page_small(g_page.tiny, TINY_SIZE, free_ptr);
// 	}
// 	else if ((free_ptr = find_prev_block(g_page.small, ptr)) != NULL)
// 	{
// 		concat_free_next(free_ptr, free_ptr->next, ptr);
// 		munmap_page_small(g_page.small, SMALL_SIZE, free_ptr);
// 	}
// 	else if ((free_ptr = find_prev_block(g_page.large, ptr)) != NULL)
// 		munmap_page_large(free_ptr, free_ptr->next, ptr);
	
// 	return ;
// }