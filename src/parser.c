/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 23:35:09 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/07/11 00:55:19 by tkempf-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/so_long.h"

/*
map_parser :
- With the parser_loop function :
	Parses the map and replaces the spawning sprites by
	ground sprites to avoid having multiple player sprites
	on display since the map can contain multiple spawning areas.
- Sets the player sprite and position on the first spawning area it finds.
*/
void	map_parser(t_data *data)
{
	int		i;
	int		x;
	int		y;

	parser_loop(data);
	i = 0;
	x = 0;
	y = 0;
	while (data->map[i] && data->map[i - 1] != 'P')
	{
		if (data->map[i] == 'P')
		{
			put_sprite(x, y, data, "sprites/player.xpm");
			data->player_position_x = x;
			data->player_position_y = y;
		}
		else if (data->map[i] == '\n')
		{
			x = -1;
			y++;
		}
		x++;
		i++;
	}
}

void	parser_loop(t_data *data)
{
	int		i;
	int		x;
	int		y;

	i = 0;
	x = 0;
	y = 0;
	while (data->map[i])
	{
		if (data->map[i] == '0' || data->map[i] == 'P')
			put_sprite(x, y, data, "sprites/ground.xpm");
		else if (data->map[i] == '1')
			put_sprite(x, y, data, "sprites/wall.xpm");
		else if (data->map[i] == 'C')
			put_sprite(x, y, data, "sprites/collectible.xpm");
		else if (data->map[i] == 'E')
			put_sprite(x, y, data, "sprites/exit_close.xpm");
		else if (data->map[i] == '\n')
		{
			x = -1;
			y++;
		}
		x++;
		i++;
	}
}

void	put_sprite(int x, int y, t_data *data, char *sprite_filename)
{
	int		height;
	int		width;

	height = 31;
	width = 31;
	data->img = mlx_xpm_file_to_image(data->mlx,
			sprite_filename, &width, &height);
	mlx_put_image_to_window(data->mlx, data->mlx_win,
		data->img, x * 31, y * 31);
}
