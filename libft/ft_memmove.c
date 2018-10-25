/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/19 16:05:30 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/04/19 16:05:31 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char	*d;
	char	*s;
	size_t	i;

	d = (char *)dest;
	s = (char *)src;
	i = 0;
	if (d < s)
		while (i < n)
		{
			d[i] = s[i];
			i++;
		}
	else
		while (i < n)
		{
			d[n - i - 1] = s[n - i - 1];
			i++;
		}
	return (dest);
}
