/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/27 15:12:00 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/04/01 16:38:56 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strtrim(char const *s)
{
	char	*tmp;
	char	*s2;
	char	*tmp2;

	if (s == 0)
		return (0);
	tmp = (char *)s;
	while (*tmp != '\0')
		tmp++;
	while ((*tmp == ' ') || (*tmp == '\t') || (*tmp == '\n') || (*tmp == '\0'))
		tmp--;
	while ((*s == ' ') || (*s == '\t') || (*s == '\n'))
		s++;
	if (tmp > s)
		s2 = (char *)malloc((tmp - s + 2) * sizeof(char));
	else
		s2 = (char *)malloc(sizeof(char));
	if (s2 == 0)
		return (0);
	tmp2 = s2;
	if (tmp > s)
		while (s != (tmp + 1))
			*(s2++) = *(s++);
	*s2 = '\0';
	return (tmp2);
}
