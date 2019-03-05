/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_scan.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/04 16:01:43 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/03/04 16:01:44 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void	move_scan_sector(t_doom *den, const Uint8 *state)
{
	if (state[SDL_SCANCODE_UP])
		den->find_sprite->y -= 1;
	else if (state[SDL_SCANCODE_DOWN])
		den->find_sprite->y += 1;
	else if (state[SDL_SCANCODE_LEFT])
		den->find_sprite->x -= 1;
	else if (state[SDL_SCANCODE_RIGHT])
		den->find_sprite->x += 1;
}

void	move_scan_sprite(t_doom *den, const Uint8 *state)
{
	if (state[SDL_SCANCODE_UP])
		den->starty -= (den->skape + START);
	else if (state[SDL_SCANCODE_DOWN])
		den->starty += (den->skape + START);
	else if (state[SDL_SCANCODE_LEFT])
		den->startx -= (den->skape + START);
	else if (state[SDL_SCANCODE_RIGHT])
		den->startx += (den->skape + START);
	clean_find_vec(den);
}

void	move_scancode(t_doom *den, t_col *vec, const Uint8 *state)
{
	if (den->change == 4)
		move_scan_sector(den, state);
	else if (den->change == 3 && den->select == 1)
	{
		if (state[SDL_SCANCODE_UP])
			move_sector(den, -1, 0);
		else if (state[SDL_SCANCODE_DOWN])
			move_sector(den, 1, 0);
		else if (state[SDL_SCANCODE_LEFT])
			move_sector(den, 0, -1);
		else if (state[SDL_SCANCODE_RIGHT])
			move_sector(den, 0, 1);
	}
	else
		move_scan_sprite(den, state);
	clear_texture(den, vec);
	map_network(den);
	if (den->sec > 0)
		blonk_on_now(den);
	info_display(den);
}

void	mouse_button_rotate(t_doom *den, t_col *vec, int x, int y)
{
	den->xy.x = (x - den->startx) / SKAPE;
	den->xy.y = (y - den->starty) / SKAPE;
	in_all_sect(den);
	if (den->xy.bool_cor != -1)
	{
		den->player.x = den->xy.x;
		den->player.y = den->xy.y;
		rotate_image(den);
		den->xy.bool_cor = -1;
	}
	retry_write(den, vec);
	den->blockwall = 0;
	den->button = den->button == 1 ? 0 : 1;
	SDL_FreeCursor(den->cursor);
}

void	mouse_button_sprite(t_doom *den, t_col *vec, int x, int y)
{
	den->xy.x = (x - den->startx) / SKAPE;
	den->xy.y = (y - den->starty) / SKAPE;
	in_all_sect(den);
	if (den->xy.bool_cor != -1)
	{
		add_sprite(den);
		den->xy.bool_cor = -1;
	}
	retry_write(den, vec);
	den->blockwall = 0;
	den->button = den->button == 1 ? 0 : 1;
	SDL_FreeCursor(den->cursor);
}
