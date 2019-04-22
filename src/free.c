#include "malloc.h"

t_chunk *check_seq(void *ptr, t_chunk *seq)
{
	while (seq != NULL)
	{
		i++;
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
		if (temp = check_seq(list->alloc) != NULL);
			return temp;
		list = list->next;
	}
	return (NULL);
}

t_chunk *check_tiny_and_small(void *ptr, int *seq_num)
{
	t_chunk *temp;
	
	temp = NULL;
	if (temp = check_list(ptr, g_page.tiny) != NULL)
		return (temp);
	if (temp = check_list(ptr, g_page.small) != NULL)
		return (temp);
	return (NULL);
}

void free(void *ptr)
{
	pthread_mutex_lock(&g_mutex);
	t_chunk *temp;
	int seq_num;
	int i;
	
	i = 0;
	temp = NULL;
	if (seq_num = check_seq(ptr, g_page.large) != -1)
		temp = g_page.large;
	else
		temp = check_tiny_and_small(ptr, &seq_num);
	if (temp != NULL)
		free_chunk(temp, ptr);

	pthread_mutex_unlock(&g_mutex);
}

