#include "../guardians.h"

void	update2(t_sdl *iw)
{
	if (iw->hud.saved_time != 1)
	{
		if (clock() - iw->hud.saved_time < HUD_SAVED_TIME)
			draw_save(iw);
		else
			iw->hud.saved_time = 1;
	}
	if (iw->bag.bag == 1 || iw->map.back == 1)
		transparency(iw);
	if (iw->bag.bag == 1)
		draw_items(iw);
	if (iw->map.back == 1)
		draw_minimap(iw);
	if (iw->v.game_mode)
		draw_icon_bag(iw);
	draw_selected_tex(iw);
	draw_selected_sprite(iw);
	SDL_UpdateWindowSurface(iw->win);
}

void	update(t_sdl *iw)
{
	SDL_FillRect(iw->sur, &iw->winrect, 0x000000);
	draw(iw);
	draw_some_info(iw);
	if (iw->v.game_mode)
	{
		make_health(&iw->hud, iw);
		draw_checkpoint_text(iw);
	}
	if (iw->bag.bag != 1 && iw->map.back != 1)
		draw_crosshair(iw);
	if (iw->hud.miss_time != 1)
	{
		if (clock() - iw->hud.miss_time < HUD_MISS_TIME)
			draw_miss(iw);
		else
			iw->hud.miss_time = 1;
	}
	update2(iw);
}