#include "../guardians.h"

void	read_textures(t_sdl *iw)
{
	iw->t_sur[0] = SDL_LoadBMP("textures/0.bmp");
	iw->tsz[0] = 1.0f;
	iw->t_sur[1] = SDL_LoadBMP("textures/1.bmp");
	iw->tsz[1] = 1.0f;
	iw->t_sur[2] = SDL_LoadBMP("textures/2.bmp");
	iw->tsz[2] = 1.0f;
	iw->t_sur[3] = SDL_LoadBMP("textures/3.bmp");
	iw->tsz[3] = 1.0f;
	iw->t_sur[4] = SDL_LoadBMP("textures/4.bmp");
	iw->tsz[4] = 1.0f;
	iw->t_sur[5] = SDL_LoadBMP("textures/5.bmp");
	iw->tsz[5] = 1.0f;
	iw->t_sur[6] = SDL_LoadBMP("textures/6.bmp");
	iw->tsz[6] = 1.0f;
	iw->t_sur[7] = SDL_LoadBMP("textures/7.bmp");
	iw->tsz[7] = 1.0f;
	iw->t_sur[8] = SDL_LoadBMP("textures/8.bmp");
	iw->tsz[8] = 1.0f;
	iw->t_sur[9] = SDL_LoadBMP("textures/9.bmp");
	iw->tsz[9] = 1.0f;
	iw->t_sur[10] = SDL_LoadBMP("textures/10.bmp");
	iw->tsz[10] = 1.0f;
	iw->t_sur[11] = SDL_LoadBMP("textures/11.bmp");
	iw->tsz[11] = 1.0f;
	iw->t_sur[12] = SDL_LoadBMP("textures/12.bmp");
	iw->tsz[12] = 1.0f;
	iw->t_sur[13] = SDL_LoadBMP("textures/13.bmp");
	iw->tsz[13] = 1.0f;
	iw->t_sur[14] = SDL_LoadBMP("textures/14.bmp");
	iw->tsz[14] = 1.0f;
	iw->t_sur[15] = SDL_LoadBMP("textures/15.bmp");
	iw->tsz[15] = 1.0f;
	iw->t_sur[16] = SDL_LoadBMP("textures/16.bmp");
	iw->tsz[16] = 1.0f;
	iw->t_sur[17] = SDL_LoadBMP("textures/17.bmp");
	iw->tsz[17] = 1.0f;
	iw->t_sur[18] = SDL_LoadBMP("textures/18.bmp");
	iw->tsz[18] = 1.0f;
	iw->t_sur[19] = SDL_LoadBMP("textures/19.bmp");
	iw->tsz[19] = 1.0f;
	iw->t_sur[20] = SDL_LoadBMP("textures/20.bmp");
	iw->tsz[20] = 1.0f;
	iw->t_sur[21] = SDL_LoadBMP("textures/21.bmp");
	iw->tsz[21] = 1.0f;
	iw->t_sur[22] = SDL_LoadBMP("textures/22.bmp");
	iw->tsz[22] = 1.0f;
	iw->t_sur[23] = SDL_LoadBMP("textures/23.bmp");
	iw->tsz[23] = 1.0f;
	iw->t_sur[24] = SDL_LoadBMP("textures/24.bmp");
	iw->tsz[24] = 1.0f;
	
	iw->bag.button_sur[0] = SDL_LoadBMP("interface_textures/backpack/frame.bmp");
	iw->bag.button_sur[1] = SDL_LoadBMP("interface_textures/backpack/del.bmp");
	iw->bag.button_sur[2] = SDL_LoadBMP("interface_textures/backpack/ok.bmp");

	iw->menu.icons_sur[0] = SDL_LoadBMP("interface_textures/menu/0.bmp");
	iw->menu.icons_sur[1] = SDL_LoadBMP("interface_textures/menu/1.bmp");
	iw->menu.icons_sur[2] = SDL_LoadBMP("interface_textures/menu/2.bmp");
	iw->menu.icons_sur[3] = SDL_LoadBMP("interface_textures/menu/3.bmp");
	iw->menu.icons_sur[4] = SDL_LoadBMP("interface_textures/menu/4.bmp");
	iw->menu.icons_sur[5] = SDL_LoadBMP("interface_textures/menu/5.bmp");

	iw->map.player_sur = SDL_LoadBMP("interface_textures/map/player.bmp");
	iw->hud.enot_sur = SDL_LoadBMP("interface_textures/HUD/enot.bmp");
	iw->hud.miss_sur = SDL_LoadBMP("interface_textures/HUD/miss.bmp");
	iw->hud.dead_sur = SDL_LoadBMP("interface_textures/HUD/groot_lose.bmp");
	iw->hud.win_sur = SDL_LoadBMP("interface_textures/HUD/groot_win.bmp");
	iw->hud.saved_sur = SDL_LoadBMP("interface_textures/HUD/saved.bmp");
}

