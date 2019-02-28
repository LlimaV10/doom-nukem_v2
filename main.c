/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/26 15:54:12 by ddehtyar          #+#    #+#             */
/*   Updated: 2018/08/26 15:54:14 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# define _CRT_SECURE_NO_WARNINGS

#include "doom_nuken.h"
#include <stdio.h>

void	change_wall(t_doom *den, SDL_Color color_text) // 
{
	char	*infor;

	infor = ft_itoa(den->find_tmp->x);
	den->TTF_TextSolid = TTF_RenderText_Blended_Wrapped(den->font, infor, color_text, 200);
	SDL_Rect xwall = {1880, 150, 50, 30};
	SDL_BlitSurface(den->TTF_TextSolid, NULL, den->bmp, &xwall);
	SDL_FreeSurface(den->TTF_TextSolid);
	free(infor);

	infor = ft_itoa(den->find_tmp->y);
	den->TTF_TextSolid = TTF_RenderText_Blended_Wrapped(den->font, infor, color_text, 200);
	SDL_Rect ywall = {1880, 177, 50, 30};
	SDL_BlitSurface(den->TTF_TextSolid, NULL, den->bmp, &ywall);
	SDL_FreeSurface(den->TTF_TextSolid);
	free(infor);

	infor = ft_itoa(den->find_tmp->x1);
	den->TTF_TextSolid = TTF_RenderText_Blended_Wrapped(den->font, infor, color_text, 200);
	SDL_Rect xwallf = {1880, 204, 50, 30};
	SDL_BlitSurface(den->TTF_TextSolid, NULL, den->bmp, &xwallf);
	SDL_FreeSurface(den->TTF_TextSolid);
	free(infor);
	
	infor = ft_itoa(den->find_tmp->y1);
	den->TTF_TextSolid = TTF_RenderText_Blended_Wrapped(den->font, infor, color_text, 200);
	SDL_Rect ywallf = {1880, 231, 50, 30};
	SDL_BlitSurface(den->TTF_TextSolid, NULL, den->bmp, &ywallf);
	SDL_FreeSurface(den->TTF_TextSolid);
	free(infor);

	infor = ft_itoa(den->find_tmp->sec);
	den->TTF_TextSolid = TTF_RenderText_Blended_Wrapped(den->font, infor, color_text, 200);
	SDL_Rect sect = {1880, 258, 50, 30};
	SDL_BlitSurface(den->TTF_TextSolid, NULL, den->bmp, &sect);
	SDL_FreeSurface(den->TTF_TextSolid);
	free(infor);
}

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
	 	if (den->xy.bool_cor == -1) // сделать проверку что бы игрок находися в каком то секторе ! 
	 	{
			
	 		delete_sprite(den, tmp2);
	 		printf(" FACK        !\n");
	 	}

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

void	info_display(t_doom *den)
{
	char	*cmp; 
	char	*infor;

//	den->font = TTF_OpenFont("frameworks/ARIAL.TTF", 24);
	SDL_Color color_text;
	color_text.r = 255;
	color_text.g = 255;
	color_text.b = 255;

	cmp = "sectors:\nwalls:\nsprites:\nclosed sector:\ndedicated wall\nstart_x:\n"
	"star_y:\nfifnish_x:\nfinish_y:\nsector: ";
	den->TTF_TextSolid = TTF_RenderText_Blended_Wrapped(den->font, cmp, color_text, 200);
	SDL_Rect info = {1770, 10, 50, 30};
	SDL_BlitSurface(den->TTF_TextSolid, NULL, den->bmp, &info);
	SDL_FreeSurface(den->TTF_TextSolid);

	infor = ft_itoa(den->sec);
	den->TTF_TextSolid = TTF_RenderText_Blended_Wrapped(den->font, infor, color_text, 200);
	SDL_Rect sector = {1860, 10, 50, 30};
	SDL_BlitSurface(den->TTF_TextSolid, NULL, den->bmp, &sector);
	SDL_FreeSurface(den->TTF_TextSolid);
	free(infor);

	infor = ft_itoa(den->walls + 1);
	den->TTF_TextSolid = TTF_RenderText_Blended_Wrapped(den->font, infor, color_text, 200);
	SDL_Rect walls = {1860, 37, 50, 30};
	SDL_BlitSurface(den->TTF_TextSolid, NULL, den->bmp, &walls);
	SDL_FreeSurface(den->TTF_TextSolid);
	free(infor);
	
	sum_sprite(den);
	infor = ft_itoa(den->sprites);
	den->TTF_TextSolid = TTF_RenderText_Blended_Wrapped(den->font, infor, color_text, 200);
	SDL_Rect poral = {1860, 67, 50, 30};
	SDL_BlitSurface(den->TTF_TextSolid, NULL, den->bmp, &poral);
	SDL_FreeSurface(den->TTF_TextSolid);

	den->TTF_TextSolid = TTF_RenderText_Blended_Wrapped(den->font, den->blouk == 1 ? "YES" : "NO", color_text, 200);
	SDL_Rect closed = {1920, 94, 50, 30};
	SDL_BlitSurface(den->TTF_TextSolid, NULL, den->bmp, &closed);
	SDL_FreeSurface(den->TTF_TextSolid);
	
	if (den->change == 1 || den->change == 2)
		change_wall(den, color_text);

	free(infor);
//	TTF_CloseFont(den->font);
	board_display(den);
	SDL_UpdateWindowSurface(den->window);
}

void	drow_find(t_doom *den)
{
	printf (" den->change = %d\n", den->change);		
	if (den->change == 1)
	{
		den->find_tmp = den->find_tmp_one;
		ft_line(den, &den->rec_two);
		SDL_UpdateWindowSurface(den->window);
		den->color = 0x00FF00;
		ft_line(den, &den->rec);
		den->change = 2;
		SDL_UpdateWindowSurface(den->window);
	}
	else if (den->change == 2)
	{
		den->find_tmp = den->find_tmp_two;
		ft_line(den, &den->rec);
		SDL_UpdateWindowSurface(den->window);
		den->color = 0x00FF00;
		ft_line(den, &den->rec_two);
		den->change = 1;
		SDL_UpdateWindowSurface(den->window);
	}
	den->color = 0xFFFFFF;
	clean_display(den);
	info_display(den);
}

void	retry_write(t_doom *den, t_col *vec)
{
	clean_find_vec(den);
	clear_texture(den, vec);
	map_network(den);
	info_display(den);
	blonk_on_now(den);
}

void	all_sector(t_doom *den, t_wals *tmp)
{
	printf("ALL SECTOR\n");

	if (den->find_tmp->sec != tmp->sec || (den->find_tmp->sec == tmp->sec && den->find_tmp->inside != tmp->inside))
	{
		while(tmp)
		{
			if (den->find_tmp->sec == tmp->sec && den->find_tmp->inside == tmp->inside)
				break ;
			tmp = tmp->next;
		}
	}
	printf("ALL IS GOOD \n");
	while(tmp)
	{
		if (tmp->sec != den->find_tmp->sec || (tmp->sec == den->find_tmp->sec && tmp->inside != den->find_tmp->inside))
			break ;
		den->color = 0x00FF00;
		den->rec.x1 = tmp->x * XSKAPE;
		den->rec.y1 = tmp->y * YSKAPE;
		den->rec.x2 = tmp->x1 * XSKAPE;
		den->rec.y2 = tmp->y1 * YSKAPE;
		ft_line(den, &den->rec);
		tmp = tmp->next;
	}
	den->color = 0xFFFFFF;
	SDL_UpdateWindowSurface(den->window);
	den->change = 3;	
}

void	movie_button(t_doom *den, t_col *vec)
{
	if (den->button_change == 1 && (den->change == 1 || den->change == 2 || den->change == 3 || den->change ==4))
	{
		printf("delete \n");
		delete_wall(den, vec);
		den->button_change = 0;
	}
	else if (den->button_change == 2 && (den->change == 1 || den->change == 2))
	{
		printf("next_wall \n");
		next_wall(den);
		
		den->button_change = 0;
		clean_display(den);
		info_display(den);
		SDL_UpdateWindowSurface(den->window);
	}
	else if (den->button_change == 3 && ((den->change == 1 || den->change == 2) || den->change == 3))
	{
		printf("portal \n");
		if (den->find_tmp_one != 0 && den->find_tmp_two != 0 && den->find_tmp->inside == 0 && den->find_tmp_one->inside == 0 && den->find_tmp_two->inside == 0)
		{
			portal_sector(den);
			clear_find(den);
			den->change = 0;
			den->button_change = 0;
		}
		else if (den->change == 3 && den->find_tmp->inside != 0)
		{
			make_portal_sector(den, vec);
			den->change = 0;
			den->button_change = 0;
		}
	}
	else if (den->button_change == 4 && den->blouk == 1)
	{
		if (den->sprite)
			check_sprite(den);
		den->xy.x = den->player.x;
		den->xy.y = den->player.y;
		in_all_sect(den);
		if (den->xy.bool_cor == -1) // сделать проверку что бы игрок находися в каком то секторе ! 
		{
			den->player.x = -10000;
			den->player.y = -10000;
		}
		if (den->player.x == -10000 && den->player.y == -10000)
			blouk_player(den);
		else
			main3d_edit(den);//, den->tmp);
		den->button_change = 0;
	}
	else if (den->button_change == 5 && (den->change == 1 || den->change == 2))
	{
		printf("wall_next_sect \n");
		printf("den->change = %d\n", den->change);
		printf("rec.x1 = %d, rec_two.x1 =%d\n", den->rec.x1, den->rec_two.x1);
		if (den->find_tmp_one && den->find_tmp_two)
		{
			printf ( "    OKEY \n");
		 	drow_find(den);
		}

		den->button_change = 0;
	}
	else if (den->button_change == 6 && den->blouk == 1)
	{
		printf("SAVE \n");
		save_map_file(den);
		den->button_change = 0;
	}
	else if (den->button_change == 7)
	{
		if (den->blouk != 1 && den->walls > -1 && ((vec->x1 != -1000000 && vec->y1 != -1000000) && 
		(vec->x2 != -1000000 && vec->y2 != -1000000)))
			back_wall(den, vec);

		else if (vec->x1 == -1000000 && vec->y1 == -1000000 && vec->x2 != -1000000 && vec->y2 != -1000000)
			back_startwall(den, vec);
		den->button_change = 0;
	}
	else if (den->button_change == 8)
	{
		player(den);
		printf("PLAYER \n");
		den->button_change = 0;
		den->change = 0;
	}
	else if (den->button_change == 9 && (den->change == 1 || den->change == 2))
	{
		printf("all_sector \n");
		all_sector(den, den->tmp);
		den->button_change = 0;
	}
	else if (den->button_change == 10)
	{
		printf("sprite \n");
		sprite(den);

		den->change = 0;	
		den->button_change = 0;
	}
	else if (den->button_change == 11 && den->change == 3)
	{
		printf("finish \n");
		den->finish = den->find_tmp->sec;
		den->change = 0;
		den->button_change = 0;
		retry_write(den, vec);
	}
}

void	pres_button(int x, int y, t_doom *den, t_col *vec)
{
	if (den->sec > 0)
	{
		if (x >= 1760 && x < 1807 && y >= 500 && y < 567)
			den->button_change = 1; // delete
		else if (x >= 1810 && x < 1857 && y >= 500 && y < 567)
			den->button_change = 2; // next_wall
		else if (x >= 1875 && x < 1927 && y >= 500 && y < 567)
			den->button_change = 3; // all_wall
		else if (x >= 1935 && x < 1982 && y >= 500 && y < 567)
			den->button_change = 4; // portall
		else if (x >= 1840 && x < 1940 && y >= 280 && y < 365)
			den->button_change = 5; // wall_next_sector
		else if (x >= 1840 && x < 1900 && y >= 680 && y < 730)
			den->button_change = 6; // wall_next_sector
		else if (x >= 1760 && x < 1815 && y >= 450 && y < 500)
			den->button_change = 8; // player
		else if (x >= 1815 && x < 1870 && y >= 455 && y < 500)
			den->button_change = 9; // all_sector
		else if (x >= 1875 && x < 1935 && y >= 455 && y < 510)
			den->button_change = 10; // sprite
		else if (x >= 1940 && x < 2000 && y >= 455 && y < 510)
			den->button_change = 11; // finish
		else
			den->button_change = 0;
	}
	if (x >= 1770 && x < 1830 && y >= 280 && y < 365)
		den->button_change = 7; // back
	movie_button(den, vec);

}

void	blouk_write(t_doom *den)
{
	char	*cmp;

	clean_string(den);
//	den->font = TTF_OpenFont("frameworks/ARIAL.TTF", 20);
	SDL_Color color_text;
	color_text.r = 255;
	color_text.g = 0;
	color_text.b = 0;

	cmp = "card edit mode lock, press \"TAB\"\n";
	den->TTF_TextSolid = TTF_RenderText_Blended_Wrapped(den->font, cmp, color_text, 200);
	SDL_Rect wrong = {1770, 380, 50, 30};
	SDL_BlitSurface(den->TTF_TextSolid, NULL, den->bmp, &wrong);
	SDL_FreeSurface(den->TTF_TextSolid);

	//TTF_CloseFont(den->font);
	SDL_UpdateWindowSurface(den->window);
}

void	sprite(t_doom *den)
{
	den->cursor = SDL_CreateColorCursor(den->icon[10], 0, 0);
	SDL_SetCursor(den->cursor);
	den->blockwall = 2;
}


void	ft_sdl_init (t_doom *den)
{
	SDL_Event 		e;
	const Uint8		*state;
	int				quit;
	t_col			vec;

	quit = 0;
	den->button = 0;
	den->window = SDL_CreateWindow("doom_nuken",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	den->bmp = SDL_GetWindowSurface(den->window);
	load_image(den);
	SDL_FillRect(den->bmp, NULL, 0x000000);
	map_redactor_mane(den, &vec);
	map_network(den);
	info_display(den);
	state = SDL_GetKeyboardState(NULL);

	clear_texture(den, &vec);
	map_network(den);
	info_display(den);
	while (!quit)
	{	
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				quit = 1;
			if (e.type == SDL_KEYDOWN)
			{
				if (state[SDL_SCANCODE_ESCAPE])
					quit = 1;
				else if (state[SDL_SCANCODE_SPACE]) // вывожу координаты
					print_list(den->tmp, den);
				else if ((state[SDL_SCANCODE_UP]) || (state[SDL_SCANCODE_DOWN]) || (state[SDL_SCANCODE_LEFT]) || (state[SDL_SCANCODE_RIGHT]))
				{
					if (den->change == 4)
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
					{ 
						if (state[SDL_SCANCODE_UP])
							den->starty -= (den->skape + START);
						else if (state[SDL_SCANCODE_DOWN])
							den->starty += (den->skape + START);
						else if (state[SDL_SCANCODE_LEFT])
							den->startx -= (den->skape + START);
						else if (state[SDL_SCANCODE_RIGHT]) // стрелочки
							den->startx += (den->skape + START);
						clean_find_vec(den);
					}

					clear_texture(den, &vec);
					map_network(den);
					info_display(den);
					if (den->sec > 0)
						blonk_on_now(den);

				}
				else if (state[SDL_SCANCODE_LSHIFT])
				{
					//////////////////////
					den->button_change = 3;
					movie_button(den, &vec);
				}
				if (state[SDL_SCANCODE_TAB]) // блокировка рисовать
				{
					den->select = den->select == 1 ? 0 : 1;
					if (den->select == 0)
					{
						den->change = 0;
						retry_write(den, &vec);
					}
				}
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (e.button.button == SDL_BUTTON_LEFT && den->select == 0) 
				{
					if (den->blockwall == 1) // ставить игрока
					{
						den->xy.x = (e.button.x - den->startx) / (START + den->skape);
						den->xy.y = (e.button.y - den->starty) / (START + den->skape);

						in_all_sect(den);
						if (den->xy.bool_cor != -1)
						{
							den->player.x = den->xy.x;
							den->player.y = den->xy.y;
							rotate_image(den);
							den->xy.bool_cor = -1;
						}
						retry_write(den, &vec);
						
						den->blockwall = 0;
						den->button = den->button == 1 ? 0 : 1;
						SDL_FreeCursor(den->cursor);
					}
					else if (den->blockwall == 2)
					{
						den->xy.x = (e.button.x - den->startx) / (START + den->skape);
						den->xy.y = (e.button.y - den->starty) / (START + den->skape);

						in_all_sect(den);
						if (den->xy.bool_cor != -1)
						{
							add_sprite(den);
							den->xy.bool_cor = -1;
						}
						retry_write(den, &vec);
						den->blockwall = 0;
						den->button = den->button == 1 ? 0 : 1;
						SDL_FreeCursor(den->cursor);
					}
					else if (e.button.x <= den->border) 
					{	
						if (den->sec > 0 && den->blouk == 1)
						{
							den->xy.x = (((float) e.button.x - den->startx) / (START + den->skape) + 0.5);
							den->xy.y = (((float) e.button.y - den->starty) / (START + den->skape) + 0.5);

						 	in_all_sect(den);
						 	printf("xy_inside = %d\n", den->xy.inside);
						 	if (den->xy.bool_cor != -1)
							 	den->incede = 1;
						}
						coordinate_network(e.button.x, e.button.y, den, &vec);
						save_map(e.button.x, den, &vec);
						clean_display(den);
						info_display(den);
					}
				}
				else if (e.button.button == SDL_BUTTON_LEFT && den->select == 1 && e.button.x <= den->border)
					blouk_write(den);
				if (e.button.button == SDL_BUTTON_RIGHT && den->select == 1)
				{
					if (den->change == 3)
					{
						retry_write(den, &vec);
						den->change = 0;

					} 
					if ((e.button.x < den->player.x * XSKAPE + 10 && e.button.x > den->player.x * XSKAPE - 10) 
						&& (e.button.y < den->player.y * YSKAPE + 10 && e.button.y > den->player.y * YSKAPE - 10))
							change_way(den, 1, &vec);
					else
						find_wals(e.button.x, e.button.y, den);
					find_sprite(den, e.button.x , e.button.y);
					clean_display(den);
					info_display(den);
				}
			}
			else if (e.type == SDL_MOUSEBUTTONUP)
			{
				if (e.button.button == SDL_BUTTON_LEFT && e.button.x <= den->border 
				&& den->blockwall == 0 && den->select == 0) // нажимается всеравно при блоквалл
				{	
					//printf("button = %d\n", den->button);
					den->button = den->button == 1 ? 0 : 1;
				}
				if (e.button.x > den->border)
					pres_button(e.button.x, e.button.y, den, &vec);
				
			} 
			else if (e.type == SDL_MOUSEWHEEL)
			{
				if (e.wheel.y > 0)
        			den->skape += 10;
        		if (e.wheel.y < 0)
        		{
        			if (START + den->skape > 10)
        				den->skape -= 10;
        		}
				clear_texture(den, &vec);
				map_network(den);
				info_display(den);
				if (den->sec > 0)
					blonk_on_now(den);
			}
		}
	}
	SDL_DestroyWindow(den->window);
	free_surface(den);
	system("leaks doom-nukem");
	SDL_Quit();
}


void print_list(t_wals *tmp, t_doom *den) /// фиксить листы
{
	while (tmp)
	{
		printf("x1 = %d\ny1 = %d\n", tmp->x, tmp->y);
		printf("x2 = %d\ny2 = %d\n", tmp->x1, tmp->y1);
		printf("inside = %d\n", tmp->inside);
		//printf("button = %d\n", den->button);
		// printf("walls = %d\n", tmp->indexwall);
		// printf("next_sector = %zu\n", tmp->next_sector);
		// printf("nextsector_wall = %zu\n", tmp->nextsector_wall);
		// printf("next = %zu\n", tmp->next);
	
		printf("sec = %d\n", tmp->sec);

		// printf("sec = %d\n", den->sec);
		// printf("secbac = %d\n\n", den->secbak);

		printf("blouk = %d\n\n", tmp->blouk);
	//	printf("indexwall = %d\n\n", tmp->indexwall);
		// printf("secbac = %d\n", den->secbak);
		// printf("xsec = %d\nysec = %d\n\n", den->xsec, den->ysec);
		// printf(" YOU\n");
		tmp = tmp->next;
	}
	printf(" END\n");
}

int		count_walls(t_wals *tmp)
{
	int		len;

	len = 0;
	while (tmp != 0)
	{
		tmp->id = len;
		len++;
		tmp = tmp->next;
	}
	return (len);
}

void	get_map(t_sdl *iw, t_doom *den)
{
	int		s;
	int		w;
	int		cw;
	int		i;
	int		inside;
	int		inside_start;
	t_sector_list *sl;
	t_wals	*tmp;

	iw->v.sc = den->sec;
	iw->v.ls = 0;
	cw = count_walls(den->tmp);
	iw->sectors = (t_sector *)malloc(iw->v.sc * sizeof(t_sector));
	sl = den->sectors;
	i = -1;
	while (++i < iw->v.sc)
	{
		sl->id = i;
		iw->sectors[i].nw = 0;
		iw->sectors[i].fr.n = sl->fr.n;
		iw->sectors[i].fr.z = sl->fr.z;
		iw->sectors[i].fr.t = sl->fr.t;
		iw->sectors[i].cl.n = sl->cl.n;
		iw->sectors[i].cl.z = sl->cl.z;
		iw->sectors[i].cl.t = sl->cl.t;
		iw->sectors[i].light = sl->light;
		sl = sl->next;
	}
	
	iw->walls = (t_wall *)malloc(cw * sizeof(t_wall));
	s = 0;
	iw->sectors[0].sw = 0;
	w = 0;
	inside = 0;
	inside_start = 0;
	tmp = den->tmp;
	while (tmp != 0)
	{
		if (tmp->sec != s)
		{
			iw->walls[w - 1].next = &iw->walls[inside_start];
			s++;
			iw->sectors[s].sw = w;
			inside = 0;
			inside_start = w;
		}
		iw->sectors[s].nw++;
		iw->walls[w].x = tmp->x * MAP_SCALE;
		iw->walls[w].y = -tmp->y * MAP_SCALE;
		iw->walls[w].t = tmp->tex;
		iw->walls[w].glass = tmp->glass;
		iw->walls[w].p = tmp->p;

		if (tmp->next_sector)
		{
			iw->walls[w].nextsector = tmp->next_sector->id;
			iw->walls[w].nextsector_wall = tmp->nextsector_wall->id;
		}
		else
		{
			iw->walls[w].nextsector = -1;
			iw->walls[w].nextsector_wall = -1;
		}

		if (inside != tmp->inside)
		{
			iw->walls[w - 1].next = &iw->walls[inside_start];
			inside_start = w;
			inside++;
		}
		else if (w != iw->sectors[s].sw)
		{
			iw->walls[w - 1].next = &iw->walls[w];
		}
		else
		{
			iw->sectors[s].fr.x = iw->walls[w].x;
			iw->sectors[s].fr.y = iw->walls[w].y;
			iw->sectors[s].cl.x = iw->walls[w].x;
			iw->sectors[s].cl.y = iw->walls[w].y;
		}
		w++;
		tmp = tmp->next;
	}
	iw->walls[w - 1].next = &iw->walls[inside_start];

	*den->iw.sprite = den->sprite;
	printf("DAROVA BLET\n");
}

void	give_date(t_doom *den, t_sdl *iw)
{
	t_sprite	*tmp;

	if (den->player.x && den->player.y)
	{
		iw->l.start_x = den->player.x * MAP_SCALE;
		if (iw->l.start_x % 2 == 0)
			iw->l.start_x += 1;
		iw->l.start_y = -den->player.y * MAP_SCALE;
		if (iw->l.start_y % 2 == 0)
			iw->l.start_y += 1;
	}
	iw->l.start_rot = den->player.introt;
	iw->l.win_sector = den->finish;
	tmp = den->sprite;
	while (tmp)
	{
		tmp->x *= MAP_SCALE;
		tmp->y *= -MAP_SCALE;
		tmp = tmp->next;
	}
}

void	unget_map(t_sdl *iw, t_doom *den, t_wals *tmp)
{
	int		i;
	int		j;
	t_sector_list	*sl;
	t_sprite		*tmp2;

	sl = den->sectors;
	i = -1;
	while (++i < iw->v.sc)
	{
		sl->fr.z = iw->sectors[i].fr.z;
		sl->fr.n = iw->sectors[i].fr.n;
		sl->fr.t = iw->sectors[i].fr.t;
		sl->cl.z = iw->sectors[i].cl.z;
		sl->cl.n = iw->sectors[i].cl.n;
		sl->cl.t = iw->sectors[i].cl.t;
		// +
		sl->light = iw->sectors[i].light;
		j = iw->sectors[i].sw - 1;
		while (++j < iw->sectors[i].sw + iw->sectors[i].nw)
		{
			tmp->tex = iw->walls[j].t;
			// +
			tmp->glass = iw->walls[j].glass;
			// +
			tmp->p = iw->walls[j].p;
			tmp = tmp->next;
		}
		sl = sl->next;
	}
	den->sprite = *den->iw.sprite;
	tmp2 = den->sprite;
	while (tmp2)
	{
		tmp2->x /= 400;
		tmp2->y /= -400;
		tmp2 = tmp2->next;
	}
}

void	main3d_edit(t_doom *den)//, t_wals *w)
{
	give_date(den, &den->iw);
	get_def_new(&den->iw);
	den->iw.win = SDL_CreateWindow("Guardians of the Galaxy",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_W, WINDOW_H + 300, SDL_WINDOW_SHOWN);
	SDL_SetRelativeMouseMode(den->iw.v.mouse_mode);
	den->iw.sur = SDL_GetWindowSurface(den->iw.win);
	circle(&den->iw.hud, FOOTX, FOOTY);
	draw_tex_to_select(&den->iw);
	draw_decor_tex_to_select(&den->iw);
	draw_menu(&den->iw);

	get_map(&den->iw, den);

	//find_max_min(&den->iw);
	set_sprites_z(&den->iw);
	get_sectors_ways(&den->iw);
	create_map(&den->iw);
	draw(&den->iw);
	SDL_UpdateWindowSurface(den->iw.win);
	main_loop(&den->iw);

	SDL_SetRelativeMouseMode(0);
	SDL_FreeSurface(den->iw.sur);
	SDL_DestroyWindow(den->iw.win);

	if (den->iw.v.sector_anim != 0)
		free(den->iw.v.sector_anim);
	if (den->iw.v.wall_anim != 0)
		free(den->iw.v.wall_anim);
	undo_animations(&den->iw);
	unget_map(&den->iw, den, den->tmp);
	free(den->iw.walls);
	free(den->iw.sectors);
	free_sector_ways(&den->iw);
	Mix_HaltMusic();
}

void	get_font_file(t_doom *den)
{
	void	*tmp;
	int		fd;
	SDL_RWops	*rw;

	den->iw.arial_font = 0;
	fd = open("fonts/ARIAL.TTF", READ_MAP);
	if (fd < 0)
		return;
	tmp = malloc(70000);
	den->iw.font_pack_size = read(fd, tmp, 70000);
	close(fd);
	den->iw.font_pack = malloc(den->iw.font_pack_size);
	ft_memcpy(den->iw.font_pack, tmp, den->iw.font_pack_size);
	free(tmp);
	rw = SDL_RWFromConstMem(den->iw.font_pack, den->iw.font_pack_size);
	den->iw.arial_font = TTF_OpenFontRW(rw, 0, 24);
	den->font = den->iw.arial_font;
}

int		get_3d_def_new(t_doom *den)
{
	int		ret;

	den->iw.v.game_mode = 0;
	den->iw.v.kernel = 1;
	den->iw.l.accel = 9.81f;
	den->iw.l.skybox = 13;
	den->iw.l.story = 9;
	den->iw.v.look_wall = (t_wall **)malloc(sizeof(t_wall *));
	den->iw.v.look_sector = (t_sector **)malloc(sizeof(t_sector *));
	den->iw.v.look_picture = (t_picture **)malloc(sizeof(t_picture *));
	den->iw.sprite = (t_sprite **)malloc(sizeof(t_sprite *));
	den->iw.vw_save = (t_save_wall **)malloc(sizeof(t_save_wall *));
	den->iw.checkpoints = 0;
	den->iw.sector_animations = 0;
	den->iw.wall_animations = 0;
	get_font_file(den);
	//den->iw.arial_font = TTF_OpenFont("fonts/ARIAL.TTF", 24);
	load_kernel(&den->iw.k, &den->iw);
	read_textures(&den->iw);
	read_sprites_textures(&den->iw);
	read_weapons_textures(&den->iw);
	get_sounds(&den->iw);
	if ((ret = check_all_validation(&den->iw)) == 0)
		return (0);
	get_packaging_textures(&den->iw);
	get_kernel_mem(&den->iw);
	get_kernels(&den->iw);
	get_guns(&den->iw);
	return (ret);
}

//NEW
int		main_new(t_doom *den)
{
	int		ret;

	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	ret = get_3d_def_new(den);
	if (ret == 0)
	{
		write(1, "Error, check your files which need to be exist\n", 47);
		return (0);
	}
	else if (ret == -1)
	{
		write(1, "OpenCL initialization error\n", 28);
		den->iw.v.kernel = 0;
	}
	den->color = 0xFFFFFF;
	den->b_color = 0x505050;

	ft_sdl_init(den);
	
	return (0);
}

int		usage()
{
	write(1, "usage: doom-nukem new|edit|game map_name\n", 41);
	return (0);
}

int		check_new(t_doom *den, char *fname)
{
	den->fd = open(fname, O_WRONLY);
	if (den->fd >= 0)
	{
		close(den->fd);
		return (0);
	}
	/*den->fd = open(fname, O_DIRECTORY);
	if (den->fd >= 0)
	{
		close(den->fd);
		return (0);
	}*/
	den->fd = open(fname, O_WRONLY | O_CREAT | S_IRUSR | S_IWUSR);
	if (den->fd < 0)
		return (0);
	close(den->fd);
	den->fname = ft_strdup(fname);
	return (1);
}

