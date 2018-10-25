/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/27 18:02:46 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/03/27 18:11:38 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr(int n)
{
	long int nn;
	long int i;

	if (n == 0)
		ft_putchar('0');
	nn = n;
	if (nn < 0)
	{
		ft_putchar('-');
		nn = -nn;
	}
	i = 1;
	while ((nn / i) != 0)
		i *= 10;
	i /= 10;
	while (i >= 1)
	{
		ft_putchar((nn / i) + 48);
		nn -= (nn / i) * i;
		i /= 10;
	}
}
