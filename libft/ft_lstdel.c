/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/02 18:36:39 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/04/19 19:42:50 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdel(t_list **alst, void (*del)(void *, size_t))
{
	if ((alst == NULL) || (del == NULL))
		return ;
	if (*alst == NULL)
		return ;
	if ((*alst)->next != 0)
		ft_lstdel(&((*alst)->next), del);
	ft_lstdelone(alst, del);
}
