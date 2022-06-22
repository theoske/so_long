/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 14:53:32 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/06/22 16:20:13 by tkempf-e         ###   ########.fr       */
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
	mlx_put_image_to_window(vars->mlx, vars->mlx_win, vars->img, x, y);
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
	vars->mlx_win = mlx_new_window(vars->mlx, 1280, 720, "so_long");
	vars->img = mlx_new_image(vars->mlx, 100, 100);
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

void	put_ground(int x, int y, t_vars *vars)
{
	int		height;
	int		width;

	height = 64;
	width = 64;
	vars->img = mlx_xpm_file_to_image(vars->mlx, "ground/groundflower.xpm", &width, &height);
	mlx_put_image_to_window(vars->mlx, vars->mlx_win, vars->img, x, y);
}

void	ft_parser(char *tab, t_vars *vars)
{
	int		i;
	int		x;
	int		y;
	
	i = 0;
	x = 0;
	y = 0;
	while (tab[i])
	{
		if (tab[i] == '0')
			put_ground(x, y, vars);
		// else if (tab[i] == 1)
		// 	put_wall(x, y, vars);
		// else if (tab[i] == 'C')
		// 	put_item(x, y, vars);
		// else if (tab[i] == 'E')
		// 	put_exit(x, y, vars);
		// else if (tab[i] == 'P')
		// 	put_spawn(x, y, vars);
		// else if (tab[i] == '\n')
		// {
		// 	x = 0;
		// 	y += 99;
		// }
		x += 63;
		i++;
	}
}

int	charchecker(char c)
{
	if (c != '0' && c != '1' && c != 'C' && c != 'E' && c != 'P')
		return (-1);
}

int	contentchecker(char c)
{
	static int	exit = 0;
	static int	item = 0;
	static int	spawn = 0;

	if (c == 'E')
		exit++;
	else if (c == 'C')
		item++;
	else if (c == 'P')
		spawn++;
	if (spawn > 0 && exit > 0 && item > 0)
		return (0);
	else
		return (-1);
}

int	mapchecker(char *mapname)//remplacer par utilisation gnl
{
	int		fd;
	int		octet;
	char	buff[2];
	int		rect[2];
	char	*map;

	fd = open(mapname, O_RDONLY);
	if (fd == -1)
		perror("erreur ouverture map");
	octet = 1;
	rect[0] = 0;
	rect[1] = 0;
	while (octet == 1)
	{
		
		octet = read(fd, buff, 1);
		buff[1] = 0;
		if (charchecker(buff[0]) == -1)
		{
			close(fd);
			return (-1);
		}
		if (buff[0] != '\n')
			rect[0]++;
		else
		{
			if (rect[1] != rect[0] && rect[1] != 0)//si taille ligne i != taille ligne i-1
				return (-1);
			rect[1] = rect[0];
			rect[0] = 0;
		}
		contentchecker(buff[0]);
	}
	close (fd);
	if (contentchecker('2') == -1)
		return (-1);
	return (0);
}

int main(int argc, char *argv[])//utiliser images
{
	t_vars	vars;
	char	*map;
	
	if (argc != 2)
		return (1);
	if (mapchecker(argv[1]) == -1)
		return (-1);
	initialisation(&vars);
	ft_parser(argv[1], &vars);
	// color_img(&vars, 50, 50, 10760863);
	mlx_hook(vars.mlx_win, 2, 1L<<0, key_hook, &vars);
	mlx_loop(vars.mlx);
	return (0);
}