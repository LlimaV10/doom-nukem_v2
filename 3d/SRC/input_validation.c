#include "../guardians.h"

int		check_textures(t_sdl *iw)
{
	int		i;

	i = -1;
	while (++i < TEXTURES_COUNT)
		if (iw->t_sur[i] == 0)
			return (0);
	i = -1;
	while (++i < DECOR_TEXTURES_COUNT)
		if (iw->t_decor_sur[i] == 0)
			return (0);
	i = -1;
	while (++i < PICK_UP_TEXTURES_COUNT)
		if (iw->t_pickup_sur[i] == 0)
			return (0);
	i = -1;
	while (++i < ENEMIES_TEXTURES_COUNT)
		if (iw->t_enemies_sur[i] == 0)
			return (0);
	i = -1;
	while (++i < WEAPONS_TEXTURES_COUNT)
		if (iw->t_weap_sur[i] == 0)
			return (0);
	if (iw->hud.enot_sur == 0)
		return (0);
	if (iw->hud.miss_sur == 0)
		return (0);
	if (iw->hud.dead_sur == 0)
		return (0);
	if (iw->hud.win_sur == 0)
		return (0);
	i = -1;
	while (++i < 3)
		if (iw->bag.button_sur[i] == 0)
			return (0);
	if (iw->map.player_sur == 0)
		return (0);
	i = -1;
	while (++i < 6)
		if (iw->menu.icons_sur[i] == 0)
			return (0);
	return (1);
}

int		check_sound(t_sdl *iw)
{
	int		i;

	i = -1;
	while (++i < MUSIC_COUNT)
		if (iw->sounds.music[i] == 0)
			return (0);
	i = -1;
	while (++i < ENV_COUNT)
		if (iw->sounds.env[i] == 0)
			return (0);
	return (1);
}

int		check_all_validation(t_sdl *iw)
{
	if (!check_textures(iw))
		return (0);
	if (!check_sound(iw))
		return (0);
	if (iw->k.ret != 0)
		return (-1);
	if (iw->arial_font == 0)
		return (0);

	return (1);
}