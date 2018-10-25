/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/27 15:04:59 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/03/27 15:13:36 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memalloc(size_t size)
{
	void	*s;
	char	*tmp;

	s = malloc(size * sizeof(char));
	if (s == 0)
		return (0);
	tmp = s;
	while (size-- > 0)
		*(tmp++) = 0;
	return (s);
}