int		check_load(t_doom *den, char *fname)
{
	/*den->fd = open(fname, O_DIRECTORY);
	if (den->fd >= 0)
	{
		close(den->fd);
		return (0);
	}*/
	den->fd = open(fname, O_RDWR);
	if (den->fd < 0)
		return (0);
	close(den->fd);
	den->fname = ft_strdup(fname);
	return (1);
}

int		get_3d_def_edit(t_doom *den)
{
	int		ret;

	den->iw.v.game_mode = 0;
	den->iw.v.kernel = 1;
	den->iw.v.look_wall = (t_wall **)malloc(sizeof(t_wall *));
	den->iw.v.look_sector = (t_sector **)malloc(sizeof(t_sector *));
	den->iw.v.look_picture = (t_picture **)malloc(sizeof(t_picture *));
	den->iw.sprite = (t_sprite **)malloc(sizeof(t_sprite *));
	den->iw.vw_save = (t_save_wall **)malloc(sizeof(t_save_wall *));
	get_font_file(den);
	//den->iw.arial_font = TTF_OpenFont("fonts/ARIAL.TTF", 24);
	load_kernel(&den->iw.k, &den->iw);
	read_textures(&den->iw);
	read_sprites_textures(&den->iw);
	read_weapons_textures(&den->iw);
	get_sounds(&den->iw);
	if ((ret = check_all_validation(&den->iw)) == 0)
		return (0);
	get_packaging_textures(&den->iw);
	get_kernel_mem(&den->iw);
	get_kernels(&den->iw);
	get_guns(&den->iw);
	return (ret);
}

