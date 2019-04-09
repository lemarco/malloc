#include "malloc.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
/*
t_chunk *create_new_allocation_area(size_t size, bool flag)
{
	t_chunk *cur; // old
	t_chunk *temp; // new

	flag ? (cur = g_page.small) : (cur = g_page.tiny);
	while(cur->size >= size && cur->is_free)
		cur = cur->next;
	temp = cur;
	cur = (void*)temp + size + CHUNK_SZ;
	cur->is_free = temp->is_free;
	cur->size = temp->size - size - CHUNK_SZ;
	cur->next = temp->next;
	temp->size = size;
	temp->is_free = 0;
	temp->next = cur;
	return (temp);

}

bool mmaper(t_chunk *chunk, bool flag)
{
	size_t size;
	flag ? (size = SMALL_AREA) : (size  = TINY_AREA);

	if (!(chunk = mmap(0, size, PROTECT, FLAGS, -1, 0)))
			return (false);
	return true;
}

void add_chunk_to_page(t_chunk *cur, t_chunk *add)
{
	if (cur == NULL)
		add->next = NULL;
	else
add->next = cur;
	cur = add;
}

t_chunk *init_new_chunk(bool flag)
{
	t_chunk *free_chunk;
	size_t size;
	
	free_chunk = NULL;
	flag ? (size = SMALL_AREA) : (size  = TINY_AREA);
	if (mmaper(free_chunk, flag) == false)
		return (NULL);
	free_chunk->size = size;
	free_chunk->is_free = true;
	if (!flag)
		add_chunk_to_page(g_page.tiny, free_chunk);
	else
		add_chunk_to_page(g_page.small, free_chunk);
	return (free_chunk);
}

t_chunk *find_free_chunk(size_t size, bool flag)
{
	t_chunk *cur;

	if (flag)
		cur = g_page.small;
	else
		cur = g_page.tiny;
	while (cur)
	{
		if (cur->is_free && cur->size >= size)
			return cur;
		else 
			cur = cur->next;
	}
	return (NULL);
}

void		*allocator(size_t size, bool flag)
{
	t_chunk *chunk;
	t_chunk *cur;

	chunk = NULL;
	if (flag)
		cur = g_page.small;
	else
		cur = g_page.tiny;
	if (cur == NULL)
	{
		if (init_new_chunk(flag) == NULL)
			return (NULL);
		return (create_new_allocation_area(size, flag));
	}
	else
	{
		if ((chunk = find_free_chunk(size, 1)) == NULL)
		{
			if ((chunk = init_new_chunk(flag)) == NULL)
			return (NULL);
		}
		return (create_new_allocation_area(size, flag));
	}
}


t_chunk		*add_large_chunk(t_chunk *prev, size_t size)
{
	t_chunk *new;

	if (!(new = mmap(0, size + CHUNK_SZ, PROTECT, FLAGS, -1, 0)))
		return (NULL);
	new->size = size;
	new->is_free = false;
	new->next = NULL;
	prev->next = new;
	return (new);
}

t_chunk		*get_last_large_chunk()
{
	t_chunk *tmp;

	tmp = g_page.large;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

void *large_allocator(size_t size)
{
	t_chunk *last;

	if (g_page.large == NULL)
	{
		if (!(g_page.large = mmap(0, size + CHUNK_SZ, PROTECT, FLAGS, -1, 0)))
			return (NULL);
		g_page.large->is_free = false;
		g_page.large->size = size;
		g_page.large->next = NULL;
		return (g_page.large);
	}
	last = get_last_large_chunk();
	last->next = add_large_chunk(last, size);
	return (last->next);
}
*/

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

