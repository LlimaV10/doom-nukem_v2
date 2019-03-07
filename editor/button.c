/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 15:40:41 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/03/01 15:40:42 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void	pres_button_two(int x, int y, t_doom *den)
{
	if (x >= 1840 && x < 1890 && y >= 680 && y < 730)
		den->button_change = 6;
	else if (x >= 1760 && x < 1810 && y >= 440 && y < 597)
		den->button_change = 8;
	else if (x >= 1815 && x < 1865 && y >= 440 && y < 597)
		den->button_change = 9;
	else if (x >= 1875 && x < 1925 && y >= 443 && y < 600)
		den->button_change = 10;
	else if (x >= 1940 && x < 1990 && y >= 443 && y < 600)
		den->button_change = 11;
}

void	pres_button(int x, int y, t_doom *den, t_col *vec)
{
	if (den->sec > 0 && den->blouk == 1)
	{
		if (x >= 1760 && x < 1810 && y >= 510 && y < 567)
			den->button_change = 1;
		else if (x >= 1820 && x < 1870 && y >= 510 && y < 567)
			den->button_change = 2;
		else if (x >= 1880 && x < 1930 && y >= 510 && y < 567)
			den->button_change = 3;
		else if (x >= 1940 && x < 1990 && y >= 510 && y < 567)
			den->button_change = 4;
		else if (x >= 1840 && x < 1890 && y >= 300 && y < 357)
			den->button_change = 5;
		else if (x >= 1840 && x < 1900 && y >= 680 && y < 737)
			den->button_change = 6;
		else if (x >= 1760 && x < 2010 && y >= 300 && y < 730)
			pres_button_two(x, y, den);
		else
			den->button_change = 0;
	}
	if (x >= 1770 && x < 1830 && y >= 300 && y < 357)
		den->button_change = 7;
	movie_button(den, vec);
}

int		check_picture_wall(t_doom *den, t_wals *tmp)
{
	while (tmp)
	{
		if (tmp == den->find_tmp)
		{
			if (tmp->p != NULL)
				return (1);
			else
				return (0);
		}
		tmp = tmp->next;
	}
	return (0);
}

int		check_picture(t_doom *den)
{
	t_wals		*tmp;

	tmp = den->tmp;
	if (den->change != 3)
	{
		if (check_picture_wall(den, tmp) == 1)
			return (1);
	}
	else
	{
		if (tmp->sec != den->find_tmp->sec)
		{
			while (tmp->next && tmp->next->sec != den->find_tmp->sec)
				tmp = tmp->next;
			tmp = tmp->next;
		}
		while (tmp && tmp->sec == den->find_tmp->sec)
		{
			if (tmp->p != NULL)
				return (1);
			tmp = tmp->next;
		}
	}
	return (0);
}

void	blouk_delete(t_doom *den)
{
	char		*cmp;
	SDL_Color	color_text;

	clean_string(den);
	color_text.r = 255;
	color_text.g = 0;
	color_text.b = 0;
	cmp = "please delete picture in wall\n";
	den->TTF_TextSolid = TTF_RenderText_Blended_Wrapped(den->font, cmp,
		color_text, 200);
	den->rect.x = 1770;
	den->rect.y = 380;
	den->rect.h = 50;
	den->rect.w = 30;
	SDL_BlitSurface(den->TTF_TextSolid, NULL, den->bmp, &den->rect);
	SDL_FreeSurface(den->TTF_TextSolid);
	SDL_UpdateWindowSurface(den->window);
}
