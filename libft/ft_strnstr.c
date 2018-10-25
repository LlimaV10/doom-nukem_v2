/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/19 16:07:23 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/04/19 16:59:36 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *hay, const char *nee, size_t l)
{
	size_t	i;
	size_t	j;

	i = 0;
	while ((hay[i] != '\0') && (i < l))
	{
		j = 0;
		while ((nee[j] != '\0') && (hay[i + j] == nee[j]) && ((i + j) < l))
			j++;
		if (nee[j] == '\0')
			return ((char *)hay + i);
		i++;
	}
	return (NULL);
}
