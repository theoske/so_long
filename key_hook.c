/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 23:42:22 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/07/10 23:44:30 by tkempf-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

/*
key_hook :
- Adds the movement to the character.
- Adds the ability to close the game by pressing ESC key. 
*/
int	key_hook(int keycode, t_data *data)
{
	if (keycode == 13)
		move_up(data);
	else if (keycode == 1)
		move_down(data);
	else if (keycode == 2)
		move_right(data);
	else if (keycode == 0)
		move_left(data);
	if (keycode == 53)
		close_window(data);
	return (0);
}

void	move_left(t_data *data)
{
	if (move_tester(data, data->player_position_x - 1,
			data->player_position_y) == 0)
	{
		put_step_nbr();
		put_sprite(data->player_position_x,
			data->player_position_y, data, "sprites/ground.xpm");
		data->player_position_x--;
		put_sprite(data->player_position_x,
			data->player_position_y, data, "sprites/player.xpm");
	}
}

void	move_right(t_data *data)
{
	if (move_tester(data, data->player_position_x + 1,
			data->player_position_y) == 0)
	{
		put_step_nbr();
		put_sprite(data->player_position_x,
			data->player_position_y, data, "sprites/ground.xpm");
		data->player_position_x++;
		put_sprite(data->player_position_x,
			data->player_position_y, data, "sprites/player.xpm");
	}
}

void	move_down(t_data *data)
{
	if (move_tester(data, data->player_position_x,
			data->player_position_y + 1) == 0)
	{
		put_step_nbr();
		put_sprite(data->player_position_x,
			data->player_position_y, data, "sprites/ground.xpm");
		data->player_position_y++;
		put_sprite(data->player_position_x,
			data->player_position_y, data, "sprites/player.xpm");
	}
}

void	move_up(t_data *data)
{
	if (move_tester(data, data->player_position_x,
			data->player_position_y - 1) == 0)
	{
		put_step_nbr();
		put_sprite(data->player_position_x,
			data->player_position_y, data, "sprites/ground.xpm");
		data->player_position_y--;
		put_sprite(data->player_position_x,
			data->player_position_y, data, "sprites/player.xpm");
	}
}
