/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/27 15:09:49 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/04/01 15:43:46 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmap(char const *s, char (*f)(char))
{
	char	*s2;
	char	*tmp;

	if ((s != 0) && (f != 0))
	{
		s2 = (char *)malloc((ft_strlen(s) + 1) * sizeof(char));
		if (s2 == 0)
			return (0);
		tmp = s2;
		while (*s != '\0')
			*(s2++) = f(*(s++));
		*s2 = '\0';
		return (tmp);
	}
	return (0);
}