void	read_sprites_textures(t_sdl *iw)
{
	iw->t_decor_sur[0] = SDL_LoadBMP("sprites/decorations/0.bmp");
	iw->t_decor_sur[1] = SDL_LoadBMP("sprites/decorations/1.bmp");
	/*iw->t_decor[1] = SDL_LoadBMP("sprites/decorations/1.bmp");
	iw->t_decor[2] = SDL_LoadBMP("sprites/decorations/2.bmp");
	iw->t_decor[3] = SDL_LoadBMP("sprites/decorations/3.bmp");*/

	iw->t_enemies_sur[0] = SDL_LoadBMP("sprites/enemies/0.bmp");
	iw->t_enemies_sur[1] = SDL_LoadBMP("sprites/enemies/1.bmp");
	iw->t_enemies_sur[2] = SDL_LoadBMP("sprites/enemies/2.bmp");
	iw->t_enemies_sur[3] = SDL_LoadBMP("sprites/enemies/3.bmp");
	iw->t_enemies_sur[4] = SDL_LoadBMP("sprites/enemies/4.bmp");
	iw->t_enemies_sur[5] = SDL_LoadBMP("sprites/enemies/5.bmp");
	iw->t_enemies_sur[6] = SDL_LoadBMP("sprites/enemies/6.bmp");
	iw->t_enemies_sur[7] = SDL_LoadBMP("sprites/enemies/7.bmp");
	iw->t_enemies_sur[8] = SDL_LoadBMP("sprites/enemies/8.bmp");
	iw->t_enemies_sur[9] = SDL_LoadBMP("sprites/enemies/9.bmp");
	iw->t_enemies_sur[10] = SDL_LoadBMP("sprites/enemies/10.bmp");
	iw->t_enemies_sur[11] = SDL_LoadBMP("sprites/enemies/11.bmp");
	iw->t_enemies_sur[12] = SDL_LoadBMP("sprites/enemies/12.bmp");
	iw->t_enemies_sur[13] = SDL_LoadBMP("sprites/enemies/13.bmp");
	iw->t_enemies_sur[14] = SDL_LoadBMP("sprites/enemies/14.bmp");
	iw->t_enemies_sur[15] = SDL_LoadBMP("sprites/enemies/15.bmp");
	iw->t_enemies_sur[16] = SDL_LoadBMP("sprites/enemies/16.bmp");
	iw->t_enemies_sur[17] = SDL_LoadBMP("sprites/enemies/17.bmp");
	iw->t_enemies_sur[18] = SDL_LoadBMP("sprites/enemies/18.bmp");
	iw->t_enemies_sur[19] = SDL_LoadBMP("sprites/enemies/19.bmp");

	iw->t_enemies_sur[20] = SDL_LoadBMP("sprites/enemies/20.bmp");
	iw->t_enemies_sur[21] = SDL_LoadBMP("sprites/enemies/21.bmp");
	iw->t_enemies_sur[22] = SDL_LoadBMP("sprites/enemies/22.bmp");
	iw->t_enemies_sur[23] = SDL_LoadBMP("sprites/enemies/23.bmp");
	iw->t_enemies_sur[24] = SDL_LoadBMP("sprites/enemies/24.bmp");
	iw->t_enemies_sur[25] = SDL_LoadBMP("sprites/enemies/25.bmp");
	iw->t_enemies_sur[26] = SDL_LoadBMP("sprites/enemies/26.bmp");
	iw->t_enemies_sur[27] = SDL_LoadBMP("sprites/enemies/27.bmp");
	iw->t_enemies_sur[28] = SDL_LoadBMP("sprites/enemies/28.bmp");
	iw->t_enemies_sur[29] = SDL_LoadBMP("sprites/enemies/29.bmp");
	

	iw->t_pickup_sur[0] = SDL_LoadBMP("sprites/to_pick_up/0.bmp");
	iw->t_pickup_sur[1] = SDL_LoadBMP("sprites/to_pick_up/1.bmp");
	iw->t_pickup_sur[2] = SDL_LoadBMP("sprites/to_pick_up/2.bmp");
	iw->t_pickup_sur[3] = SDL_LoadBMP("sprites/to_pick_up/3.bmp");
	iw->t_pickup_sur[4] = SDL_LoadBMP("sprites/to_pick_up/4.bmp");
	iw->t_pickup_sur[5] = SDL_LoadBMP("sprites/to_pick_up/5.bmp");
	iw->t_pickup_sur[6] = SDL_LoadBMP("sprites/to_pick_up/6.bmp");
	iw->t_pickup_sur[7] = SDL_LoadBMP("sprites/to_pick_up/7.bmp");
	iw->t_pickup_sur[8] = SDL_LoadBMP("sprites/to_pick_up/8.bmp");
	iw->t_pickup_sur[9] = SDL_LoadBMP("sprites/to_pick_up/9.bmp");
	iw->t_pickup_sur[10] = SDL_LoadBMP("sprites/to_pick_up/10.bmp");
}

