/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/27 18:15:50 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/03/27 19:03:03 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	long int nn;
	long int i;

	if (n == 0)
		ft_putchar_fd('0', fd);
	nn = n;
	if (nn < 0)
	{
		ft_putchar_fd('-', fd);
		nn = -nn;
	}
	i = 1;
	while ((nn / i) != 0)
		i *= 10;
	i /= 10;
	while (i >= 1)
	{
		ft_putchar_fd((nn / i) + 48, fd);
		nn -= (nn / i) * i;
		i /= 10;
	}
}
