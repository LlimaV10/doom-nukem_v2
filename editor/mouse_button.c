/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_button.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/04 16:02:06 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/03/04 16:02:07 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void	mouse_button_down(t_doom *den, t_col *vec, int x, int y)
{
	if (den->sec > 0 && den->blouk == 1)
	{
		den->xy.x = (((float)x - den->startx) /
		(START + den->skape) + 0.5);
		den->xy.y = (((float)y - den->starty) /
		(START + den->skape) + 0.5);
		in_all_sect(den);
		if (den->xy.bool_cor != -1)
			den->incede = 1;
	}
	coordinate_network(x, y, den, vec);
	save_map(x, den, vec);
	clean_display(den);
	info_display(den);
}

void	ft_sdl_init_star(t_doom *den, t_col *vec)
{
	den->quit = 0;
	den->button = 0;
	den->window = SDL_CreateWindow("doom_nuken",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	den->bmp = SDL_GetWindowSurface(den->window);
	load_image(den);
	SDL_FillRect(den->bmp, NULL, 0x000000);
	map_redactor_mane(den, vec);
	map_network(den);
	info_display(den);
	clear_texture(den, vec);
	map_network(den);
	if (den->sec > 0)
		blonk_on_now(den);
	info_display(den);
}

void	key_down_but(t_doom *den, t_col *vec, const Uint8 *state)
{
	if ((state[SDL_SCANCODE_UP]) || (state[SDL_SCANCODE_DOWN]) ||
		(state[SDL_SCANCODE_LEFT]) || (state[SDL_SCANCODE_RIGHT]))
		move_scancode(den, vec, state);
	else if (state[SDL_SCANCODE_LSHIFT])
	{
		den->button_change = 3;
		movie_button(den, vec);
	}
	if (state[SDL_SCANCODE_TAB])
	{
		den->select = den->select == 1 ? 0 : 1;
		if (den->select == 0)
		{
			den->change = 0;
			retry_write(den, vec);
		}
	}
}

void	mousewheel(t_doom *den, t_col *vec, int y)
{
	if (y > 0)
		den->skape += 10;
	if (y < 0)
	{
		if (START + den->skape > 10)
			den->skape -= 10;
	}
	den->change = 0;
	clear_texture(den, vec);
	map_network(den);
	if (den->sec > 0)
		blonk_on_now(den);
	info_display(den);
}

void	mouse_button_down_select(t_doom *den, t_col *vec, SDL_Event e)
{
	if (den->change == 3)
	{
		retry_write(den, vec);
		den->change = 0;
	}
	if ((e.button.x < den->player.x * XSKAPE + 10 &&
		e.button.x > den->player.x * XSKAPE - 10) &&
		(e.button.y < den->player.y * YSKAPE + 10 &&
		e.button.y > den->player.y * YSKAPE - 10))
		change_way(den, 1, vec);
	else
		find_wals(e.button.x, e.button.y, den);
	find_sprite(den, e.button.x, e.button.y);
	clean_display(den);
	info_display(den);
}
