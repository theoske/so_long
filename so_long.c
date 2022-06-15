/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 14:53:32 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/06/15 16:44:27 by tkempf-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int *move(void *mlx, void *mlx_win)
{
    static int  i = 0;
    
    mlx_pixel_put(mlx, mlx_win, i, i, 15033088);
    i++;
    return (0);
}

int main()
{
    void    *mlx;
    void    *mlx_win;
    int     i;
    int     j;
    int     *ptrmove;
    
    mlx = mlx_init();
    mlx_win = mlx_new_window(mlx, 1440, 1080, "win");
    i = 0;
    while (i++ < 500)
    {
        j = 0;
        while (j++ < 500)
            mlx_pixel_put(mlx, mlx_win, i, j, 7430598);
    }
    mlx_key_hook(mlx, move(mlx, mlx_win), 0);
    mlx_loop(mlx);
    return (0);
}