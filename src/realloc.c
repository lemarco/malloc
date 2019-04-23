#include "malloc.h"

bool is_relocation_on_place(t_chunk *chunk, int flag, size_t size)
{
	if (flag == 0 && size + CHUNK_SZ <= TINY_CHUNK_SZ )
		return (true);
	if (flag == 1 && size + CHUNK_SZ <= SMALL_CHUNK_SZ )
		return (true);
	if (flag == 2 && size + CHUNK_SZ <= ALIGN(chunk->size + CHUNK_SZ))
		return (true);
	return (false);
}

void *realloc_out_of_place(t_chunk* temp, size_t size)
{
	void *new;
	size_t old_sz;

	if ((new = malloc(size)) == NULL)
		return (NULL);
	old_sz = ((t_chunk*)temp)->size;
	memcpy(new, temp + CHUNK_SZ, old_sz);
	free(temp + CHUNK_SZ);
	return (new);
	
}

void *realloc(void *ptr, size_t size)
{
	t_chunk *temp;
	int flag;

	temp = NULL;
	if (!ptr || size == 0 )
		return (NULL);
	pthread_mutex_lock(&g_mmutex);

	if ((temp = check_seq(ptr, g_page.large)) != NULL)
		flag = 2;
	else
	{
		if ((temp = check_tiny_and_small(ptr, &flag)) == NULL)
		{
			pthread_mutex_unlock(&g_mmutex);
			return (NULL);	
		}
	}
	if (is_relocation_on_place(temp, flag, size))
	{
		((t_chunk *)temp)->size = size;
		pthread_mutex_unlock(&g_mmutex);
		return (ptr);
	}
	pthread_mutex_unlock(&g_mmutex);
	return (realloc_out_of_place(temp, size));
}
