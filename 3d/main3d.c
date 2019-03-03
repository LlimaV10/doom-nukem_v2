#include "guardians.h"
#include <stdio.h>







// HUD ////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////



//BACKPACK/////////////////////////////////////////////////////////


// void	draw_item(t_packaging_texture *tex, SDL_Surface *winsur, SDL_Rect *rect)
// {
// 	int		i;
// 	int		j;
//     int     color;

// 	i = -1;
// 	while (++i < rect->w)
// 	{
// 		j = -1;
// 		while (++j < rect->h)
//         {
//             color = get_pixel(tex, tex->w * j / rect->w, tex->h * i / rect->h);
//             if (color != 0x010000)
//                 set_pixel2(winsur, rect->x + j, rect->y + i, color);
//         }
// 	}
	
// }

//////////////////// NEED TO DELETE SPRITE AND DO SOME...


// just deletes item from inventory



// void draw_button(t_sdl	*iw, SDL_Rect	*rect, t_packaging_texture *texture)
// {
// 	int x;
// 	int y;
// 	int color;

// 	x = -1;
// 	while(++x < rect->w)
// 	{
// 		y = -1;
// 		while(++y < rect->h)
// 		{
// 			color = get_pixel(texture, texture->w * x / rect->w, texture->h * y / rect->h);
//             if (color != 0xff00e3 && color != 0x000000)
//                 set_pixel2(iw->sur, rect->x + x, rect->y + y, color);
// 		}
// 	}
// }

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\







// ENEMIES FUNCTIONS /////////////////////////////////////////////////////////////

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\



//void	fill_bot_by_wallTop(t_sdl *iw, t_save_wall *left, int len)
//{
//	int		i;
//
//	i = -1;
//	while (++i < len)
//		if (iw->d.wallTop[i] < iw->d.bottom[left->x + i])
//			iw->d.bottom[left->x + i] = iw->d.wallTop[i];
//}



// int		portal_not_visited(t_sdl *iw, t_wall *wall)
// {
// 	t_visited_portal	*p;

// 	p = iw->visited_portals;
// 	while (p)
// 	{
// 		if (wall == p->wall)
// 			return (0);
// 		p = p->next;
// 	}
// 	return (1);
// }











// int		get_max(int i1, int i2)
// {
// 	if (i1 > i2)
// 		return (i1);
// 	else
// 		return (i2);
// }

// int		get_min(int i1, int i2)
// {
// 	if (i1 < i2)
// 		return (i1);
// 	else
// 		return (i2);
// }
// SPRITES DRAW /////////////////////////////////////////////////////////////////////////////////////////////////////




//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\





// void add_sprite(t_sdl *iw, int x, int y, int z, int t, int num, int type, float scale)
// {
// 	t_sprite *tmp;

// 	tmp = (t_sprite *)malloc(sizeof(t_sprite));
// 	tmp->x = x;
// 	tmp->y = y;
// 	tmp->z = z;
// 	//tmp->t = t;
// 	tmp->num_sec = num;
// 	tmp->next = *iw->sprite;
// 	tmp->type = type;
// 	tmp->scale = scale;
// 	tmp->e.vis_esp.curr_sector = -1;
// 	tmp->e.previous_picture_change = 1;
// 	tmp->e.prev_update_time = 1;
// 	tmp->fall_time = 1;

// 	tmp->taken = 0;

// 	//tmp->e.previous_move_to_player_time = 1;
// 	(*iw->sprite) = tmp;
// 	tmp->t_numb = t;
// 	if (type == 0)
// 	{
// 		tmp->t = iw->t_decor[t];
// 		tmp->t_kernel = &iw->k.m_t_decor[t];
// 	}
// 	else if (type == 1)
// 	{
// 		tmp->t = iw->t_pickup[t];
// 		tmp->t_kernel = &iw->k.m_t_pickup[t];
// 	}
// 	else if (type == 2)
// 	{
// 		tmp->t = iw->t_enemies[t];
// 		tmp->t_kernel = &iw->k.m_t_enemies[t];
// 	}
	
