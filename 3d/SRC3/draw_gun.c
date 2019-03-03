#include "../guardians.h"

void	draw_gun(t_sdl *iw)
{
	int		i;
	int		j;
	int		to_i;
	int		start_j;
	int		to_j;
	int		pixel;
	SDL_Rect	changed_rect;

	changed_rect = iw->guns.t_rect[iw->guns.t];
	changed_rect.x += iw->v.weapon_change_x;
	changed_rect.y += iw->v.weapon_change_y + iw->v.weapon_change_y_hide;
	to_i = changed_rect.y + changed_rect.h;
	if (to_i > WINDOW_H)
		to_i = WINDOW_H;
	if (changed_rect.y < 0)
		i = -1;
	else
		i = changed_rect.y - 1;
	if (changed_rect.x < 0)
		start_j = -1;
	else
		start_j = changed_rect.x - 1;
	to_j = changed_rect.w + changed_rect.x;
	if (to_j > WINDOW_W)
		to_j = WINDOW_W;
	while (++i < to_i)
	{
		j = start_j;
		while (++j < to_j)
		{
			pixel = get_pixel(iw->t_weap[iw->guns.t],
				(j - changed_rect.x) * iw->t_weap[iw->guns.t]->w / changed_rect.w,
				(i - changed_rect.y) * iw->t_weap[iw->guns.t]->h / changed_rect.h);
			if (pixel != 0x010000)
				set_pixel2(iw->sur, j, i, get_light_color(pixel, iw->sectors[iw->d.cs].light));
		}
	}
}