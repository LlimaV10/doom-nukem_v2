/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_allsector.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 16:36:09 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/02/28 16:36:10 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void	drow_find(t_doom *den)
{
	if (den->change == 1)
	{
		den->find_tmp = den->find_tmp_one;
		ft_line(den, &den->rec_two);
		SDL_UpdateWindowSurface(den->window);
		den->color = 0x00FF00;
		ft_line(den, &den->rec);
		den->change = 2;
		SDL_UpdateWindowSurface(den->window);
	}
	else if (den->change == 2)
	{
		den->find_tmp = den->find_tmp_two;
		ft_line(den, &den->rec);
		SDL_UpdateWindowSurface(den->window);
		den->color = 0x00FF00;
		ft_line(den, &den->rec_two);
		den->change = 1;
		SDL_UpdateWindowSurface(den->window);
	}
	den->color = 0xFFFFFF;
	clean_display(den);
	info_display(den);
}

void	all_sector(t_doom *den, t_wals *tmp)
{
	if (den->find_tmp->sec != tmp->sec || (den->find_tmp->sec == tmp->sec &&
		den->find_tmp->inside != tmp->inside))
	{
		while (tmp)
		{
			if (den->find_tmp->sec == tmp->sec &&
				den->find_tmp->inside == tmp->inside)
				break ;
			tmp = tmp->next;
		}
	}
	while (tmp)
	{
		if (tmp->sec != den->find_tmp->sec || (tmp->sec == den->find_tmp->sec &&
			tmp->inside != den->find_tmp->inside))
			break ;
		den->color = 0x00FF00;
		den->rec.x1 = tmp->x * XSKAPE;
		den->rec.y1 = tmp->y * YSKAPE;
		den->rec.x2 = tmp->x1 * XSKAPE;
		den->rec.y2 = tmp->y1 * YSKAPE;
		ft_line(den, &den->rec);
		tmp = tmp->next;
	}
	SDL_UpdateWindowSurface(den->window);
}

void	blouk_write(t_doom *den)
{
	char		*cmp;
	SDL_Color	ct;

	den->rect.x = 1770;
	den->rect.y = 380;
	den->rect.h = 50;
	den->rect.w = 30;
	clean_string(den);
	ct.r = 255;
	ct.g = 0;
	ct.b = 0;
	cmp = "card edit mode lock, press \"TAB\"\n";
	den->TTF_TextSolid = TTF_RenderText_Blended_Wrapped(den->font, cmp,
		ct, 200);
	SDL_BlitSurface(den->TTF_TextSolid, NULL, den->bmp, &den->rect);
	SDL_FreeSurface(den->TTF_TextSolid);
	SDL_UpdateWindowSurface(den->window);
}

void	clean_vec(t_col *vec)
{
	vec->x1 = LIM;
	vec->y1 = LIM;
	vec->x2 = LIM;
	vec->y2 = LIM;
}