int		get_3d_def_game(t_doom *den)
{
	den->iw.v.game_mode = 1;
	den->iw.v.kernel = 1;
	den->iw.v.look_wall = (t_wall **)malloc(sizeof(t_wall *));
	den->iw.v.look_sector = (t_sector **)malloc(sizeof(t_sector *));
	den->iw.v.look_picture = (t_picture **)malloc(sizeof(t_picture *));
	den->iw.sprite = (t_sprite **)malloc(sizeof(t_sprite *));
	den->iw.vw_save = (t_save_wall **)malloc(sizeof(t_save_wall *));
	/*get_font_file(den);*/
	den->iw.arial_font = TTF_OpenFontRW(SDL_RWFromConstMem(den->iw.font_pack,
		den->iw.font_pack_size), 0, 24);
	if (den->iw.arial_font == 0)
	{
		write(1, "Error loading file\n", 20);
		return (0);
	}
	den->font = den->iw.arial_font;
	//den->iw.arial_font = TTF_OpenFont("fonts/ARIAL.TTF", 24);
	load_kernel(&den->iw.k, &den->iw);
	//read_textures(&den->iw);
	//read_sprites_textures(&den->iw);
	//read_weapons_textures(&den->iw);
	//get_packaging_textures(&den->iw);
	get_kernel_mem(&den->iw);
	get_kernels(&den->iw);
	get_guns(&den->iw);
	get_sounds_game(&den->iw);
	//get_sounds(&den->iw);
	return (1);
}

