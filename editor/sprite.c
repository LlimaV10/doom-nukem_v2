/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/19 18:59:04 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/02/19 18:59:05 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void	delete_sprite(t_doom *den, t_sprite *s)
{
	t_sprite	head;
	t_sprite	*tmp;
	t_sprite	*tmp2;

	head.next = den->sprite;
	tmp = &head;
	while (tmp->next)
	{
		if (tmp->next == s)
		{
			tmp2 = tmp->next;
			tmp->next = tmp->next->next;
			free(tmp2);
			break ;
		}
		tmp = tmp->next;
	}
	den->sprite = head.next;
}

void	check_finish_sprite(t_doom *den)
{
	t_sprite	*tmp;
	t_sprite	*tmp_next;

	tmp = den->sprite;
	if (den->find_tmp->sec < den->finish)
		den->finish -= 1;
	if (den->find_tmp->sec == den->finish)
		den->finish = -1;
	while (tmp)
	{
		tmp_next = tmp->next;
		if (den->find_tmp->sec == tmp->num_sec)
			delete_sprite(den, tmp);
		tmp = tmp_next;
	}
	tmp = den->sprite;
	while (tmp)
	{
		if (den->find_tmp->sec < tmp->num_sec)
			tmp->num_sec -= 1;
		tmp = tmp->next;
	}
}

void	see_sprite(t_doom *den)
{
	t_sprite	*tmp;

	tmp = den->sprite;
	while (tmp)
	{
		if (tmp->x * XSKAPE < den->border && tmp->x * XSKAPE >= 0)
		{
			den->rect.x = (tmp->x * XSKAPE - (den->icon[10]->w / 2));
			den->rect.y = (tmp->y * YSKAPE - (den->icon[10]->h / 2));
			den->rect.h = 20;
			den->rect.w = 20;
		}
		SDL_BlitSurface(den->icon[10], NULL, den->bmp, &den->rect);
		tmp = tmp->next;
	}
}

void	find_sprite(t_doom *den, int x, int y)
{
	t_sprite	*tmp;

	tmp = den->sprite;
	while (tmp)
	{
		if ((x < tmp->x * XSKAPE + 10 && x > tmp->x * XSKAPE - 10) &&
			(y < tmp->y * YSKAPE + 10 && y > tmp->y * YSKAPE - 10))
		{
			den->rect.x = tmp->x * XSKAPE - (den->icon[12]->w / 2);
			den->rect.y = tmp->y * YSKAPE - (den->icon[12]->h / 2);
			den->rect.h = 20;
			den->rect.w = 20;
			SDL_BlitSurface(den->icon[12], NULL, den->bmp, &den->rect);
			den->find_sprite = tmp;
			den->change = 4;
			break ;
		}
		tmp = tmp->next;
	}
}

void	add_sprite(t_doom *den)
{
	t_sprite	*tmp;

	tmp = (t_sprite *)malloc(sizeof(t_sprite));
	tmp->x = den->xy.x;
	tmp->y = den->xy.y;
	tmp->num_sec = den->xy.bool_cor;
	tmp->t_numb = 0;
	tmp->type = 0;
	tmp->scale = 1.0f;
	tmp->e.vis_esp.curr_sector = -1;
	tmp->e.previous_picture_change = 1;
	tmp->e.prev_update_time = 1;
	tmp->fall_time = 1;
	tmp->taken = 0;
	tmp->next = den->sprite;
	den->sprite = tmp;
}
