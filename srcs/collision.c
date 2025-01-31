/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 12:00:34 by marvin            #+#    #+#             */
/*   Updated: 2022/07/01 12:00:34 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libc3d.h"

static bool	collider(double pos_x, double pos_y, t_v2i *r, char **map);

void	collision_behaviour(t_app *p, t_v2d v)
{
	t_v2i	r;

	r.x = true;
	r.y = true;
	if (collider(p->pos.x + v.x, p->pos.y + v.y, &r, p->map.data))
	{
		p->pos.x += v.x;
		p->pos.y += v.y;
	}
	else
	{
		if ((r.x && !r.y)
			|| collider(p->pos.x + v.x, p->pos.y, NULL, p->map.data))
			p->pos.x += v.x;
		if ((!r.x && r.y)
			|| collider(p->pos.x, p->pos.y + v.y, NULL, p->map.data))
			p->pos.y += v.y;
	}
}

static bool	collider(double pos_x, double pos_y, t_v2i *r, char **map)
{
	t_v2d	v;
	double	old_dir;
	bool	no_col;
	double	theta;

	no_col = true;
	theta = 0.05;
	v.x = 0.2;
	v.y = 0;
	while (theta < 6.25)
	{
		old_dir = v.x;
		v.x = v.x * cos(theta) - v.y * sin(theta);
		v.y = old_dir * sin(theta) + v.y * cos(theta);
		if (r && map[(int)(pos_y)][(int)(pos_x + v.x)] == '1')
			r->x = false;
		if (r && map[(int)(pos_y + v.y)][(int)(pos_x)] == '1')
			r->y = false;
		if (map[(int)(pos_y + v.y)][(int)(pos_x + v.x)] == '1')
			no_col = false;
		if (no_col == false && (!r || r->x == false) && (!r || r->y == false))
			return (false);
		theta += 0.05;
	}
	return (no_col);
}
