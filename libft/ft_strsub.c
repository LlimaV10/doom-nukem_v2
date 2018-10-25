/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/27 15:11:47 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/04/01 16:20:03 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
	char	*s2;
	char	*tmp;

	if (s != 0)
	{
		s2 = (char *)malloc((len + 1) * sizeof(char));
		if (s2 == 0)
			return (0);
		tmp = s2;
		while (start-- > 0)
			s++;
		while (len-- > 0)
			*(s2++) = *(s++);
		*s2 = '\0';
		return (tmp);
	}
	return (0);
}
