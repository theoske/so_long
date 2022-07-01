/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 14:53:32 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/07/01 16:42:13 by tkempf-e         ###   ########.fr       */
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
}	t_vars;

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

int key_hook(int keycode, t_vars *vars)
{
	static int	x = 0;
	static int	y = 0;

	if (keycode == 13)
		y -= 63;
	else if (keycode == 1)
		y += 63;
	else if (keycode == 2)
		x += 63;
	else if (keycode == 0)
		x -= 63;
	// mlx_put_image_to_window(vars->mlx, vars->mlx_win, vars->img, x, y);
	if (keycode == 53)
	{
		mlx_destroy_image(vars->mlx, vars->img);
		mlx_destroy_window(vars->mlx, vars->mlx_win);
		exit(EXIT_SUCCESS);
	}
	return (0);
}

void	initialisation(t_vars *vars)
{
	vars->mlx = mlx_init();
	vars->mlx_win = mlx_new_window(vars->mlx, 1080, 1080, "so_long");
	vars->img = mlx_new_image(vars->mlx, 1080, 1080);
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

void	put_sprite(int x, int y, t_vars *vars, char *filename)
{
	int		height;
	int		width;

	height = 64;
	width = 64;
	vars->img = mlx_xpm_file_to_image(vars->mlx, filename, &width, &height);
	mlx_put_image_to_window(vars->mlx, vars->mlx_win, vars->img, x, y);
}

void	ft_parser(const char *arg, t_vars *vars)
{
	char	*tab;
	char	*join;
	int		i;
	int		x;
	int		y;
	
	i = open(arg, O_RDONLY);
	tab = 0;
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
			put_sprite(x, y, vars, "ground/groundflower.xpm");
		else if (tab[i] == '1')
			put_sprite(x, y, vars, "wall/wallup.xpm");
		else if (tab[i] == 'C')
			put_sprite(x, y, vars, "item/item.xpm");
		else if (tab[i] == 'E')
			put_sprite(x, y, vars, "exit/exitclose.xpm");
		else if (tab[i] == 'P')
			put_sprite(x, y, vars, "player/player1.xpm");
		else if (tab[i] == '\n')
		{
			x = -63;
			y += 63;
		}
		x += 63;
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
/*
	finir affichage de la map bien :
		-faire personnage.
		-centrer map dans la fenetre.
		-mettre eau.
*/
int main(int argc, char *argv[])
{
	t_vars	vars;
	char	*map;
	
	if (argc != 2)
		return (1);
	if (mapchecker(argv[1]) == -1)//v1 works
	{
		printf("not working\n");
		return (-1);
	}
	else
		printf("working\n");
	initialisation(&vars);
	ft_parser(argv[1], &vars);
	// color_img(&vars, 64, 64, 10760863);
	mlx_hook(vars.mlx_win, 2, 1L<<0, key_hook, &vars);
	mlx_loop(vars.mlx);
	return (0);
}