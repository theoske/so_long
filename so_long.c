/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 14:53:32 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/06/16 17:14:33 by tkempf-e         ###   ########.fr       */
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
}	t_vars;


int key_hook(int keycode, t_vars *vars)
{
	static int	x = 700;
	static int	y = 700;

	if (keycode == 13)
		y++;
	else if (keycode == 1)
		y--;
	else if (keycode == 2)
		x++;
	else if (keycode == 0)
		x--;
	printf("%d\n", keycode);
	mlx_pixel_put(vars->mlx, vars->mlx_win, x, y, 0xc52907);
	return (0);
}

int main()//utiliser images
{
	t_vars	vars;
	int		i;
	int		j;
    
	vars.mlx = mlx_init();
	vars.mlx_win = mlx_new_window(vars.mlx, 1440, 1080, "win");
	i = 0;
	while (i++ < 500)
	{
		j = 0;
		while (j++ < 500)
			mlx_pixel_put(vars.mlx, vars.mlx_win, i, j, 7430598);
    }
	mlx_key_hook(vars.mlx_win, key_hook, &vars);
	mlx_loop(vars.mlx);
	return (0);
}