/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_from_files2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 16:48:36 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 16:48:37 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	read_textures3(t_sdl *iw)
{
	iw->t_sur[24] = SDL_LoadBMP("textures/24.bmp");
	iw->tsz[24] = 1.0f;
	iw->bag.button_sur[0] =
		SDL_LoadBMP("interface_textures/backpack/frame.bmp");
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

void	read_textures2_1(t_sdl *iw)
{
	iw->t_sur[25] = SDL_LoadBMP("textures/25.bmp");
	iw->tsz[25] = 2.5f;
	iw->t_sur[26] = SDL_LoadBMP("textures/26.bmp");
	iw->tsz[26] = 1.0f;
	iw->t_sur[27] = SDL_LoadBMP("textures/27.bmp");
	iw->tsz[27] = 1.0f;
	iw->t_sur[28] = SDL_LoadBMP("textures/28.bmp");
	iw->tsz[28] = 1.0f;
	iw->t_sur[29] = SDL_LoadBMP("textures/29.bmp");
	iw->tsz[29] = 1.0f;
	iw->t_sur[30] = SDL_LoadBMP("textures/30.bmp");
	iw->tsz[30] = 1.0f;
	iw->t_sur[31] = SDL_LoadBMP("textures/31.bmp");
	iw->tsz[31] = 1.0f;
	iw->t_sur[32] = SDL_LoadBMP("textures/32.bmp");
	iw->tsz[32] = 1.0f;
	iw->t_sur[33] = SDL_LoadBMP("textures/33.bmp");
	iw->tsz[33] = 1.0f;
	iw->t_sur[34] = SDL_LoadBMP("textures/34.bmp");
	iw->tsz[34] = 1.0f;
	iw->t_sur[35] = SDL_LoadBMP("textures/35.bmp");
	iw->tsz[35] = 2.5f;
	iw->t_sur[36] = SDL_LoadBMP("textures/36.bmp");
	iw->tsz[36] = 2.5f;
	iw->t_sur[37] = SDL_LoadBMP("textures/37.bmp");
	iw->tsz[37] = 1.0f;
	iw->t_sur[38] = SDL_LoadBMP("textures/38.bmp");
	iw->tsz[38] = 5.0f;
	iw->t_sur[39] = SDL_LoadBMP("textures/39.bmp");
	iw->tsz[39] = 1.0f;
	iw->t_sur[40] = SDL_LoadBMP("textures/40.bmp");
	iw->tsz[40] = 1.0f;
	iw->t_sur[41] = SDL_LoadBMP("textures/41.bmp");
	iw->tsz[41] = 0.666666f;
	iw->t_sur[42] = SDL_LoadBMP("textures/42.bmp");
	iw->tsz[42] = 1.0f;
	iw->t_sur[43] = SDL_LoadBMP("textures/43.bmp");
	iw->tsz[43] = 1.0f;
	iw->t_sur[44] = SDL_LoadBMP("textures/44.bmp");
	iw->tsz[44] = 1.0f;
	iw->t_sur[45] = SDL_LoadBMP("textures/45.bmp");
	iw->tsz[45] = 2.0f;
	iw->t_sur[46] = SDL_LoadBMP("textures/46.bmp");
	iw->tsz[46] = 2.0f;
	iw->t_sur[47] = SDL_LoadBMP("textures/47.bmp");
	iw->tsz[47] = 1.0f;
}

void	read_textures2(t_sdl *iw)
{
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
	read_textures2_1(iw);
	read_textures3(iw);
}

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
	read_textures2(iw);
}

void	read_sprites_textures2(t_sdl *iw)
{
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

void	read_sprites_textures(t_sdl *iw)
{
	iw->t_decor_sur[0] = SDL_LoadBMP("sprites/decorations/0.bmp");
	iw->t_decor_sur[1] = SDL_LoadBMP("sprites/decorations/1.bmp");
	iw->t_decor_sur[2] = SDL_LoadBMP("sprites/decorations/2.bmp");
	iw->t_decor_sur[3] = SDL_LoadBMP("sprites/decorations/3.bmp");
	iw->t_decor_sur[4] = SDL_LoadBMP("sprites/decorations/4.bmp");
	iw->t_decor_sur[5] = SDL_LoadBMP("sprites/decorations/5.bmp");
	iw->t_decor_sur[6] = SDL_LoadBMP("sprites/decorations/6.bmp");
	iw->t_decor_sur[7] = SDL_LoadBMP("sprites/decorations/7.bmp");
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
	read_sprites_textures2(iw);
}
