/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moving.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 23:49:44 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/07/11 00:20:09 by tkempf-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header/so_long.h"

/*
move_tester :
- Adds the inability to go through walls.
- Opens exits if all items have been collected.
*/
int	move_tester(t_data *data, int x_tested, int y_tested)
{
	int		i;

	i = 0;
	while (y_tested > 0)
	{
		if (data->map[i] == '\n')
			y_tested--;
		i++;
	}
	i += x_tested;
	if (data->map[i] == 'C')
	{
		data->map[i] = '0';
		collectible_progress(data);
	}
	if (data->map[i] == '0' || data->map[i] == 'C' || data->map[i] == 'P')
		return (0);
	else if (data->map[i] == 'E' && exit_status(0) == 1)
		close_window(data);
	return (-1);
}

void	collectible_progress(t_data *data)
{
	int			i;
	int			collectible_nbr;

	i = 0;
	collectible_nbr = 0;
	while (data->map[i])
	{
		if (data->map[i] == 'C')
			collectible_nbr++;
		i++;
	}
	if (collectible_nbr == 0)
	{
		change_exit_to_open(data);
		exit_status(1);
	}
}

void	change_exit_to_open(t_data *data)
{
	int		i;
	int		x;
	int		y;

	i = 0;
	x = 0;
	y = 0;
	while (data->map[i])
	{
		if (data->map[i] == 'E')
			put_sprite(x, y, data, "sprites/exit_open.xpm");
		x++;
		if (data->map[i] == '\n')
		{
			x = 0;
			y++;
		}
		i++;
	}
	exit_status(1);
}

// option == 0 : check exit status
// option == 1 : set exit to open
int	exit_status(int option)
{
	static int	exit_status = 0;

	if (option == 1)
		exit_status = 1;
	return (exit_status);
}

/*
put_step_nbr :
- Displays the number of steps the player takes in the terminal.
- Uses the itoa function to convert the number of steps into
	a printable string of char.
*/
void	put_step_nbr(void)
{
	static int	step_nbr = 0;
	char		*itoa_result;

	step_nbr++;
	itoa_result = ft_itoa(step_nbr);
	ft_putstr(itoa_result);
	write(1, "\n", 1);
	free (itoa_result);
}
