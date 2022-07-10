/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itoa.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 23:55:59 by tkempf-e          #+#    #+#             */
/*   Updated: 2022/07/11 00:10:48 by tkempf-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

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
