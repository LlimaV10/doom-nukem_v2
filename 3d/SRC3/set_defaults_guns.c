#include "../guardians.h"

void	get_guns_center(t_sdl *iw, int i, int scale)
{
	scale /= 2;
	iw->guns.t_rect[i].w = iw->t_weap[i]->w * WINDOW_W / scale;
	iw->guns.t_rect[i].h = iw->guns.t_rect[i].w *
		iw->t_weap[i]->h / iw->t_weap[i]->w;
	iw->guns.t_rect[i].x = (WINDOW_W - iw->guns.t_rect[i].w) / 2;
	iw->guns.t_rect[i].y = (WINDOW_H - iw->guns.t_rect[i].h) / 2;
}

void	get_guns_center_down(t_sdl *iw, int i, int scale)
{
	scale /= 2;
	iw->guns.t_rect[i].w = iw->t_weap[i]->w * WINDOW_W / scale;
	iw->guns.t_rect[i].h = iw->guns.t_rect[i].w *
		iw->t_weap[i]->h / iw->t_weap[i]->w;
	iw->guns.t_rect[i].x = (WINDOW_W - iw->guns.t_rect[i].w) / 2;
	iw->guns.t_rect[i].y = WINDOW_H - iw->guns.t_rect[i].h;
}

void	get_guns3(t_sdl *iw)
{
	iw->guns.t_rect[7].x += iw->guns.t_rect[7].w * 3 / 5;
	get_guns_center_down(iw, 8, 2500);
	iw->guns.t_rect[8].x += iw->guns.t_rect[8].w * 3 / 5;
	get_guns_center_down(iw, 9, 2500);
	iw->guns.t_rect[9].x += iw->guns.t_rect[9].w * 3 / 5;
	get_guns_center_down(iw, 10, 1500);
	get_guns_center_down(iw, 11, 1500);
	get_guns_center_down(iw, 12, 1500);
	get_guns_center_down(iw, 13, 1500);
	get_guns_center_down(iw, 14, 1500);
	get_guns_center_down(iw, 15, 1500);
	get_guns_center_down(iw, 16, 1500);
	iw->guns.prev_update_time = clock();
}

void	get_guns2(t_sdl *iw)
{
	get_guns_center(iw, 3, 1700);
	iw->guns.t_rect[3].x += WINDOW_W / 9;
	iw->guns.t_rect[3].y -= WINDOW_H / 3;
	get_guns_center(iw, 4, 2200);
	iw->guns.t_rect[4].x += WINDOW_W / 8;
	iw->guns.t_rect[4].y -= WINDOW_H / 3;
	get_guns_center(iw, 5, 2200);
	iw->guns.t_rect[5].x += WINDOW_W / 7;
	iw->guns.t_rect[5].y -= WINDOW_H / 5;
	get_guns_center(iw, 6, 2200);
	iw->guns.t_rect[6].x += WINDOW_W / 6;
	iw->guns.t_rect[6].y += WINDOW_H / 10;
	get_guns_center_down(iw, 7, 2500);
	get_guns3(iw);
}

void	get_guns(t_sdl *iw)
{
	iw->guns.t = 17;
	iw->guns.max_bullets[0] = 1;
	iw->guns.max_bullets[1] = 4;
	iw->guns.max_bullets[2] = 10;
	iw->guns.max_bullets_in_stock[0] = 0;
	iw->guns.max_bullets_in_stock[1] = 40;
	iw->guns.max_bullets_in_stock[2] = 100;
	iw->guns.bullets[0] = iw->guns.max_bullets[0];
	iw->guns.bullets[1] = iw->guns.max_bullets[1];
	iw->guns.bullets[2] = iw->guns.max_bullets[2];
	iw->guns.bullets_in_stock[0] = 0;
	iw->guns.bullets_in_stock[1] = 8;
	iw->guns.bullets_in_stock[2] = 10;
	iw->guns.gun_in_hands = 0;
	iw->guns.status = 0;
	get_guns_center_down(iw, 17, 2000);
	get_guns_center_down(iw, 18, 2000);
	get_guns_center_down(iw, 0, 3000);
	iw->guns.t_rect[0].x += iw->guns.t_rect[0].w / 3 * 2;
	get_guns_center_down(iw, 1, 3000);
	iw->guns.t_rect[1].x += iw->guns.t_rect[1].w * 4 / 5;
	get_guns_center(iw, 2, 1700);
	iw->guns.t_rect[2].x += WINDOW_W / 10;
	iw->guns.t_rect[2].y -= WINDOW_H / 5;
	get_guns2(iw);
}