// }









//void	add_picture1(t_sdl *iw)
//{
//	t_picture	*tmp;
//
//	tmp = (t_picture *)malloc(sizeof(t_picture));
//	tmp->left_plus = 600;
//	tmp->zu = 1000;
//	tmp->tw = 500;
//	tmp->t = 15;
//	tmp->next = 0;
//	iw->walls[5].p = tmp;
//	calculate_picture(iw, &iw->walls[5], tmp);
//	tmp = (t_picture *)malloc(sizeof(t_picture));
//	tmp->left_plus = 600;
//	tmp->zu = 1000;
//	tmp->tw = 500;
//	tmp->t = 5;
//	tmp->next = 0;
//	iw->walls[18].p = tmp;
//	calculate_picture(iw, &iw->walls[18], tmp);
//	tmp = (t_picture *)malloc(sizeof(t_picture));
//	tmp->left_plus = 600;
//	tmp->zu = 1000;
//	tmp->tw = 500;
//	tmp->t = 6;
//	tmp->next = 0;
//	iw->walls[19].p = tmp;
//	calculate_picture(iw, &iw->walls[19], tmp);
//	tmp = (t_picture *)malloc(sizeof(t_picture));
//	tmp->left_plus = 600;
//	tmp->zu = 1000;
//	tmp->tw = 500;
//	tmp->t = 7;
//	tmp->next = 0;
//	iw->walls[20].p = tmp;
//	calculate_picture(iw, &iw->walls[20], tmp);
//}

//	GET_SECTOR_WAYS_FUNCTIONS //////////////////////////////////////////////////////////


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\







//void	find_max_min(t_sdl *iw)
//{
//	int sec;
//	int w;
//
//	sec = -1;
//	iw->map.max_x = iw->walls->x;
//	iw->map.max_y = iw->walls->y;
//	iw->map.min_x = iw->walls->x;
//	iw->map.min_y = iw->walls->y;
//    while (++sec < iw->v.sc)
//    {
//        w = iw->sectors[sec].sw - 1;
//        while (++w < iw->sectors[sec].sw + iw->sectors[sec].nw)
//	{
//		iw->map.max_x = ft_max(iw->map.max_x, iw->walls[w].x);
//		iw->map.max_y = ft_max(iw->map.max_y, iw->walls[w].y);
//		iw->map.min_x = ft_min(iw->map.min_x, iw->walls[w].x);
//		iw->map.min_y = ft_min(iw->map.min_y, iw->walls[w].y);
//	}
//	}
//}







// int		main3d(void)
// {
// 	t_sdl	iw;
	
// 	iw.v.game_mode = 1;
// 	get_def(&iw);
// 	read_textures(&iw);
// 	read_sprites_textures(&iw);
// 	read_weapons_textures(&iw);
// 	get_packaging_textures(&iw);
// 	get_kernel_mem(&iw);
// 	get_kernels(&iw);
// 	get_guns(&iw);
// 	//enemy
// 	// add_sprite(&iw, 4700, -900, 0, 9, 0, 0, 0.5f);
// 	// (*iw.sprite)->type = 2;
// 	// (*iw.sprite)->e.enemy_numb = 1;
// 	// (*iw.sprite)->e.health = ENEMY_HEALTH1;
// 	// (*iw.sprite)->e.status = 1;
// 	// (*iw.sprite)->t = iw.t_enemies[9];
// 	// (*iw.sprite)->t_kernel = &iw.k.m_t_enemies[9];

// 	//add_sprite(&iw, 9151, 2272, 0, 20, 1, 0, 0.5f);
// 	// (*iw.sprite)->type = 2;
// 	// (*iw.sprite)->e.enemy_numb = 2;
// 	// (*iw.sprite)->e.health = ENEMY_HEALTH2;
// 	// (*iw.sprite)->e.status = 0;
// 	// (*iw.sprite)->t = iw.t_enemies[20];
// 	// (*iw.sprite)->t_kernel = &iw.k.m_t_enemies[20];
// 	//pickup

