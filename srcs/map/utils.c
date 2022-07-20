/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 18:17:16 by marvin            #+#    #+#             */
/*   Updated: 2022/07/19 18:17:16 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "c3d_map.h"

/* Clean all the memory related to the map file */
void	map_clean(t_mp *mp)
{
	size_t	i;

	if (mp->fd >= 0)
		close(mp->fd);
	if (mp->line)
		free(mp->line);
	if (mp->p->tex[PATH_NO].path)
		free(mp->p->tex[PATH_NO].path);
	if (mp->p->tex[PATH_SO].path)
		free(mp->p->tex[PATH_SO].path);
	if (mp->p->tex[PATH_EA].path)
		free(mp->p->tex[PATH_EA].path);
	if (mp->p->tex[PATH_WE].path)
		free(mp->p->tex[PATH_WE].path);
	i = 0;
	while (i < mp->p->map.size_y)
		free(mp->p->map.data[i++]);
	free(mp->p->map.data);
}

/* A simple realloc suited for reading the map */
char	**readmap_realloc(char **src, size_t bytes)
{
	char	**buff;
	size_t	i;

	if (!src)
		return (NULL);
	buff = (char **)malloc(sizeof(char *) * bytes);
	if (!buff)
		return (NULL);
	i = 0;
	while (src[i] && i < bytes)
	{
		buff[i] = src[i];
		i++;
	}
	while (i < bytes)
		buff[i++] = 0;
	return (buff);
}

/* Returns true if c equals space (' ') */
int	ft_isblank(int c)
{
	return (c == ' ');
}

/* Check if a line only contains spaces */
int	ft_empty_line(char const *s)
{
	while (*s && *s != '\n')
	{
		if (!ft_isblank(*s))
			return (false);
		s++;
	}
	return (true);
}

/* Increment iter starting at += begin bytes for each space in p */
void	ft_hop_blank(char *p, size_t begin, size_t *iter)
{
	*iter += begin;
	while (ft_isblank(p[*iter]))
		(*iter) += 1;
	*iter -= begin;
}
