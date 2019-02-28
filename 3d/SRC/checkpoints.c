/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkpoints.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 15:37:25 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/02/28 15:41:39 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void		draw_input_on_screen(t_sdl *iw, t_keyb_inp *ki)
{
	int		i;
	int		to_i;
	int		start_j;
	int		to_j;
	int		j;

	set_pixel2(iw->sur, 0, 0, 0xAAAAAA);
	i = WINDOW_W / 2 - INPUT_LINE_LEN - 1;
	to_i = WINDOW_W / 2 + INPUT_LINE_LEN;
	start_j = WINDOW_H / 2 - 16 - 1;
	to_j = WINDOW_H / 2 + 16;
	while (++i < to_i)
	{
		j = start_j;
		while (++j < to_j)
			set_pixel2(iw->sur, i, j, 0xCCCCCC);
	}
	ki->s[ki->s_len] = '\0';
	draw_text_blue(iw, ki->s, WINDOW_W / 2 - INPUT_LINE_LEN + 12,
		WINDOW_H / 2 - 14);
	SDL_UpdateWindowSurface(iw->win);
}

void		input_loop_keydown(t_sdl *iw, t_input_loop *d, SDL_Event *e)
{
	if (e->key.keysym.scancode == 41)
		exit_x(iw);
	else if (e->key.keysym.scancode >= 4
		&& e->key.keysym.scancode <= 29
		&& d->ki->s_len < INPUT_STRING_LEN)
	{
		if (d->shift == 0)
			d->ki->s[d->ki->s_len++] =
				d->alphabet[e->key.keysym.scancode - 4];
		else
			d->ki->s[d->ki->s_len++] = ft_toupper(
					d->alphabet[e->key.keysym.scancode - 4]);
		draw_input_on_screen(iw, d->ki);
	}
	else if (e->key.keysym.scancode == 44)
	{
		d->ki->s[d->ki->s_len++] = ' ';
		draw_input_on_screen(iw, d->ki);
	}
	else if (e->key.keysym.scancode == 42)
	{
		d->ki->s_len -= ((d->ki->s_len > 0) ? 1 : 0);
		draw_input_on_screen(iw, d->ki);
	}
}

t_keyb_inp	*input_loop(t_sdl *iw)
{
	t_input_loop	d;
	SDL_Event		e;

	d.shift = 0;
	d.alphabet = ft_strdup("abcdefghijklmnopqrstuvwxyz");
	d.ki = (t_keyb_inp *)malloc(sizeof(t_keyb_inp));
	d.ki->s_len = 0;
	draw_input_on_screen(iw, d.ki);
	d.quit = 0;
	while (!d.quit)
		while (SDL_PollEvent(&e) != 0)
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.scancode == 225)
					d.shift = 1;
				else if (e.key.keysym.scancode == 40)
					d.quit = 1;
				else
					input_loop_keydown(iw, &d, &e);
			}
			else if (e.type == SDL_KEYUP && e.key.keysym.scancode == 225)
				d.shift = 0;
	free(d.alphabet);
	return (d.ki);
}

void		add_checkpoint(t_sdl *iw, t_sprite *s)
{
	t_keyb_inp	*ki;

	ki = input_loop(iw);
	ki->sprite = s;
	ki->next = iw->checkpoints;
	iw->checkpoints = ki;
}

void		draw_checkpoint_text(t_sdl *iw)
{
	SDL_Rect	rect;
	SDL_Surface	*stext;
	SDL_Color	col;

	if (!iw->v.last_to_write)
		return ;
	rect.x = WINDOW_W - 400;
	rect.y = 230;
	col.a = 0;
	col.r = 0;
	col.g = 255;
	col.b = 255;
	rect.w = 0;
	rect.h = 0;
	iw->v.last_to_write->s[iw->v.last_to_write->s_len] = '\0';
	stext = TTF_RenderText_Solid(iw->arial_font, iw->v.last_to_write->s, col);
	SDL_BlitSurface(stext, NULL, iw->sur, &rect);
	SDL_FreeSurface(stext);
}
