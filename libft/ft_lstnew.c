/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/02 18:35:17 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/04/06 16:30:50 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void const *content, size_t content_size)
{
	t_list	*tmp;

	tmp = (t_list *)malloc(sizeof(t_list));
	if (tmp == 0)
		return (0);
	tmp->content_size = 0;
	if (content == 0)
		tmp->content = 0;
	else
	{
		tmp->content = (void *)malloc(content_size);
		if (tmp->content == 0)
			return (0);
		ft_memcpy(tmp->content, content, content_size);
		tmp->content_size = content_size;
	}
	tmp->next = 0;
	return (tmp);
}
