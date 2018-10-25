/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/27 15:11:10 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/04/01 15:37:51 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnew(size_t size)
{
	char	*s;
	char	*tmp;

	s = (char *)malloc(((int)size + 1) * sizeof(char));
	if (s == 0)
		return (0);
	tmp = s;
	while (size-- > 0)
		*(s++) = '\0';
	*s = '\0';
	return (tmp);
}
