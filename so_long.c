/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 14:53:32 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/07/10 23:03:16 by tkempf-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

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

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

void	ft_putstr(char *s)
{
	int		i;

	i = 0;
	while (s[i])
	{
		write(1, &s[i], 1);
		i++;
	}
}

/*
ft_strjoin :
- Takes 2 char strings.
- Joins the 2 strings in a 3rd one and returns it.
*/
char	*ft_strjoin(char *s1, char *s2)
{
	char	*join;
	int		i;
	int		j;

	j = 0;
	i = 0;
	join = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!join)
		return (NULL);
	while (s1 && s1[i])
	{
		join[i] = *(char *)(s1 + i);
		i++;
	}
	while (s2[j])
	{
		join[i + j] = *(char *)(s2 + j);
		j++;
	}
	join[i + j] = '\0';
	free(s1);
	return (join);
}

char	*ft_line(char *line, char *buffer, int octet, int fd)
{
	while (buffer[0] != '\n' && octet > 0)
	{
		octet = read(fd, buffer, 1);
		buffer[octet] = '\0';
		line = ft_strjoin(line, buffer);
	}
	return (line);
}

/*
get_next_line :
- Takes a file descriptor as an argument and return the
	first line of the file in a char string.
- Each call of this function return the next line of the file.
*/
char	*get_next_line(int fd)
{
	char		buffer[2];
	char		*line;
	int			octet;

	line = NULL;
	octet = 0;
	if (octet == 0)
	{
		octet = read(fd, buffer, 1);
		buffer[octet] = '\0';
		line = ft_strjoin(line, buffer);
	}
	if (octet > 0 && buffer[0] != '\0')
		line = ft_line(line, buffer, octet, fd);
	else
	{
		free(line);
		return (NULL);
	}
	return (line);
}

long int	ft_numbers(long int n)
{
	long int	numbers;

	numbers = 1;
	if (n == 0)
		numbers = 2;
	if (n < 0)
		numbers++;
	while (n != 0)
	{
		n /= 10;
		numbers++;
	}
	return (numbers);
}

char	*filling(long int n, char *tab, long int j, long int numbers)
{
	if (n == 0)
	{
		tab[j] = 48;
		tab[j + 1] = '\0';
		return (tab);
	}
	while (n > 0)
	{
		numbers = n / 10;
		tab[j] = n - (numbers * 10) + 48;
		n /= 10;
		j--;
	}
	return (tab);
}

char	*ft_itoa(int n)
{
	char		*tab;
	long int	i;
	long int	j;
	long int	numbers;

	i = 0;
	numbers = ft_numbers((long int)n);
	tab = malloc(sizeof(char) * numbers);
	if (!tab)
		return (NULL);
	j = numbers - 2;
	tab[numbers - 1] = '\0';
	tab = filling((long int)n, tab, j, numbers);
	return (tab);
}

/*
get_file :
- Puts the entire map from the argument file in the map string with 
	the get_next_line and ft_strjoin functions.
*/
char	*get_file(const char *map_filename)
{
	int		fd;
	char	*join;
	char	*map;

	map = NULL;
	join = NULL;
	fd = open(map_filename, O_RDWR);
	join = get_next_line(fd);
	while (join)
	{
		map = ft_strjoin(map, join);
		join = get_next_line(fd);
	}
	close (fd);
	free (join);
	return (map);
}

int	close_window(t_data *data)
{
	free (data->map);
	mlx_destroy_image(data->mlx, data->img);
	mlx_destroy_window(data->mlx, data->mlx_win);
	exit(EXIT_SUCCESS);
	return (0);
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

int	content_checker(char *map)
{
	if (exit_on_map(map) == -1 || collectible_on_map(map) == -1
		|| spawn_on_map(map) == -1)
		return (-1);
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

int	map_error(void)
{
	ft_putstr("Error\nInvalid map\n");
	return (-1);
}

int	argument_error(void)
{
	ft_putstr("Error\nInvalid number of arguments\n");
	return (-1);
}

/*
so_long :
- Checks if there is only 1 argument (the map file name).
- Checks if the map respect the rules of the subject.
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