void	read_weapons_textures(t_sdl *iw)
{
	iw->t_weap_sur[0] = SDL_LoadBMP("weapons/0.bmp");
	iw->t_weap_sur[1] = SDL_LoadBMP("weapons/1.bmp");
	iw->t_weap_sur[2] = SDL_LoadBMP("weapons/2.bmp");
	iw->t_weap_sur[3] = SDL_LoadBMP("weapons/3.bmp");
	iw->t_weap_sur[4] = SDL_LoadBMP("weapons/4.bmp");
	iw->t_weap_sur[5] = SDL_LoadBMP("weapons/5.bmp");
	iw->t_weap_sur[6] = SDL_LoadBMP("weapons/6.bmp");
	iw->t_weap_sur[7] = SDL_LoadBMP("weapons/7.bmp");
	iw->t_weap_sur[8] = SDL_LoadBMP("weapons/8.bmp");
	iw->t_weap_sur[9] = SDL_LoadBMP("weapons/9.bmp");
	iw->t_weap_sur[10] = SDL_LoadBMP("weapons/10.bmp");
	iw->t_weap_sur[11] = SDL_LoadBMP("weapons/11.bmp");
	iw->t_weap_sur[12] = SDL_LoadBMP("weapons/12.bmp");
	iw->t_weap_sur[13] = SDL_LoadBMP("weapons/13.bmp");
	iw->t_weap_sur[14] = SDL_LoadBMP("weapons/14.bmp");
	iw->t_weap_sur[15] = SDL_LoadBMP("weapons/15.bmp");
	iw->t_weap_sur[16] = SDL_LoadBMP("weapons/16.bmp");
	iw->t_weap_sur[17] = SDL_LoadBMP("weapons/17.bmp");
	iw->t_weap_sur[18] = SDL_LoadBMP("weapons/18.bmp");
}

void	get_sounds_game(t_sdl *iw)
{
	int		i;

	i = -1;
	while (++i < MUSIC_COUNT)
	{
		iw->sounds.music_rw[i] = SDL_RWFromConstMem(iw->sounds.music_pack[i], iw->sounds.music_pack_size[i]);
		iw->sounds.music[i] = Mix_LoadMUS_RW(iw->sounds.music_rw[i], 0);
	}
}

void	get_sounds(t_sdl *iw)
{
	SDL_RWops	*d;
	void		*c;
	size_t	size;

	get_music(iw, 0, "sound/background/0.mp3");
	get_music(iw, 1, "sound/background/1.mp3");
	get_music(iw, 2, "sound/background/2.mp3");
	get_music(iw, 3, "sound/background/3.mp3");
	get_music(iw, 4, "sound/background/4.mp3");
	get_music(iw, 5, "sound/background/5.mp3");

	iw->sounds.env[0] = Mix_LoadWAV("sound/environment/0.wav");
	iw->sounds.env[1] = Mix_LoadWAV("sound/environment/1.wav");
	iw->sounds.env[2] = Mix_LoadWAV("sound/environment/2.wav");
	iw->sounds.env[3] = Mix_LoadWAV("sound/environment/3.wav");
	iw->sounds.env[4] = Mix_LoadWAV("sound/environment/4.wav");
	iw->sounds.env[5] = Mix_LoadWAV("sound/environment/5.wav");
	iw->sounds.env[6] = Mix_LoadWAV("sound/environment/6.wav");
	iw->sounds.env[7] = Mix_LoadWAV("sound/environment/7.wav");
	iw->sounds.env[8] = Mix_LoadWAV("sound/environment/8.wav");
	iw->sounds.env[9] = Mix_LoadWAV("sound/environment/9.wav");
	iw->sounds.env[10] = Mix_LoadWAV("sound/environment/10.wav");
	iw->sounds.env[11] = Mix_LoadWAV("sound/environment/11.wav");
	iw->sounds.env[12] = Mix_LoadWAV("sound/environment/12.wav");
	iw->sounds.env[13] = Mix_LoadWAV("sound/environment/13.wav");
	iw->sounds.env[14] = Mix_LoadWAV("sound/environment/14.wav");
	iw->sounds.env[15] = Mix_LoadWAV("sound/environment/15.wav");
	iw->sounds.env[16] = Mix_LoadWAV("sound/environment/16.wav");
	iw->sounds.env[17] = Mix_LoadWAV("sound/environment/17.wav");
	iw->sounds.env[18] = Mix_LoadWAV("sound/environment/18.wav");
	iw->sounds.env[19] = Mix_LoadWAV("sound/environment/19.wav");
	iw->sounds.env[20] = Mix_LoadWAV("sound/environment/20.wav");
	iw->sounds.env[21] = Mix_LoadWAV("sound/environment/21.wav");
}