/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 14:53:32 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/07/10 17:03:50 by tkempf-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct s_vars
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
}	t_vars;

typedef struct s_dimension
{
	int		x;
	int		y;
}	t_dimension;

void	put_sprite(int x, int y, t_vars *vars, char *sprite_filename)
{
	int		height;
	int		width;

	height = 31;
	width = 31;
	vars->img = mlx_xpm_file_to_image(vars->mlx, sprite_filename, &width, &height);
	mlx_put_image_to_window(vars->mlx, vars->mlx_win, vars->img, x * 31, y * 31);
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

int	close_window(t_vars *vars)
{
	mlx_destroy_image(vars->mlx, vars->img);
	mlx_destroy_window(vars->mlx, vars->mlx_win);
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

void	change_exit_to_open(t_vars *vars)
{
	int		i;
	int		x;
	int		y;

	i = 0;
	x = 0;
	y = 0;
	while (vars->map[i])
	{
		if (vars->map[i] == 'E')
			put_sprite(x, y, vars, "sprites/exit_open.xpm");
		x++;
		if (vars->map[i] == '\n')
		{
			x = 0;
			y++;
		}
		i++;
	}
	exitable(1);
}

void	collectible_progress(t_vars *vars)
{
	int			i;
	int			collectible_nbr;

	i = 0;
	collectible_nbr = 0;
	while (vars->map[i])
	{
		if (vars->map[i] == 'C')
			collectible_nbr++;
		i++;
	}
	if (collectible_nbr == 0)
	{
		change_exit_to_open(vars);
		exitable(1);
	}
}

int	movetester(t_vars *vars, int x_tested, int y_tested)
{
	int		i;

	i = 0;
	while (y_tested > 0)
	{
		if (vars->map[i] == '\n')
			y_tested--;
		i++;
	}
	i += x_tested;
	if (vars->map[i] == 'C')
	{
		vars->map[i] = '0';
		collectible_progress(vars);
	}
	if (vars->map[i] == '0' || vars->map[i] == 'C' || vars->map[i] == 'P')
		return (0);
	else if (vars->map[i] == 'E' && exitable(0) == 1)
		close_window(vars);
	return (-1);
}

void	moveup(t_vars *vars)
{
	if (movetester(vars, vars->player_position_x, vars->player_position_y - 1) == 0)
	{
		put_sprite(vars->player_position_x, vars->player_position_y, vars, "sprites/ground.xpm");
		vars->player_position_y--;
		put_sprite(vars->player_position_x, vars->player_position_y, vars, "sprites/player.xpm");
	}
}

void	movedown(t_vars *vars)
{
	if (movetester(vars, vars->player_position_x, vars->player_position_y + 1) == 0)
	{
		put_sprite(vars->player_position_x, vars->player_position_y, vars, "sprites/ground.xpm");
		vars->player_position_y++;
		put_sprite(vars->player_position_x, vars->player_position_y, vars, "sprites/player.xpm");
	}
}

void	moveright(t_vars *vars)
{
	if (movetester(vars, vars->player_position_x + 1, vars->player_position_y) == 0)
	{
		put_sprite(vars->player_position_x, vars->player_position_y, vars, "sprites/ground.xpm");
		vars->player_position_x++;
		put_sprite(vars->player_position_x, vars->player_position_y, vars, "sprites/player.xpm");
	}
}

void	moveleft(t_vars *vars)
{
	if (movetester(vars, vars->player_position_x - 1, vars->player_position_y) == 0)
	{
		put_sprite(vars->player_position_x, vars->player_position_y, vars, "sprites/ground.xpm");
		vars->player_position_x--;
		put_sprite(vars->player_position_x, vars->player_position_y, vars, "sprites/player.xpm");
	}
}

int key_hook(int keycode, t_vars *vars)
{
	if (keycode == 13)
		moveup(vars);
	else if (keycode == 1)
		movedown(vars);
	else if (keycode == 2)
		moveright(vars);
	else if (keycode == 0)
		moveleft(vars);
	if (keycode == 53)
		close_window(vars);
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
void	initialisation(t_vars *vars, t_dimension dimension)
{
	int		fd;

	fd = open(vars->map_filename, O_RDWR);
	vars->map = get_file(vars->map_filename);
	close (fd);
	vars->mlx = mlx_init();
	vars->mlx_win = mlx_new_window(vars->mlx, dimension.x * 32 - dimension.x, dimension.y * 32 - dimension.y, "so_long");
	vars->img = mlx_new_image(vars->mlx, dimension.x * 32 - dimension.x, dimension.y * 32 - dimension.y);
	vars->addr = mlx_get_data_addr(vars->img, &vars->bits_per_pixel, &vars->line_length, &vars->endian);
}

/*
ft_parser :
- puts the right sprites at the right coordinates according to the map
*/
void	ft_parser(t_vars *vars, t_dimension dimension)
{
	int		i;
	int		x;
	int		y;
	
	i = 0;
	x = 0;
	y = 0;
	while (vars->map[i])
	{
		if (vars->map[i] == '0')
			put_sprite(x , y, vars, "sprites/ground.xpm");
		else if (vars->map[i] == '1')
			put_sprite(x, y, vars, "sprites/wall.xpm");
		else if (vars->map[i] == 'C')
			put_sprite(x, y, vars, "sprites/collectible.xpm");
		else if (vars->map[i] == 'E')
			put_sprite(x, y, vars, "sprites/exit_close.xpm");
		else if (vars->map[i] == 'P')
		{
			put_sprite(x, y, vars, "sprites/player.xpm");
			vars->player_position_x = x;
			vars->player_position_y = y;
		}
		else if (vars->map[i] == '\n')
		{
			x = -1;
			y++;
		}
		x++;
		i++;
	}
}

int	charchecker(char c)
{
	if (c != '0' && c != '1' && c != 'C' && c != 'E' && c != 'P')
		return (-1);
}

int	contentchecker(char *line)
{
	static int		exit = 0;
	static int		item = 0;
	static int		spawn = 0;
	int		i;

	i = 0;
	while (line[i])
	{
		if (line[i] == 'E')
			exit++;
		else if (line[i] == 'C')
			item++;
		else if (line[i] == 'P')
			spawn++;
		i++;
	}
	if (spawn > 0 && exit > 0 && item > 0)
		return (0);
	return (-1);
}

int	limwallchecker(char *line)
{
	int		i;

	i = 0;
	while (line[i])
	{
		if (line[i] != '1' && line[i] != '\n')
			return (-1);
		i++;
	}
	return (0);
}

int	sidewallchecker(char *line)
{
	if (line[0] == '1' && line[ft_strlen(line) - 2] == '1')
		return (0);
	else
		return (-1);
}

int	mapchecker(char *mapname)
{
	char	*line;
	char	*endline;
	int		fd;
	int		i;

	i = 0;
	fd = open(mapname, O_RDONLY);
	line = get_next_line(fd);
	if (limwallchecker(line) == -1)
	{
		free (line);
		close(fd);
		return (-1);
	}
	endline = line;
	while (ft_strlen(line) == ft_strlen(endline))
	{
		endline = line;
		line = get_next_line(fd);
		contentchecker(line);
		if (sidewallchecker(line) == -1)
		{
			free (line);
			close(fd);
			return (-1);
		}
	}
	close(fd);
	if (limwallchecker(line) == -1 || contentchecker(endline) == -1)
	{
		free (line);
		free (endline);
		return (-1);
	}
	free (line);
	free (endline);
	return (0);
}

// faire ending
//clean code	check leaks
//tout commenter bien
int main(int argc, char *argv[])
{
	t_vars		vars;
	t_dimension	dimension;
	
	if (argc != 2)
		return (EXIT_FAILURE);
	if (mapchecker(argv[1]) == -1)// a normer
		return (EXIT_FAILURE);
	vars.map_filename = argv[1];
	set_dimension(vars.map_filename, &dimension);//clean
	initialisation(&vars, dimension);//clean
	ft_parser(&vars, dimension);// a normer
	mlx_hook(vars.mlx_win, 2, 1L<<0, key_hook, &vars);
	mlx_hook(vars.mlx_win, 17, 0, close_window, &vars);
	mlx_loop(vars.mlx);
	return (0);
}