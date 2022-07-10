/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_content_checker_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 23:40:56 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/07/11 00:20:11 by tkempf-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header/so_long.h"

int	spawn_on_map(char *map)
{
	int		i;

	i = 0;
	while (map[i])
	{
		if (map[i] == 'P')
			return (0);
		i++;
	}
	return (-1);
}

int	collectible_on_map(char *map)
{
	int		i;

	i = 0;
	while (map[i])
	{
		if (map[i] == 'C')
			return (0);
		i++;
	}
	return (-1);
}

int	exit_on_map(char *map)
{
	int		i;

	i = 0;
	while (map[i])
	{
		if (map[i] == 'E')
			return (0);
		i++;
	}
	return (-1);
}
