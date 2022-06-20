/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 14:53:32 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/06/20 16:31:42 by tkempf-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct s_vars
{
	void	*mlx;
	void	*mlx_win;
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_vars;

int key_hook(int keycode, t_vars *vars)
{
	static int	x = 100;
	static int	y = 100;

	if (keycode == 13)
		y -= 49;
	else if (keycode == 1)
		y += 49;
	else if (keycode == 2)
		x += 49;
	else if (keycode == 0)
		x -= 49;
	mlx_put_image_to_window(vars->mlx, vars->mlx_win, vars->img, x, y);
	if (keycode == 53)
	{
		mlx_destroy_image(vars->mlx, vars->img);
		mlx_destroy_window(vars->mlx, vars->mlx_win);
		exit(EXIT_SUCCESS);
	}
	return (0);
}

int main()//utiliser images
{
	t_vars	vars;
	char	*dst;
	int		i;
	int		j;
	int		x;
	int		y;
	
	vars.mlx = mlx_init();
	vars.mlx_win = mlx_new_window(vars.mlx, 1280, 720, "so_long");
	vars.img = mlx_new_image(vars.mlx, 50, 50);
	vars.addr = mlx_get_data_addr(vars.img, &vars.bits_per_pixel, &vars.line_length, &vars.endian);
	x = 0;
	while (x <= 50)
	{
		y = 0;
		while (y <= 50)
		{
			dst = vars.addr + (y * vars.line_length + x * (vars.bits_per_pixel / 8));
			*(unsigned int*)dst = 10760863;
			y++;
		}
		x++;
	}
	mlx_hook(vars.mlx_win, 2, 1L<<0, key_hook, &vars);
	mlx_loop(vars.mlx);
	return (0);
}