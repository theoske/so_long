/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 14:53:32 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/07/11 00:09:13 by tkempf-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

/*
so_long :
- Checks if there is only 1 argument (the map file name).
- Checks if the map respect the subject rules.
- Creates the window with the right size.
- Puts the sprites on the window according to the map.
- Handles user input.
*/
int	main(int argc, char *argv[])
{
	t_data		data;

	if (argc != 2)
		return (argument_error());
	if (map_checker(argv[1]) == -1)
		return (map_error());
	data.map_filename = argv[1];
	initialisation(&data);
	map_parser(&data);
	mlx_hook(data.mlx_win, 2, 1L << 0, key_hook, &data);
	mlx_hook(data.mlx_win, 17, 0, close_window, &data);
	mlx_loop(data.mlx);
	return (0);
}
