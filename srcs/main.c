/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.00.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 000/06/08 16:00:01 by marvin            #+#    #+#             */
/*   Updated: 000/06/08 16:00:01 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libc3d.h"

void	prep_ptrs(t_app *p)
{	
	p->mlx.win = mlx_new_window(p->mlx.ptr, MAP_W, MAP_H, "Cub3D");
	if (!p->mlx.win)
	{
		perror("Error creating window");
		cleaner(p);
		exit(2);
	}
	p->mlx.data.ptr = mlx_new_image(p->mlx.ptr, MAP_W, MAP_H);
	if (!p->mlx.data.ptr)
	{
		perror("Error creating image");
		cleaner(p);
		exit(2);
	}
	p->mlx.data.addr = mlx_get_data_addr(p->mlx.data.ptr, &p->mlx.data.bpp,
			&p->mlx.data.line_length, &p->mlx.data.endian);
	p->mlx.data.width = MAP_W;
	p->mlx.data.height = MAP_H;
}

void	prep_tex_data(t_app *p)
{
	int	i;

	i = 0;
	p->mlx.ptr = mlx_init();
	if (!p->mlx.ptr)
	{
		perror("Error establishing connection to graphical system");
		exit(2);
	}
	p->tex[TEX_HANDS].path = "./srcs/textures/pistol.xpm";
	while (i < TEX_COUNT)
	{
		if (!handle_new_image(&p->tex[i], p->mlx.ptr))
		{
			ft_putstr_fd("Error\nFailed to load image: ", STDERR_FILENO);
			ft_putstr_fd(p->tex[i].path, STDERR_FILENO);
			ft_putchar_fd('\n', STDERR_FILENO);
			cleaner(p);
			exit (2);
		}
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_app	p;
	char	*tmp;

	ft_memset(&p, 0, sizeof(t_app));
	if (argc != 2)
	{
		ft_putstr_fd("Error\nUsage: ./prog *.cub\n", STDERR_FILENO);
		exit (1);
	}
	tmp = (argv[1]) + (ft_strlen(argv[1]) - 4);
	if (tmp <= argv[1] || ft_strncmp(tmp, ".cub", 5) != 0)
	{
		ft_putstr_fd("Error\nUnknown map file extension\n", STDERR_FILENO);
		exit(1);
	}
	readmap(&p, argv[1]);
	prep_tex_data(&p);
	prep_ptrs(&p);
	hooks(&p);
	p.in_window = true;
	p.mouse_enable = true;
	mlx_mouse_hide(p.mlx.ptr, p.mlx.win);
	mlx_mouse_move(p.mlx.ptr, p.mlx.win, MAP_W / 2, MAP_H / 2);
	mlx_loop(p.mlx.ptr);
	return (0);
}
