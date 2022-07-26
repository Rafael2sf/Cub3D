/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 16:36:01 by marvin            #+#    #+#             */
/*   Updated: 2022/06/30 16:36:01 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libc3d.h"
#include <sys/time.h>

static void		rotation_handler(t_app *p, double theta);
static double	handle_mouse(t_app *p);
static int		handle_key_events(t_app *p);

int	handlers(t_app *p)
{
	struct timeval time;
	double	time2;

	gettimeofday(&time, NULL);
	time2 = p->time;
	p->time = time.tv_sec + (time.tv_usec / 1000000.0);
	p->mov_speed = (p->time - time2) * 64;
	//printf("%f\n", p->mov_speed);
	raycaster(p);
	draw_image(p, &p->tex[TEX_HANDS],
		v2i((MAP_W / 5) * 2, 2 + MAP_H - (MAP_H / 3)),
		v2i(MAP_W / 5, MAP_H / 3));
	ft_minimap(p, 1, 1);
	handle_key_events(p);
	if (!p->mouse_enable)
		mlx_mouse_show(p->mlx.ptr, p->mlx.win);
	else
		mlx_mouse_hide(p->mlx.ptr, p->mlx.win);
	mlx_put_image_to_window(p->mlx.ptr, p->mlx.win, p->mlx.data.ptr, 0, 0);
	return (0);
}

int	esc_handler(t_app *p)
{
	mlx_mouse_show(p->mlx.ptr, p->mlx.win);
	mlx_loop_end(p->mlx.ptr);
	cleaner(p);
	exit(0);
	return (0);
}

static void	rotation_handler(t_app *p, double theta)
{
	double	cs;
	double	sn;
	double	old_dir;

	if (theta == 0)
		return ;
	cs = cos(theta);
	sn = sin(theta);
	old_dir = p->dir.x;
	p->dir.x = p->dir.x * cs - p->dir.y * sn;
	p->dir.y = old_dir * sn + p->dir.y * cs;
	old_dir = p->plane.x;
	p->plane.x = p->plane.x * cs - p->plane.y * sn;
	p->plane.y = old_dir * sn + p->plane.y * cs;
}

static double	handle_mouse(t_app *p)
{
	t_v2i			m_pos;
	t_v2d			mouse;

	if (!p->mouse_enable || !p->in_window)
		return (0);
	mouse.x = 0.0;
	mouse.y = 0.0;
	mlx_mouse_get_pos(p->mlx.ptr, p->mlx.win, &(m_pos.x), &(m_pos.y));
	if (m_pos.x >= 0 && m_pos.x <= MAP_W && m_pos.y >= 0 && m_pos.y <= MAP_H
		&& (m_pos.x != MAP_W / 2 || m_pos.y != MAP_H / 2))
	{
		if (m_pos.x < MAP_W / 2 - 1)
			mouse.x = -((double)(MAP_W / 2 - m_pos.x) / 64) * X_SEN;
		else if (m_pos.x > MAP_W / 2 + 1)
			mouse.x = ((double)-(MAP_W / 2 - m_pos.x) / 64) * X_SEN;
		if (m_pos.y < MAP_H / 2 - 1)
			mouse.y = -((double)-(MAP_H / 2 - m_pos.y)) * X_SEN;
		else if (m_pos.y > MAP_H / 2 + 1)
			mouse.y = ((double)(MAP_H / 2 - m_pos.y)) * X_SEN;
		if (mouse.y != 0)
			p->mouse.y += mouse.y * (MAP_H / 100);
		mlx_mouse_move(p->mlx.ptr, p->mlx.win, MAP_W / 2, MAP_H / 2);
	}
	return (mouse.x);
}

static int	handle_key_events(t_app *p)
{
	t_v2d	v;

	if (p->kmap[_UA])
		p->mouse.y += (MAP_H / 100) / 32;
	if (p->kmap[_DA])
		p->mouse.y -= (MAP_H / 100) / 32;
	rotation_handler(p,
		((handle_mouse(p) + (p->kmap[_RA] * X_ROT - p->kmap[_LA] * X_ROT)) / 2) * p->mov_speed);
	v.x = (((p->dir.x * p->kmap[_W]) + (p->dir.y * p->kmap[_A])
			- (p->dir.x * p->kmap[_S]) - (p->dir.y * p->kmap[_D])) / X_VEL) * p->mov_speed;
	v.y = (((p->dir.y * p->kmap[_W]) - (p->dir.x * p->kmap[_A])
			- (p->dir.y * p->kmap[_S]) + (p->dir.x) * p->kmap[_D]) / X_VEL) * p->mov_speed;
	if (v.x != 0 || v.y != 0)
		collision_behaviour(p, v);
	if (p->mouse.y >= MAP_H)
		p->mouse.y = MAP_H - 1;
	else if (p->mouse.y < -MAP_H)
		p->mouse.y = -(MAP_H - 1);
	return (0);
}
