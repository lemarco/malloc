/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihoienko <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 12:21:04 by ihoienko          #+#    #+#             */
/*   Updated: 2019/04/30 12:21:09 by ihoienko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	*malloc(size_t size)
{
	void *(*m_size[3])(size_t);
	void *ptr;

	m_size[0] = &tiny_alloc;
	m_size[1] = &small_alloc;
	m_size[2] = &large_alloc;
	if (size == 0 || size > MAX_SIZE - CHUNK_SZ)
		return (NULL);
	pthread_mutex_lock(&g_mmutex);
	ptr = m_size[MSIZE(size)](size);
	pthread_mutex_unlock(&g_mmutex);
	return (ptr);
}
