/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 14:53:32 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/06/20 15:59:55 by tkempf-e         ###   ########.fr       */
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
		y -= 100;
	else if (keycode == 1)
		y += 100;
	else if (keycode == 2)
		x += 100;
	else if (keycode == 0)
		x -= 100;
	mlx_put_image_to_window(vars->mlx, vars->mlx_win, vars->img, x, y);
	if (keycode == 53)
	{
		mlx_destroy_image(vars->mlx, vars->img);
		mlx_destroy_window(vars->mlx, vars->mlx_win);
		return (-1);
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
	vars.mlx_win = mlx_new_window(vars.mlx, 900, 900, "so_long");
	vars.img = mlx_new_image(vars.mlx, 100, 100);
	vars.addr = mlx_get_data_addr(vars.img, &vars.bits_per_pixel, &vars.line_length, &vars.endian);
	x = 0;
	while (x <= 100)
	{
		y = 0;
		while (y <= 100)
		{
			dst = vars.addr + (y * vars.line_length + x * (vars.bits_per_pixel / 8));
			*(unsigned int*)dst = 10760863;
			y++;
		}
		x++;
	}
	if (mlx_hook(vars.mlx_win, 2, 1L<<0, key_hook, &vars) == -1)
		exit(EXIT_SUCCESS);
	mlx_loop(vars.mlx);
	return (0);
}