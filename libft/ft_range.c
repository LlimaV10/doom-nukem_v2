/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_range.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/31 16:50:41 by ddehtyar          #+#    #+#             */
/*   Updated: 2018/03/31 16:52:10 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	*ft_range(int min, int max)
{
	int	i;
	int	*res;
	int	size;

	i = 0;
	if (min >= max)
		return (0);
	size = (max - min);
	res = (int*)malloc(sizeof(int) * size);
	while (min < max)
	{
		res[i] = min;
		i++;
		min++;
	}
	return (res);
}
