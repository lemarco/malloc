/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihoienko <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 12:53:55 by ihoienko          #+#    #+#             */
/*   Updated: 2019/04/30 12:53:57 by ihoienko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include <unistd.h>
# include <sys/mman.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>

# define MAX_SIZE (~(size_t)0)
# define NB_BLOCKS 128
# define TINY_CHUNK_SZ 64
# define SMALL_CHUNK_SZ 1024
# define PAGE_SZ getpagesize()
# define ALIGN(x) ((x) + (!!((x) % PAGE_SZ) * (PAGE_SZ - (x) % PAGE_SZ)))
# define MSIZE(x) (((x) > TINY_CHUNK_SZ) + ((x) > SMALL_CHUNK_SZ))
# define FLAGS (MAP_ANON | MAP_PRIVATE)
# define PROTECT (PROT_READ | PROT_WRITE)
# define CHUNK_SZ sizeof(t_chunk)
# define PAGE_ST_SZ sizeof(t_page)

typedef struct			s_chunk
{
	size_t				size;
	struct s_chunk		*next;
	struct s_chunk		*prev;
}						t_chunk;

typedef struct			s_alloc
{
	struct s_alloc		*next;
	struct s_alloc		*prev;
	t_chunk				*alloc;
	t_chunk				*free;
}						t_alloc;

typedef struct			s_page
{
	t_alloc				*tiny;
	t_alloc				*small;
	t_chunk				*large;
}						t_page;

t_page					g_page;
pthread_mutex_t			g_mmutex;

void					*tiny_alloc(size_t size);
void					*small_alloc(size_t size);
void					*large_alloc(size_t size);
void					free_chunk(t_chunk *chunk, int flag);
t_chunk					*check_seq(void *ptr, t_chunk *seq);
t_chunk					*check_tiny_and_small(void *ptr, int *flag);
void					*malloc(size_t size);
void					*realloc(void *ptr, size_t size);
void					free(void *ptr);
void					show_alloc_mem();

#endif
