/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/27 15:09:23 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/06/17 14:34:38 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*s3;
	char	*tmp;

	if ((s1 != 0) && (s2 != 0))
	{
		s3 = (char *)malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
		if (s3 == 0)
			return (0);
		tmp = s3;
		while (*s1 != '\0')
			*(s3++) = *(s1++);
		while (*s2 != '\0')
			*(s3++) = *(s2++);
		*s3 = '\0';
		return (tmp);
	}
	else if (s1 != 0)
		return (ft_strdup(s1));
	else
		return (ft_strdup(s2));
	return (0);
}
