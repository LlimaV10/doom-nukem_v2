#include "../guardians.h"

void	draw_input_on_screen(t_sdl *iw, t_keyb_inp *ki)
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
	//draw_text_blue(iw, "qwertyuiopasdfghjklzxcvbnmqwerty", WINDOW_W / 2 - INPUT_LINE_LEN + 12,
	//	WINDOW_H / 2 - 14);
	//printf("strlen %d\n", ft_strlen("qwertyuiopasdfghjklzxcvbnmqwerty"));
	SDL_UpdateWindowSurface(iw->win);
}

t_keyb_inp  *input_loop(t_sdl *iw)
{
	int  quit;
	SDL_Event   e;
	t_keyb_inp  *ki;
	char		*alphabet;
	int		shift;

	shift = 0;
	alphabet = ft_strdup("abcdefghijklmnopqrstuvwxyz");
	ki = (t_keyb_inp *)malloc(sizeof(t_keyb_inp));
	ki->s_len = 0;
	draw_input_on_screen(iw, ki);
	quit = 0;
	while (!quit)
		while (SDL_PollEvent(&e) != 0)
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.scancode == 41)
					exit_x(iw);
				else if (e.key.keysym.scancode >= 4 && e.key.keysym.scancode <= 29
					&& ki->s_len < INPUT_STRING_LEN)
				{
					if (shift == 0)
						ki->s[ki->s_len++] = alphabet[e.key.keysym.scancode - 4];
					else
						ki->s[ki->s_len++] = ft_toupper(alphabet[e.key.keysym.scancode - 4]);
					draw_input_on_screen(iw, ki);
				}
				else if (e.key.keysym.scancode == 44)
				{
					ki->s[ki->s_len++] = ' ';
					draw_input_on_screen(iw, ki);
				}
				else if (e.key.keysym.scancode == 42)
				{
					ki->s_len -= ((ki->s_len > 0) ? 1 : 0);
					draw_input_on_screen(iw, ki);
				}
				else if (e.key.keysym.scancode == 225)
					shift = 1;
				else if (e.key.keysym.scancode == 40)
					quit = 1;
			}
			else if (e.type == SDL_KEYUP && e.key.keysym.scancode == 225)
				shift = 0;
	free (alphabet);
	return (ki);
}

void	add_checkpoint(t_sdl *iw, t_sprite *s)
{
	t_keyb_inp 	*ki;
	//int			i;
	//t_sprite	*tmp;

	ki = input_loop(iw);
	ki->sprite = s;
	//i = 0;
	// tmp = *iw->sprite;
	// while (tmp)
	// {
	// 	if (tmp == s)
	// 		break;
	// 	i++;
	// 	tmp = tmp->next;
	// }
	// ki->sprite_numb = i;
	ki->next = iw->checkpoints;
	iw->checkpoints = ki;
}

void	draw_checkpoint_text(t_sdl *iw)
{
	SDL_Rect	rect;
	SDL_Surface	*stext;
	SDL_Color	col;

	if (!iw->v.last_to_write)
		return;
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