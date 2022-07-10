/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialisation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 23:33:04 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/07/10 23:36:30 by tkempf-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

/*
initialisation :
- Transfer the map file to the map string.
- Creates the game window with the dimension
	of the map that we got from the set_dimension function.
*/
void	initialisation(t_data *data)
{
	int			fd;
	t_dimension	dimension;

	set_dimension(data->map_filename, &dimension);
	fd = open(data->map_filename, O_RDWR);
	data->map = get_file(data->map_filename);
	close (fd);
	data->mlx = mlx_init();
	data->mlx_win = mlx_new_window(data->mlx,
			dimension.x * 32 - dimension.x,
			dimension.y * 32 - dimension.y, "so_long");
	data->img = mlx_new_image(data->mlx,
			dimension.x * 32 - dimension.x, dimension.y * 32 - dimension.y);
	data->addr = mlx_get_data_addr(data->img,
			&data->bits_per_pixel, &data->line_length, &data->endian);
}

/*
set_dimension :
- Finds the height and width of the window and puts
	them in the dimension structure's values.
*/
void	set_dimension(char *map_filename, t_dimension *dimension)
{
	int		i;
	int		fd;
	char	*map;

	i = 0;
	dimension->x = 0;
	fd = open(map_filename, O_RDONLY);
	map = get_next_line(fd);
	while (map[i] && map[i] != '\n')
		i++;
	dimension->x = i;
	dimension->y = 0;
	i = 0;
	while (map)
	{
		dimension->y++;
		map = get_next_line(fd);
	}
	free (map);
	close (fd);
}
