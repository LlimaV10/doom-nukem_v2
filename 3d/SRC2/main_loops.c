#include "../guardians.h"

void	menu_loop(t_sdl *iw)
{
	SDL_Event	e;
	int			quit;

	game_start_menu(iw);
	quit = 0;
	while (!quit)
	{
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
			else if (e.type == SDL_MOUSEMOTION)
			{
				mouse_move(e.motion.xrel, e.motion.yrel, iw);
				iw->v.mouse_x = e.motion.x;
				iw->v.mouse_y = e.motion.y;
			}
			else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT)
			{
				mouse_buttonleft_up(e.button.x, e.button.y, iw);
				iw->bag.click_x = e.button.x;
        		iw->bag.click_y = e.button.y;
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && iw->v.game_mode)
				iw->v.left_mouse_pressed = 1;
			else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_RIGHT)
				mouse_buttonright_up(e.button.x, e.button.y, iw);
			else if (e.type == SDL_MOUSEWHEEL && e.wheel.y != 0)
				mouse_wheel(&e, iw);
		loop(iw);
	}
}