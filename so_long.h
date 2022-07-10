/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 14:53:49 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/07/10 23:30:19 by tkempf-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include <mlx.h>
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>

typedef struct s_data
{
	void	*mlx;
	void	*mlx_win;
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		player_position_x;
	int		player_position_y;
	char	*map_filename;
	char	*map;
}	t_data;

typedef struct s_dimension
{
	int		x;
	int		y;
}	t_dimension;

void		put_sprite(int x, int y, t_data *data, char *sprite_filename);
size_t		ft_strlen(const char *s);
void		ft_putstr(char *s);
char		*ft_strjoin(char *s1, char *s2);
char		*ft_line(char *line, char *buffer, int octet, int fd);
char		*get_next_line(int fd);
long int	ft_numbers(long int n);
char		*filling(long int n, char *tab, long int j, long int numbers);
char		*ft_itoa(int n);
char		*get_file(const char *map_filename);
int			close_window(t_data *data);
int			exit_status(int option);
void		change_exit_to_open(t_data *data);
void		collectible_progress(t_data *data);
int			move_tester(t_data *data, int x_tested, int y_tested);
void		put_step_nbr(void);
void		move_up(t_data *data);
void		move_down(t_data *data);
void		move_right(t_data *data);
void		move_left(t_data *data);
int			key_hook(int keycode, t_data *data);
void		set_dimension(char *map_filename, t_dimension *dimension);
void		initialisation(t_data *data);
void		parser_loop(t_data *data);
void		map_parser(t_data *data);
int			exit_on_map(char *map);
int			collectible_on_map(char *map);
int			spawn_on_map(char *map);
int			content_checker(char *map);
int			top_bot_walls_checker(char *map);
int			side_wall_checker(char *map);
int			rect_map_checker(char *map);
int			map_checker(char *map_filename);
int			map_error(void);
int			argument_error(void);

#endif