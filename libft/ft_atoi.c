/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/19 16:22:43 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/04/19 16:42:49 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_atoi(const char *s)
{
	unsigned long long	num;
	size_t				i;
	int					sign;

	i = 0;
	num = 0;
	sign = 1;
	while (((s[i] >= 9) && (s[i] <= 13)) || (s[i] == ' '))
		i++;
	if (s[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (s[i] == '+')
		i++;
	while ((s[i] >= '0') && (s[i] <= '9'))
		num = num * 10 + s[i++] - '0';
	return ((int)num * sign);
}
