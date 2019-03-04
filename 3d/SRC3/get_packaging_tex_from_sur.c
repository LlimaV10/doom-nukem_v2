#include "../guardians.h"

void	get_packaging_texture(t_packaging_texture **pack, SDL_Surface *sur)
{
	*pack = (t_packaging_texture *)malloc(sizeof(t_packaging_texture));
	(*pack)->w = sur->w;
	(*pack)->h = sur->h;
	(*pack)->pitch = sur->pitch;
	(*pack)->bpp = sur->format->BytesPerPixel;
	(*pack)->pixels = malloc(sur->pitch * sur->h);
	ft_memcpy((*pack)->pixels, sur->pixels, sur->pitch * sur->h);
	SDL_FreeSurface(sur);
}

void	get_packaging_textures2(t_sdl *iw)
{
	int		i;

	get_packaging_texture(&iw->bag.button[0], iw->bag.button_sur[0]);
	get_packaging_texture(&iw->bag.button[1], iw->bag.button_sur[1]);
	get_packaging_texture(&iw->bag.button[2], iw->bag.button_sur[2]);
	i = -1;
	while (++i < 6)
		get_packaging_texture(&iw->menu.icons[i], iw->menu.icons_sur[i]);
	get_packaging_texture(&iw->map.player, iw->map.player_sur);
}

void	get_packaging_textures(t_sdl *iw)
{
	int		i;

	get_packaging_texture(&iw->hud.enot, iw->hud.enot_sur);
	get_packaging_texture(&iw->hud.miss, iw->hud.miss_sur);
	get_packaging_texture(&iw->hud.win, iw->hud.win_sur);
	get_packaging_texture(&iw->hud.dead, iw->hud.dead_sur);
	get_packaging_texture(&iw->hud.saved, iw->hud.saved_sur);
	i = -1;
	while (++i < TEXTURES_COUNT)
		get_packaging_texture(&iw->t[i], iw->t_sur[i]);
	i = -1;
	while (++i < DECOR_TEXTURES_COUNT)
		get_packaging_texture(&iw->t_decor[i], iw->t_decor_sur[i]);
	i = -1;
	while (++i < ENEMIES_TEXTURES_COUNT)
		get_packaging_texture(&iw->t_enemies[i], iw->t_enemies_sur[i]);
	i = -1;
	while (++i < PICK_UP_TEXTURES_COUNT)
		get_packaging_texture(&iw->t_pickup[i], iw->t_pickup_sur[i]);
	i = -1;
	while (++i < WEAPONS_TEXTURES_COUNT)
		get_packaging_texture(&iw->t_weap[i], iw->t_weap_sur[i]);
	get_packaging_textures2(iw);
}