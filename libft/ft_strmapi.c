/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/27 15:10:01 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/04/01 15:44:21 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*s2;
	char			*tmp;
	unsigned int	i;

	if ((s != 0) && (f != 0))
	{
		s2 = (char *)malloc((ft_strlen(s) + 1) * sizeof(char));
		if (s2 == 0)
			return (0);
		i = 0;
		tmp = s2;
		while (*s != '\0')
			*(s2++) = f(i++, *(s++));
		*s2 = '\0';
		return (tmp);
	}
	return (0);
}
