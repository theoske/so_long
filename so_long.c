/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 14:53:32 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/07/09 16:07:49 by tkempf-e         ###   ########.fr       */
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
	int		posx;
	int		posy;
	char	*filename;
}	t_vars;

typedef struct s_dimension
{
	int		x;
	int		y;
}	t_dimension;

void	put_sprite(int x, int y, t_vars *vars, char *filename)
{
	int		height;
	int		width;

	height = 32;
	width = 32;
	vars->img = mlx_xpm_file_to_image(vars->mlx, filename, &width, &height);
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

char	*get_file(const char *filename)
{
	int		fd;
	char	*join;
	char	*map;

	map = NULL;
	join = NULL;
	fd = open(filename, O_RDONLY);
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

int	collectible_progress() // compte les C restatns et ouvre porte si besoin
{
	
}

int	movetester(t_vars *vars, int x_tested, int y_tested)
{
	char	*map;
	char	c;
	int		i;

	i = 0;
	map = get_file(vars->filename);
	while (y_tested > 0)
	{
		if (map[i] == '\n')
			y_tested--;
		i++;
	}
	c = map[i + x_tested];
	free (map);
	if (c == 'C')
		collectible_progress();
	if (c == '0' || c == 'C' || c == 'P')
		return (0);
	else
		return (-1);
}

void	moveup(t_vars *vars)
{
	if (movetester(vars, vars->posx, vars->posy - 1) == 0)
	{
		put_sprite(vars->posx, vars->posy, vars, "sprites/ground.xpm");
		vars->posy--;
		put_sprite(vars->posx, vars->posy, vars, "sprites/player.xpm");
	}
}

void	movedown(t_vars *vars)
{
	if (movetester(vars, vars->posx, vars->posy + 1) == 0)
	{
		put_sprite(vars->posx, vars->posy, vars, "sprites/ground.xpm");
		vars->posy++;
		put_sprite(vars->posx, vars->posy, vars, "sprites/player.xpm");
	}
}

void	moveright(t_vars *vars)
{
	if (movetester(vars, vars->posx + 1, vars->posy) == 0)
	{
		put_sprite(vars->posx, vars->posy, vars, "sprites/ground.xpm");
		vars->posx++;
		put_sprite(vars->posx, vars->posy, vars, "sprites/player.xpm");
	}
}

void	moveleft(t_vars *vars)
{
	if (movetester(vars, vars->posx - 1, vars->posy) == 0)
	{
		put_sprite(vars->posx, vars->posy, vars, "sprites/ground.xpm");
		vars->posx--;
		put_sprite(vars->posx, vars->posy, vars, "sprites/player.xpm");
	}
}

int key_hook(int keycode, t_vars *vars)
{
	if (keycode == 13)//up
		moveup(vars);
	else if (keycode == 1)//down
		movedown(vars);
	else if (keycode == 2)//right
		moveright(vars);
	else if (keycode == 0)//left
		moveleft(vars);
	// mlx_put_image_to_window(vars->mlx, vars->mlx_win, vars->img, x, y);
	if (keycode == 53)
	{
		mlx_destroy_image(vars->mlx, vars->img);
		mlx_destroy_window(vars->mlx, vars->mlx_win);
		exit(EXIT_SUCCESS);
	}
	return (0);
}

void	ft_dimension(char *tab, t_dimension *dimension)
{
	int		i;
	int		fd;
	char	*map;


	i = 0;
	dimension->x = 0;
	fd = open(tab, O_RDONLY);
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

void	initialisation(t_vars *vars, t_dimension dimension)
{
	vars->mlx = mlx_init();
	vars->mlx_win = mlx_new_window(vars->mlx, dimension.x * 32 - dimension.x, dimension.y * 32 - dimension.y, "so_long");
	vars->img = mlx_new_image(vars->mlx, dimension.x * 32 - dimension.x, dimension.y * 32 - dimension.y);
	vars->addr = mlx_get_data_addr(vars->img, &vars->bits_per_pixel, &vars->line_length, &vars->endian);
}

void	color_img(t_vars *vars, int length, int height, int color)
{
	char	*dst;
	int		x;
	int		y;
	
	x = 0;
	while (x <= length)
	{
		y = 0;
		while (y <= height)
		{
			dst = vars->addr + (y * vars->line_length + x * (vars->bits_per_pixel / 8));
			*(unsigned int*)dst = color;
			y++;
		}
		x++;
	}
}

void	ft_parser(const char *arg, t_vars *vars, t_dimension dimension)
{
	char	*tab;
	char	*join;
	int		i;
	int		x;
	int		y;
	
	i = open(arg, O_RDONLY);
	tab = NULL;
	join = get_next_line(i);
	while (join)
	{
		tab = ft_strjoin(tab, join);
		join = get_next_line(i);
	}
	i = 0;
	x = 0;
	y = 0;
	while (tab[i])
	{
		if (tab[i] == '0')
			put_sprite(x , y, vars, "sprites/ground.xpm");
		else if (tab[i] == '1')
			put_sprite(x, y, vars, "sprites/wall.xpm");
		else if (tab[i] == 'C')
			put_sprite(x, y, vars, "sprites/collectible.xpm");
		else if (tab[i] == 'E')
			put_sprite(x, y, vars, "sprites/exit_close.xpm");
		else if (tab[i] == 'P')
		{
			put_sprite(x, y, vars, "sprites/player.xpm");
			vars->posx = x;
			vars->posy = y;
		}
		else if (tab[i] == '\n')
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

int main(int argc, char *argv[])
{
	t_vars		vars;
	char		*map;
	t_dimension	dimension;
	
	if (argc != 2)
		return (1);
	if (mapchecker(argv[1]) == -1)//v1 works
	{
		printf("not working\n");
		return (-1);
	}
	else
		printf("working\n");
	ft_dimension(argv[1], &dimension);
	initialisation(&vars, dimension);
	ft_parser(argv[1], &vars, dimension);
	// color_img(&vars, 64, 64, 10760863);
	vars.filename = argv[1];
	mlx_hook(vars.mlx_win, 2, 1L<<0, key_hook, &vars);
	mlx_loop(vars.mlx);
	return (0);
}