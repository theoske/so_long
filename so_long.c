/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 14:53:32 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/07/10 23:57:05 by tkempf-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

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
