/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/19 16:07:42 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/04/19 17:22:39 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *hay, const char *nee)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (nee[0] == '\0')
		return ((char *)hay);
	while (hay[i] != '\0')
	{
		j = 0;
		while ((nee[j] != '\0') && (hay[i + j] == nee[j]))
			j++;
		if (nee[j] == '\0')
			return ((char *)hay + i);
		i++;
	}
	return (NULL);
}
