/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 00:03:01 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/07/11 00:54:58 by tkempf-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/so_long.h"

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
