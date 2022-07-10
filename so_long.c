/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 14:53:32 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/07/10 19:19:38 by tkempf-e         ###   ########.fr       */
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
	data->img = mlx_xpm_file_to_image(data->mlx, sprite_filename, &width, &height);
	mlx_put_image_to_window(data->mlx, data->mlx_win, data->img, x * 31, y * 31);
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

// option == 0 to check
// option == 1 to set to open exit
int		exitable(int option)
{
	static int	exitable = 0;
	
	if (option == 1)
		exitable = 1;
	return (exitable);
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
	exitable(1);
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
		exitable(1);
	}
}

int	movetester(t_data *data, int x_tested, int y_tested)
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
	else if (data->map[i] == 'E' && exitable(0) == 1)
		close_window(data);
	return (-1);
}

void	moveup(t_data *data)
{
	if (movetester(data, data->player_position_x, data->player_position_y - 1) == 0)
	{
		put_sprite(data->player_position_x, data->player_position_y, data, "sprites/ground.xpm");
		data->player_position_y--;
		put_sprite(data->player_position_x, data->player_position_y, data, "sprites/player.xpm");
	}
}

void	movedown(t_data *data)
{
	if (movetester(data, data->player_position_x, data->player_position_y + 1) == 0)
	{
		put_sprite(data->player_position_x, data->player_position_y, data, "sprites/ground.xpm");
		data->player_position_y++;
		put_sprite(data->player_position_x, data->player_position_y, data, "sprites/player.xpm");
	}
}

void	moveright(t_data *data)
{
	if (movetester(data, data->player_position_x + 1, data->player_position_y) == 0)
	{
		put_sprite(data->player_position_x, data->player_position_y, data, "sprites/ground.xpm");
		data->player_position_x++;
		put_sprite(data->player_position_x, data->player_position_y, data, "sprites/player.xpm");
	}
}

void	moveleft(t_data *data)
{
	if (movetester(data, data->player_position_x - 1, data->player_position_y) == 0)
	{
		put_sprite(data->player_position_x, data->player_position_y, data, "sprites/ground.xpm");
		data->player_position_x--;
		put_sprite(data->player_position_x, data->player_position_y, data, "sprites/player.xpm");
	}
}

int key_hook(int keycode, t_data *data)
{
	if (keycode == 13)
		moveup(data);
	else if (keycode == 1)
		movedown(data);
	else if (keycode == 2)
		moveright(data);
	else if (keycode == 0)
		moveleft(data);
	if (keycode == 53)
		close_window(data);
	return (0);
}

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
- transfer the map file to the map string
- creates the game window with the dimension of the map that we got from the set_dimension function
*/
void	initialisation(t_data *data, t_dimension dimension)
{
	int		fd;

	fd = open(data->map_filename, O_RDWR);
	data->map = get_file(data->map_filename);
	close (fd);
	data->mlx = mlx_init();
	data->mlx_win = mlx_new_window(data->mlx, dimension.x * 32 - dimension.x, dimension.y * 32 - dimension.y, "so_long");
	data->img = mlx_new_image(data->mlx, dimension.x * 32 - dimension.x, dimension.y * 32 - dimension.y);
	data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel, &data->line_length, &data->endian);
}

/*
ft_parser :
- puts the right sprites at the right coordinates according to the map
*/
void	ft_parser(t_data *data, t_dimension dimension)
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
			put_sprite(x , y, data, "sprites/ground.xpm");
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

int	contentchecker(char *map)
{
	if (exit_on_map(map) == -1 || collectible_on_map(map) == -1
		|| spawn_on_map(map) == -1)
		return (-1);
	return (0);
}

// check if the first and last lines of the map are walls as it should be
int	limwallchecker(char *map)
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

// check if each line of the map is surrounded by a wall
int	sidewallchecker(char *map)
{
	int		i;
	
	i = 0;
	while (map[i + 1])
	{
		if (map[i] == '\n' && (map[i - 1] != '1' || map[i + 1] != '1'))
			return (-1);
		i++;
	}
	if (map[i] != '1')
		return (-1);
	return (0);
}

int	rectanglemapchecker(char *map)
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

int	mapchecker(char *map_filename)
{
	char	*map;

	map = get_file(map_filename);
	if (!map)
		return (-1);
	if (limwallchecker(map) == -1 || contentchecker(map) == -1
		|| sidewallchecker(map) == -1 || rectanglemapchecker(map) == -1)
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

// gerer plusieurs spawn
// clean code	check leaks
// tout commenter bien
int main(int argc, char *argv[])
{
	t_data		data;
	t_dimension	dimension;
	
	if (argc != 2)
		return (EXIT_FAILURE);
	if (mapchecker(argv[1]) == -1)// a normer
		return (map_error());
	data.map_filename = argv[1];
	set_dimension(data.map_filename, &dimension);//clean
	initialisation(&data, dimension);//clean
	ft_parser(&data, dimension);// a normer
	mlx_hook(data.mlx_win, 2, 1L<<0, key_hook, &data);
	mlx_hook(data.mlx_win, 17, 0, close_window, &data);
	mlx_loop(data.mlx);
	return (0);
}