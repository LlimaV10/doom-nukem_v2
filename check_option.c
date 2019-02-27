/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_option.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 16:17:30 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/02/20 16:17:31 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

int		in_all_help(t_doom *den, t_wals *lmp, int sec)
{
	den->tmp = lmp;
	den->xy.x += 1;
	if (inside_sect(den, &den->tmp, sec) != 0)
	{
		den->tmp = lmp;
		den->xy.y -= 1;
		if (inside_sect(den, &den->tmp, sec) != 0)
		{
			den->xy.x -= 1;
			den->xy.bool_cor = sec;
			return (1);
		}
		else
			den->xy.x -= 1;
	}
	else
	{
		den->xy.x -= 1;
		den->xy.y -= 1;
	}
	return (0);
}

void	in_all_sect(t_doom *den)
{
	t_wals	*rmp;
	t_wals	*lmp;

	rmp = den->tmp;
	den->i = -1;
	while (++den->i < den->sec)
	{
		lmp = den->tmp;
		if (inside_sect(den, &den->tmp, den->i) != 0)
		{
			den->tmp = lmp;
			den->xy.y += 1;
			if (inside_sect(den, &den->tmp, den->i) != 0)
			{
				if (in_all_help(den, lmp, den->i) == 1)
					break ;
			}
			else
				den->xy.y -= 1;
		}
		den->xy.bool_cor = -1;
		if (den->tmp->next != NULL)
			den->tmp = den->tmp->next;
	}
	den->tmp = rmp;
}

void	blonk_on_now(t_doom *den)
{
	t_wals	*wals;

	wals = den->tmp;
	if (!den->tmp)
	{
		den->blouk = 1;
		return ;
	}
	while (wals->next)
		wals = wals->next;
	if (wals->blouk == 1)
		den->blouk = 1;
}

void	clean_string(t_doom *den)
{
	int i;
	int	j;

	j = 380;
	while (++j < 420)
	{
		i = den->border;
		while (++i < WIDTH)
			set_pixel(den->bmp, i, j, 0);
	}
}

void	back_startwall(t_doom *den, t_col *vec)
{
	clean_vec(vec);
	den->xsec = -1000000;
	den->ysec = -1000000;
	clear_texture(den, vec);
	map_network(den);
	info_display(den);
	den->button = 1;
	den->blouk = 1;
}
