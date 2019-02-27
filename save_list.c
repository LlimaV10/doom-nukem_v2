/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/04 13:02:37 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/02/04 13:02:38 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void	save_list_help(t_doom *den, t_col *vec)
{
	if (den->button == 1)
	{
		ft_lstnewmap(&den->tmp, vec->x1, vec->y1, den);
		ft_lstnewmap_sec(&den->tmp, vec->x2, vec->y2, den);
	}
	else
	{
		ft_lstnewmap(&den->tmp, vec->x2, vec->y2, den);
		ft_lstnewmap_sec(&den->tmp, vec->x1, vec->y1, den);
	}
}

void	save_list(t_doom *den, t_col *vec)
{
	save_list_help(den, vec);
	if (den->sec != den->secbak)
	{
		if (den->secbak == -1)
		{
			// den->xsec = (vec->x1 - den->startx) / (START + den->skape);
			// den->ysec = (vec->y1 - den->starty) / (START + den->skape);
			den->xsec = den->tmp->x;
			den->ysec = den->tmp->y;

		}
		den->secbak += 1;
		if (den->sec > 0)
			clean_vec(vec);
	}
}

void	save_map_help_two(t_doom *den, t_col *vec)
{
	if (vec->x1 > LIM && vec->x2 > LIM && vec->y1 > LIM && vec->y2 > LIM)
	{
		if ((vec->xmod - den->startx) / (START + den->skape) == den->xsec &&
			(vec->ymod - den->starty) / (START + den->skape) == den->ysec)
			den->sec += 1;
		ft_line(den, vec);
		den->walls += 1;
		if (den->sec != den->secbak && den->secbak >= 0)
		{
			den->blouk = 1;
			save_list(den, vec);
			den->button = 0;
		}
		else
			save_list(den, vec);
	}
}

void	save_map_help(t_doom *den, t_col *vec)
{
	if ((den->blouk == 1 && den->secbak > -1) || (den->xsec == LIM
	&& den->ysec == LIM))
	{
		if (den->button == 1)
		{
			den->xsec = (vec->x2 - den->startx) / (START + den->skape);
			den->ysec = (vec->y2 - den->starty) / (START + den->skape);
		}
		else if (den->button == 0)
		{
			den->xsec = (vec->x1 - den->startx) / (START + den->skape);
			den->ysec = (vec->y1 - den->starty) / (START + den->skape);
		}
		den->secbaktmp = den->sec;
		den->blouk = 0;
	}
	save_map_help_two(den, vec);
}

void	save_map(int x, t_doom *den, t_col *vec)
{
	if (den->button == 0 && x < den->border && (vec->xmod != vec->x2
	|| vec->ymod != vec->y2))
	{
		vec->x1 = vec->xmod;
		vec->y1 = vec->ymod;
		pixel_bigwidth(den, vec->x1, vec->y1, 0x00FF00);
	}
	else if (den->button == 1 && x < den->border && (vec->xmod != vec->x1
	|| vec->ymod != vec->y1))
	{
		vec->x2 = vec->xmod;
		vec->y2 = vec->ymod;
		pixel_bigwidth(den, vec->x2, vec->y2, 0x00FF00);
	}
	else if ((vec->xmod == vec->x1 && vec->ymod == vec->y1) || (vec->xmod ==
	vec->x2 && vec->ymod == vec->y2))
		den->button = den->button == 1 ? 0 : 1;
	save_map_help(den, vec);
}
