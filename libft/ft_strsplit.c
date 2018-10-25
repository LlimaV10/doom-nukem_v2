/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/27 15:11:30 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/06/17 17:56:16 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	ft_count_splitwords(char const *s, char c)
{
	unsigned int	i;

	if (*s == '\0')
		return (-2);
	i = 0;
	while (*s != '\0')
	{
		while (*s == c)
			s++;
		if (*s != '\0')
		{
			while ((*s != c) && (*s != '\0'))
				s++;
			i++;
		}
	}
	return (i);
}

static char			**ft_mem_splitwords(char const *s, char c)
{
	char			**spl;
	unsigned int	size;
	char			**tmp;

	spl = (char **)malloc((ft_count_splitwords(s, c) + 1) * sizeof(char *));
	if (spl == 0)
		return (0);
	tmp = spl;
	while (*s != '\0')
	{
		while ((*s == c) && (*s != '\0'))
			s++;
		size = 0;
		if (*s != '\0')
		{
			while ((*s != c) && (*s != '\0'))
			{
				s++;
				size++;
			}
			(*spl++) = (char *)malloc((size + 1) * sizeof(char));
		}
	}
	return (tmp);
}

char				**ft_strsplit(char const *s, char c)
{
	char			**spl;
	char			**tmp;
	unsigned int	i;

	if ((s == 0) || ((spl = ft_mem_splitwords(s, c)) == 0))
		return (0);
	tmp = spl;
	while (*s != '\0')
	{
		while ((*s == c) && (*s != '\0'))
			s++;
		if (*s != '\0')
		{
			i = 0;
			while ((*s != c) && (*s != '\0'))
				spl[0][i++] = *(s++);
			(spl++)[0][i] = '\0';
			if (*s == '\0')
				*spl = 0;
		}
		else
			*(spl++) = 0;
	}
	return (tmp);
}
