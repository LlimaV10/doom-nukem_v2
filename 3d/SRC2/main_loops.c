/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loops.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 12:07:33 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 12:07:34 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	menu_loop(t_sdl *iw)
{
	SDL_Event	e;
	int			quit;

	game_start_menu(iw);
	quit = 0;
	while (!quit)
		while (SDL_PollEvent(&e) != 0)
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.scancode == 81)
				{
					iw->menu.count += ((iw->menu.count < 3) ? 1 : -2);
					game_start_menu(iw);
				}
				else if (e.key.keysym.scancode == 82)
				{
					iw->menu.count -= ((iw->menu.count > 1) ? 1 : -2);
					game_start_menu(iw);
				}
				else if (e.key.keysym.scancode == 40)
					quit = 1;
				else if (e.key.keysym.scancode == 41)
					exit_x(iw);
			}
}

void	image_loop(t_sdl *iw, t_packaging_texture *tex)
{
	SDL_Event	e;
	int			quit;
	SDL_Rect	rect;

	rect.w = WINDOW_W;
	rect.h = WINDOW_H;
	rect.x = 0;
	rect.y = 0;
	ft_scaled_blit2(tex, iw->sur, &rect);
	SDL_UpdateWindowSurface(iw->win);
	quit = 0;
	while (!quit)
		while (SDL_PollEvent(&e) != 0)
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.scancode == 41)
					exit_x(iw);
				else if (e.key.keysym.scancode == 40)
					quit = 1;
			}
}

void	main_loop2(t_sdl *iw, SDL_Event *e)
{
	if (e->type == SDL_MOUSEMOTION)
	{
		mouse_move(e->motion.xrel, e->motion.yrel, iw);
		iw->v.mouse_x = e->motion.x;
		iw->v.mouse_y = e->motion.y;
	}
	else if (e->type == SDL_MOUSEBUTTONUP &&
		e->button.button == SDL_BUTTON_LEFT)
	{
		mouse_buttonleft_up(e->button.x, e->button.y, iw);
		iw->bag.click_x = e->button.x;
		iw->bag.click_y = e->button.y;
	}
	else if (e->type == SDL_MOUSEBUTTONDOWN &&
		e->button.button == SDL_BUTTON_LEFT && iw->v.game_mode)
		iw->v.left_mouse_pressed = 1;
	else if (e->type == SDL_MOUSEBUTTONUP &&
		e->button.button == SDL_BUTTON_RIGHT)
		mouse_buttonright_up(iw);
	else if (e->type == SDL_MOUSEWHEEL && e->wheel.y != 0)
		mouse_wheel(e, iw);
}

void	main_loop(t_sdl *iw)
{
	SDL_Event e;

	iw->quit = 0;
	while (!iw->quit)
	{
		while (SDL_PollEvent(&e) != 0)
			if (e.type == SDL_QUIT)
				iw->quit = 1;
			else if (e.type == SDL_KEYDOWN)
			{
				key_down_repeat(e.key.keysym.scancode, iw);
				if (e.key.repeat == 0)
					key_down(e.key.keysym.scancode, iw);
			}
			else if (e.type == SDL_KEYUP)
				key_up(e.key.keysym.scancode, iw);
			else
				main_loop2(iw, &e);
		loop(iw);
	}
}

void	game_start_menu1(t_sdl *iw, SDL_Rect *player,
	SDL_Rect *zast, SDL_Rect *diff)
{
	player->x = 0;
	player->y = 0;
	player->w = WINDOW_W;
	player->h = WINDOW_H;
	zast->x = WINDOW_W / 10;
	zast->y = WINDOW_H / 8;
	zast->w = WINDOW_W / 4;
	zast->h = WINDOW_H / 8;
	diff->w = WINDOW_W / 6;
	diff->h = WINDOW_H / 10;
	diff->x = zast->x + (zast->w - diff->w) / 2;
	diff->y = zast->y + (zast->h - diff->h) / 2;
	ft_scaled_blit(iw->menu.icons[0], iw->sur, player);
}
