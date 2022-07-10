/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_checker.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 23:38:24 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/07/11 00:55:33 by tkempf-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/so_long.h"

/*
map_checker checks if the map follows the following instructions :
- The map must have at least 1 exit 'E', 1 collectible item 'C'
	and 1 spawning area for the player 'P'.
- The map must have a rectangular shape.
- The map must be surrounded on all sides by walls.
*/
int	map_checker(char *map_filename)
{
	char	*map;

	map = get_file(map_filename);
	if (!map || top_bot_walls_checker(map) == -1 || content_checker(map) == -1
		|| side_wall_checker(map) == -1 || rect_map_checker(map) == -1)
	{
		free (map);
		return (-1);
	}
	free (map);
	return (0);
}

int	rect_map_checker(char *map)
{
	int		i;
	int		first_line_size;
	int		line_size;

	first_line_size = 0;
	while (map[first_line_size] != '\n')
		first_line_size++;
	line_size = 0;
	i = 0;
	while (map[i])
	{
		if (map[i] != '\n')
			line_size++;
		else
		{
			if (line_size != first_line_size)
				return (-1);
			line_size = 0;
		}
		i++;
	}
	if (line_size != first_line_size)
		return (-1);
	return (0);
}

int	side_wall_checker(char *map)
{
	int		i;

	i = 0;
	while (map[i + 1])
	{
		if (map[i] == '\n' && (map[i - 1] != '1' || map[i + 1] != '1'))
			return (-1);
		i++;
	}
	return (0);
}

int	top_bot_walls_checker(char *map)
{
	int		i;

	i = 0;
	while (map[i] && map[i] != '\n')
	{
		if (map[i] != '1')
			return (-1);
		i++;
	}
	i = ft_strlen(map) - 1;
	while (map[i] && map[i] != '\n')
	{
		if (map[i] != '1')
			return (-1);
		i--;
	}
	return (0);
}

int	content_checker(char *map)
{
	if (exit_on_map(map) == -1 || collectible_on_map(map) == -1
		|| spawn_on_map(map) == -1)
		return (-1);
	return (0);
}
