/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unmap.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihoienko <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 12:30:24 by ihoienko          #+#    #+#             */
/*   Updated: 2019/04/30 12:30:25 by ihoienko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	free_large(t_chunk *chunk, size_t size)
{
	if (chunk->prev)
		chunk->prev->next = chunk->next;
	else
		g_page.large = chunk->next;
	if (chunk->next)
		chunk->next->prev = chunk->prev;
	munmap(chunk, ALIGN(size + CHUNK_SZ));
}

void	unmap_zone(t_alloc *list, int flag)
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

void	free_chunk(t_chunk *chunk, int flag)
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
	if (!temp->free)
		unmap_zone(temp, flag);
}
