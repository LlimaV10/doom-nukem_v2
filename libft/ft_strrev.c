/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrev.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/06 17:40:11 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/04/06 17:40:20 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrev(char *str)
{
	char	*b;
	char	*e;
	char	cmp;

	b = str;
	e = str;
	while (*e)
		e++;
	e--;
	while (b < e)
	{
		cmp = *b;
		*b = *e;
		*e = cmp;
		b++;
		e--;
	}
	return (str);
}
