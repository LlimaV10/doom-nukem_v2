/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_two.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 16:26:54 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/02/28 16:26:55 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void	check_sprite(t_doom *den)
{
	t_sprite	*tmp;
	t_sprite	*tmp2;

	tmp = den->sprite;
	while (tmp)
	{
		den->xy.x = tmp->x;
		den->xy.y = tmp->y;
		in_all_sect(den);
		tmp->num_sec = den->xy.bool_cor;
		tmp2 = tmp;
		tmp = tmp->next;
		if (den->xy.bool_cor == -1)
			delete_sprite(den, tmp2);
	}
}

void	sum_sprite(t_doom *den)
{
	t_sprite	*tmp;

	tmp = den->sprite;
	den->sprites = 0;
	if (den->sprite)
	{
		while (tmp)
		{
			den->sprites += 1;
			tmp = tmp->next;
		}
	}
}

void	sprite(t_doom *den)
{
	den->cursor = SDL_CreateColorCursor(den->icon[10], 0, 0);
	SDL_SetCursor(den->cursor);
	den->blockwall = 2;
}

void	retry_write(t_doom *den, t_col *vec)
{
	clean_find_vec(den);
	clear_texture(den, vec);
	map_network(den);
	info_display(den);
	blonk_on_now(den);
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
