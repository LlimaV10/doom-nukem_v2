/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/05 16:16:05 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/02/05 16:16:05 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void	write_map_help(t_doom *den, t_col *vec)
{
	vec->x1 = (den->tmp->x * XSKAPE);
	vec->y1 = (den->tmp->y * YSKAPE);
	vec->x2 = (den->tmp->x1 * XSKAPE);
	vec->y2 = (den->tmp->y1 * YSKAPE);
	if (vec->x1 < den->border && vec->x1 >= 0)
		pixel_bigwidth(den, vec->x1, vec->y1, 0x00FF00);
	if (vec->x2 < den->border && vec->x2 >= 0)
		pixel_bigwidth(den, vec->x2, vec->y2, 0x00FF00);
	if (den->tmp->next_sector && den->tmp->nextsector_wall)
		den->color = 0xFF0000;
	if (den->tmp->sec == den->finish)
		den->color = 0xFFFF00;
	ft_line(den, vec);
	if (den->tmp->blouk == 1)
	{
		vec->x1 = -1000000;
		vec->y1 = -1000000;
		vec->x2 = -1000000;
		vec->y2 = -1000000;
	}
	den->color = 0xFFFFFF;
}

void	write_map(t_doom *den, t_col *vec)
{
	t_wals	*rmp;

	rmp = den->tmp;
	if (den->tmp)
	{
		while (den->tmp)
		{
			if (den->retskape != den->skape)
				findstart(den);
			write_map_help(den, vec);
			den->tmp = den->tmp->next;
		}
		den->button = 0;
		if (den->player.x != -10000 && den->player.y != -10000)
			rotate_image(den);
		if (den->sprite)
			see_sprite(den);
	//	SDL_UpdateWindowSurface(den->window);
	}
	den->tmp = rmp;
}

void	clear_texture(t_doom *den, t_col *vec)
{
	SDL_FillRect(den->bmp, NULL, 0x000000);
	vec->x1 = LIM;
	vec->y1 = LIM;
	vec->x2 = LIM;
	vec->y2 = LIM;
	vec->xmod = 0;
	vec->ymod = 0;
	write_map(den, vec);
}

void	write_list(t_doom *den, t_wals *tmp)
{
	t_wals *rmp;
	t_wals *nmp;

	SDL_FillRect(den->bmp, NULL, 0x000000);
	rmp = tmp;
	if (den->retskape != den->skape)
		findstart(den);
	if (tmp)
	{
		if (tmp != den->find_tmp)
		{
			while (tmp->next && tmp->next != den->find_tmp)
				tmp = tmp->next;
		}
		rmp = tmp->next;
		if (tmp->next->next && tmp->next->next->sec == tmp->next->sec)
		{
			tmp->next->next->x = tmp->next->x;
			tmp->next->next->y = tmp->next->y;
			tmp->next->next->sec = tmp->next->sec;
			tmp->next = tmp->next->next;
			delete_pointer(den, rmp);
			if (rmp->next_sector != 0)
			{
				nmp = rmp->nextsector_wall;
				nmp->next_sector = 0;
				nmp->nextsector_wall = 0;
				rmp->next_sector = 0;
				rmp->nextsector_wall = 0;
			}
			free(rmp);
			den->walls -= 1;
		}
	}
}

void	move_sector(t_doom *den, int y, int x)
{
	t_wals	*rmp;

	rmp = den->tmp;
	while (den->tmp)
	{
		if (den->tmp->sec == den->find_tmp->sec)
		{
			den->tmp->x += x;
			den->tmp->x1 += x;
			den->tmp->y += y;
			den->tmp->y1 += y;
		}
		else if (den->tmp->sec && den->tmp->sec > den->find_tmp->sec)
			break ;
		den->tmp = den->tmp->next;
	}
	den->tmp = rmp;
}
