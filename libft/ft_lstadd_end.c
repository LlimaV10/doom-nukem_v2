/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_end.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/06 18:16:20 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/04/06 18:20:19 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_end(t_list **alst, t_list *new)
{
	if (alst != 0)
	{
		if (*alst == 0)
			*alst = new;
		else
		{
			while ((*alst)->next != 0)
				*alst = (*alst)->next;
			(*alst)->next = new;
		}
	}
}