int		main_edit(t_doom *den)
{
	int		ret;

	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	ret = get_3d_def_edit(den);
	if (ret == 0)
	{
		write(1, "Error, check your files which need to be exist\n", 47);
		return (0);
	}
	else if (ret == -1)
	{
		write(1, "OpenCL initialization error\n", 28);
		den->iw.v.kernel = 0;
	}

	den->color = 0xFFFFFF;
	den->b_color = 0x505050;

	ft_sdl_init(den);

	return (0);
}

void	main3d_game(t_doom *den)//, t_wals *w)
{
	give_date(den, &den->iw);
	get_def_new(&den->iw);
	den->iw.win = SDL_CreateWindow("Guardians of the Galaxy",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
	SDL_SetRelativeMouseMode(den->iw.v.mouse_mode);
	den->iw.sur = SDL_GetWindowSurface(den->iw.win);
	circle(&den->iw.hud, FOOTX, FOOTY);
	get_map(&den->iw, den);
	set_sprites_z(&den->iw);
	get_sectors_ways(&den->iw);
	create_map(&den->iw);
	Mix_PlayMusic(den->iw.sounds.music[5], -1);
	image_loop(&den->iw, den->iw.t[den->iw.l.story]);
	menu_loop(&den->iw);
	Mix_HaltMusic();
	draw(&den->iw);
	SDL_UpdateWindowSurface(den->iw.win);
	main_loop(&den->iw);
	SDL_SetRelativeMouseMode(0);
	SDL_FreeSurface(den->iw.sur);
	SDL_DestroyWindow(den->iw.win);
	if (den->iw.v.sector_anim != 0)
		free(den->iw.v.sector_anim);
	if (den->iw.v.wall_anim != 0)
		free(den->iw.v.wall_anim);
	undo_animations(&den->iw);
	unget_map(&den->iw, den, den->tmp);
	free(den->iw.walls);
	free(den->iw.sectors);
	free_sector_ways(&den->iw);
}

int		main_game(t_doom *den)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	if (!get_3d_def_game(den))
		return (0);
	main3d_game(den);
	/*den->color = 0xFFFFFF;
	den->b_color = 0x505050;

	ft_sdl_init(den);*/
	return (0);
}

int		main(int argc, char **argv)
{
	t_doom	den;

	if (argc != 3)
		return (usage());
	if (ft_strcmp(argv[1], "new") == 0)
	{
		if (!check_new(&den, argv[2]))
		{
			write(1, "Error creating file\n", 20);
			return (0);
		}
		map_redactor_mane2(&den);
		main_new(&den);
	}
	else if (ft_strcmp(argv[1], "edit") == 0)
	{
		if (!check_load(&den, argv[2]))
		{
			write(1, "Error loading file\n", 19);
			return (0);
		}
		den.fd = open(den.fname, READ_MAP);
		if (!load_map(&den))
		{
			write(1, "Error loading file\n", 20);
			close(den.fd);
			return (0);
		}
		close(den.fd);
		main_edit(&den);

	}
	else if (ft_strcmp(argv[1], "game") == 0)
	{
		if (!check_load(&den, argv[2]))
		{
			write(1, "Error loading file\n", 19);
			return (0);
		}
		den.fd = open(den.fname, READ_MAP);
		if (!load_map(&den) || !load4_game(&den))
		{
			write(1, "Error loading file\n", 20);
			close(den.fd);
			return (0);
		}
		close(den.fd);
		main_game(&den);
	}
	else
		return (usage());
	return (0);
}