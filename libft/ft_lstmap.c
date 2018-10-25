/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/02 18:37:32 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/04/19 19:41:07 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list	*new;
	t_list	*start;

	if ((lst == 0) || (f == 0))
		return (0);
	start = f(lst);
	if (start == 0)
		return (0);
	new = start;
	while (lst->next != 0)
	{
		new->next = f(lst->next);
		if (new->next == 0)
		{
			ft_lstfree(start);
			return (0);
		}
		new = new->next;
		lst = lst->next;
	}
	return (start);
}
