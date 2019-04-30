/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihoienko <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 12:30:13 by ihoienko          #+#    #+#             */
/*   Updated: 2019/04/30 12:30:15 by ihoienko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_chunk		*check_seq(void *ptr, t_chunk *seq)
{
	while (seq != NULL)
	{
		if (((void *)seq + CHUNK_SZ) == ptr)
			return (seq);
	}
	return (NULL);
}

t_chunk		*check_list(void *ptr, t_alloc *list)
{
	t_chunk *temp;

	while (list)
	{
		if ((temp = check_seq(ptr, list->alloc)) != NULL)
			return (temp);
		list = list->next;
	}
	return (NULL);
}

t_chunk		*check_tiny_and_small(void *ptr, int *flag)
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

void		free(void *ptr)
{
	t_chunk	*temp;
	int		flag;

	pthread_mutex_lock(&g_mmutex);
	temp = NULL;
	if ((temp = check_seq(ptr, g_page.large)) != NULL)
		flag = 2;
	else
		temp = check_tiny_and_small(ptr, &flag);
	if (temp != NULL)
		free_chunk(temp, flag);
	pthread_mutex_unlock(&g_mmutex);
}
