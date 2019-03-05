/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sdl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/04 11:40:51 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/03/04 11:40:53 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void	mouse_button_down_sdl(t_doom *den, t_col *vec, SDL_Event e)
{
	if (e.button.button == SDL_BUTTON_LEFT && den->select == 0)
	{
		if (den->blockwall == 1)
			mouse_button_rotate(den, vec, e.button.x, e.button.y);
		else if (den->blockwall == 2)
			mouse_button_sprite(den, vec, e.button.x, e.button.y);
		else if (e.button.x <= den->border)
			mouse_button_down(den, vec, e.button.x, e.button.y);
	}
	else if (e.button.button == SDL_BUTTON_LEFT && den->select == 1
		&& e.button.x <= den->border)
		blouk_write(den);
	if (e.button.button == SDL_BUTTON_RIGHT && den->select == 1)
		mouse_button_down_select(den, vec, e);
}

void	mouse_button_down_up_sdl(t_doom *den, t_col *vec, SDL_Event e)
{
	if (e.button.button == SDL_BUTTON_LEFT && e.button.x <= den->border
		&& den->blockwall == 0 && den->select == 0)
		den->button = den->button == 1 ? 0 : 1;
	if (e.button.x > den->border)
		pres_button(e.button.x, e.button.y, den, vec);
}

void	key_down_sdl(t_doom *den, t_col *vec, const Uint8 *state)
{
	if (state[SDL_SCANCODE_ESCAPE])
		den->quit = 1;
	else
		key_down_but(den, vec, state);
}

void	ft_sdl_init(t_doom *den)
{
	SDL_Event		e;
	const Uint8		*state;
	t_col			vec;

	ft_sdl_init_star(den, &vec);
	state = SDL_GetKeyboardState(NULL);
	while (!den->quit)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				den->quit = 1;
			if (e.type == SDL_KEYDOWN)
				key_down_sdl(den, &vec, state);
			else if (e.type == SDL_MOUSEBUTTONDOWN)
				mouse_button_down_sdl(den, &vec, e);
			else if (e.type == SDL_MOUSEBUTTONUP)
				mouse_button_down_up_sdl(den, &vec, e);
			else if (e.type == SDL_MOUSEWHEEL)
				mousewheel(den, &vec, e.wheel.y);
		}
	}
	SDL_DestroyWindow(den->window);
	free_surface(den);
	SDL_Quit();
}
