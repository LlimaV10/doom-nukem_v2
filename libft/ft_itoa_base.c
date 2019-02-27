/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/31 18:18:08 by ddehtyar          #+#    #+#             */
/*   Updated: 2018/04/01 17:08:40 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char		*ft_base(int b, long int qwe, int base)
{
	long int	a;
	int			i;
	int			iter;
	char		*let;
	char		*c;

	let = "0123456789ABCDEF";
	a = 1;
	i = 1;
	iter = 0;
	while (qwe / a != 0 && (i++ >= 0))
		a = a * base;
	a = a / base;
	c = (char*)malloc(i + b);
	if (b == 1)
		c[0] = '-';
	while (iter < i - 1)
	{
		c[iter + b] = let[(qwe / a) % base];
		a = a / base;
		iter++;
	}
	c[iter + b] = '\0';
	return (c);
}

char			*ft_itoa_base(int value, int base)
{
	char		*c;
	long int	qwe;
	int			b;

	qwe = value;
	b = 0;
	if (base > 16 && base < 2)
		return (0);
	if (qwe == 0)
	{
		c = (char*)malloc(2);
		c[0] = '0';
		c[1] = '\0';
		return (c);
	}
	if (base != 10 && qwe < 0)
		qwe = -qwe;
	if (base == 10 && qwe < 0)
	{
		b = 1;
		qwe = -qwe;
	}
	c = ft_base(b, qwe, base);
	return (c);
}
