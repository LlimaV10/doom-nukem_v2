/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/27 15:04:43 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/04/01 20:58:14 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*check2_ft_itoa(int n)
{
	char	*s;

	if (n == -2147483648)
	{
		s = (char *)malloc(12 * sizeof(char));
		s = ft_strcpy(s, "-2147483648");
		return (s);
	}
	if (n == 0)
	{
		s = (char *)malloc(2 * sizeof(char));
		s[0] = '0';
		s[1] = '\0';
		return (s);
	}
	return (0);
}

static char	*check_ft_itoa(int *n, int *len, char *min, long int *k)
{
	char			*s;

	s = check2_ft_itoa(*n);
	if (s != 0)
		return (s);
	if (*n < 0)
	{
		*len = 2;
		*n = -1 * (*n);
		*min = 1;
	}
	else
		*len = 1;
	*k = 1;
	while ((*n / *k) != 0)
	{
		*k *= 10;
		(*len)++;
	}
	return (0);
}

static void	assign_ft_itoa(char *s, int len, int n, long int k)
{
	while (len-- > 0)
	{
		*(s++) = (n / k) + 48;
		n -= (n / k) * k;
		k /= 10;
	}
	*s = '\0';
}

char		*ft_itoa(int n)
{
	char			*s;
	char			*tmp;
	int				len;
	char			min;
	long int		k;

	min = 0;
	s = check_ft_itoa(&n, &len, &min, &k);
	if (s != 0)
		return (s);
	s = (char *)malloc((len--) * sizeof(char));
	if (s == 0)
		return (0);
	tmp = s;
	if (min == 1)
	{
		*s = '-';
		tmp = s;
		len--;
		s++;
	}
	k /= 10;
	assign_ft_itoa(s, len, n, k);
	return (tmp);
}