// 	// add_sprite(&iw, 7240, 2640, 200, 0, 1, 0, 0.5f);
// 	// add_sprite(&iw, 6520, 2298, 200, 0, 1, 2, 0.5f);

// 	//add_sprite(&iw, 8640, 2200, 200, 1, 1, 0, 0.1f);
// 	// iw.checkpoints = (t_keyb_inp *)malloc(sizeof(t_keyb_inp));
// 	// iw.checkpoints->next = 0;
// 	// ft_memcpy(iw.checkpoints->s, "Hello World!Hello World!Hello World!Hello World!Hello World!Hello World!Hello World!", 32);
// 	// iw.checkpoints->s_len = 32;
// 	// iw.checkpoints->sprite = *iw.sprite;
// 	// add_sprite(&iw, 7500, 2000, 200, 1, 1, 1, 0.5f);
// 	// add_sprite(&iw, 7500, 3000, 200, 2, 1, 1, 0.5f);
// 	// // add_sprite(&iw, 7500, 3000, 200, 3, 1, 1, 0.5f);

// 	// add_sprite(&iw, 8500, 1500, 200, 3, 1, 1, 0.5f);
// 	// add_sprite(&iw, 9000, 5500, 200, 4, 1, 1, 0.5f);
// 	// add_sprite(&iw, 10000, 500, 200, 5, 1, 1, 0.5f);

// 	// add_sprite(&iw, 9500, 5000, 200, 6, 1, 1, 0.5f);
// 	// add_sprite(&iw, 7500, 3000, 200, 7, 1, 1, 0.5f);
// 	// add_sprite(&iw, 7500, 3000, 200, 8, 1, 1, 0.5f);

// 	// add_sprite(&iw, 8500, 1000, 200, 9, 1, 1, 0.5f);
// 	// add_sprite(&iw, 8000, 5500, 200, 10, 1, 1, 0.5f);

// 	/*add_sprite(&iw,8640,2200,400, 0, 1, 0, 1.0f);
// 	add_sprite(&iw,6520,2298,200, 0, 1, 0, 1.0f);*/


// 	SDL_Init(SDL_INIT_EVERYTHING);
// 	TTF_Init();
// 	iw.arial_font = TTF_OpenFont("fonts/ARIAL.TTF", 24);
// 	SDL_SetRelativeMouseMode(iw.v.mouse_mode);
// 	if (!iw.v.game_mode)
// 		iw.win = SDL_CreateWindow("SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
// 			WINDOW_W, WINDOW_H + 300, SDL_WINDOW_SHOWN);
// 	else
// 		iw.win = SDL_CreateWindow("SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
// 			WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
// 	//SDL_SetWindowFullscreen(iw.win, SDL_WINDOW_FULLSCREEN_DESKTOP);
// 	//iw.ren = SDL_CreateRenderer(iw.win, -1, 0);
// 	iw.sur = SDL_GetWindowSurface(iw.win);
// 	if (iw.v.game_mode)
// 	{
// 		image_loop(&iw, iw.t[iw.l.story]);
// 		menu_loop(&iw);
// 	}
// 	circle(&iw.hud, FOOTX, FOOTY);
// 	draw_tex_to_select(&iw);
// 	draw_decor_tex_to_select(&iw);
// 	draw_menu(&iw);
// 	// draw
// 	//get_map(&iw);

// 	find_max_min(&iw);

// 	set_sprites_z(&iw);
// 	get_sectors_ways(&iw);
// 	add_picture1(&iw); ///////////////
// 	create_map(&iw);
// 	draw(&iw);
// 	SDL_UpdateWindowSurface(iw.win);
// 	main_loop(&iw);
// 	SDL_FreeSurface(iw.sur);
// 	SDL_DestroyWindow(iw.win);
// 	TTF_Quit();
// 	SDL_Quit();
// 	// system("PAUSE");
// 	return (0);
// }



