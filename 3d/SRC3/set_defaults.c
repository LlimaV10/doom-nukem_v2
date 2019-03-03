#include "../guardians.h"

void	get_birth_def(t_sdl *iw)
{
	iw->p.x = iw->l.start_x;
	iw->p.y = iw->l.start_y;
	iw->p.z = -1000000;
	iw->p.introt = iw->l.start_rot;
	iw->p.rot = (float)iw->p.introt * G1;
	iw->p.rotup = 0;
	iw->v.front = 1;
	iw->v.back = 1;
	iw->v.left = 1;
	iw->v.right = 1;
	iw->v.rot_left = 1;
	iw->v.rot_right = 1;
	iw->v.rot_up = 1;
	iw->v.rot_down = 1;
	iw->v.fall = 1;
	iw->v.jump_time = 1;
	iw->v.jump = 0;
	iw->v.fly_mode = 0;
	iw->v.fly_down = 1;
	iw->v.fly_up = 1;
	iw->p.health = 100;
	iw->v.jetpack = 1;
	iw->v.sprint = 1;
	iw->v.left_mouse_pressed = 0;
	iw->v.crouch = 0;
	iw->v.crouch_pressed = 0;
}

void	get_def_new(t_sdl *iw)
{
	iw->v.ls = 0;
	iw->v.angle = (float)WINDOW_W / (float)WINDOW_H * 22.0f * G1;// 0.698132f;

	iw->loop_update_time = clock();
	iw->v.fps = 0;
	iw->winrect.x = 0;
	iw->winrect.y = 0;
	iw->winrect.w = WINDOW_W;
	iw->winrect.h = WINDOW_H;
	iw->v.tex_to_fill = 0;
	iw->v.scroll_first_tex = 0;
	iw->v.mouse_mode = 1;
	iw->v.scroll_tex_rect.h = 100;
	iw->v.scroll_tex_rect.w = WINDOW_W;
	iw->v.scroll_tex_rect.x = 0;
	iw->v.scroll_tex_rect.y = WINDOW_H;

	*(iw->v.look_wall) = 0;

	*(iw->v.look_sector) = 0;

	*(iw->v.look_picture) = 0;
	iw->v.look_sprite = 0;
	iw->v.look_portal = 0;
	iw->v.changing_fc = 0;
	iw->v.chang_fc_rect.h = 100;
	iw->v.chang_fc_rect.w = 40;
	iw->v.chang_fc_rect.x = 0;
	iw->v.chang_fc_rect.y = WINDOW_H + 100;
	iw->v.picture_changing = 0;
	iw->visited_sectors = 0;


	// *iw->sprite = 0;

	iw->v.f_button_mode = 0;

	iw->v.submenu_mode = 0;
	iw->v.sector_anim = 0;


	iw->v.wall_anim = 0;

	*(iw->vw_save) = 0;

	iw->v.sprites_select_mode = 0;
	iw->v.scroll_decor_sprites = 0;
	iw->v.scroll_pickup_sprites = 0;
	iw->v.selected_sprite_type = 0;
	iw->v.selected_sprite = 0;
	iw->v.sprite_editing = 0;



	iw->hud.rad = 100;

	iw->hud.shell = 100;

	iw->v.weapon_change_x = 0;
	iw->v.weapon_change_y = 20;
	iw->v.weapon_change_xdir = 1;
	iw->v.weapon_change_ydir = 1;
	iw->v.weapon_change_y_hide = 0;

	iw->bag.bag = 0;
	iw->bag.count_items = 0;
	iw->bag.selected_item = 0;

	iw->map.back = 0;
	iw->map.pl_rect.x = WINDOW_W / 2 - WINDOW_W / 100;
	iw->map.pl_rect.y = WINDOW_H / 2 + WINDOW_H / 34;
	iw->map.pl_rect.w = 20;
	iw->map.pl_rect.h = 20;

	iw->menu.count = 1;


	iw->hud.miss_time = 1;
	iw->v.have_clocks = 0;
	iw->v.last_checkpoint = 0;
	iw->hud.saved_time = 1;
	iw->v.last_to_write = 0;

	iw->v.next_music = 0;
	Mix_VolumeMusic(15);
	int		i;
	i = -1;
	while (++i < ENV_COUNT)
		Mix_Volume(i, 60);
	Mix_Volume(4, 15);
	Mix_Volume(0, 120);
	Mix_VolumeChunk(iw->sounds.env[0], 64);
	Mix_VolumeChunk(iw->sounds.env[1], 64);
	get_birth_def(iw);
}