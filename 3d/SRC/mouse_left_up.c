#include "../guardians.h"

void	mouse_buttonleft_up(int x, int y, t_sdl *iw)
{
	int		i;

	iw->v.left_mouse_pressed = 0;
	if (y > WINDOW_H && y < WINDOW_H + 100 && iw->v.mouse_mode == 0)
	{
		i = x / 100 + iw->v.scroll_first_tex;
		if (i < TEXTURES_COUNT)
			iw->v.tex_to_fill = i;
	}
	else if (y > WINDOW_H + 100 && y < WINDOW_H + 200
		&& iw->v.mouse_mode == 0 && *(iw->v.look_wall) != 0)
	{
		if (x < 40)
		{
			iw->v.changing_fc = ((iw->v.changing_fc) ? 0 : 1);
			SDL_FillRect(iw->sur, &iw->v.chang_fc_rect, 0x000000);
			if (iw->v.changing_fc)
				draw_text(iw, "C", 15, WINDOW_H + 135);
			else
				draw_text(iw, "F", 15, WINDOW_H + 135);
		}
		else if (x < 70)
		{
			if (y < WINDOW_H + 150)
			{
				if (iw->v.changing_fc)
					(*(iw->v.look_sector))->cl.z += 100;
				else if ((*(iw->v.look_sector))->cl.z - (*(iw->v.look_sector))->fr.z > 200)
					(*(iw->v.look_sector))->fr.z += 100;
			}
			else
			{
				if (!iw->v.changing_fc)
					(*(iw->v.look_sector))->fr.z -= 100;
				else if ((*(iw->v.look_sector))->cl.z - (*(iw->v.look_sector))->fr.z > 200)
					(*(iw->v.look_sector))->cl.z -= 100;
			}
			if (iw->v.changing_fc && (*(iw->v.look_sector))->cl.n != 0)
				(*(iw->v.look_sector))->cl.n->d = -(*(iw->v.look_sector))->cl.n->a * (*(iw->v.look_sector))->cl.x -
				(*(iw->v.look_sector))->cl.n->b * (*(iw->v.look_sector))->cl.y -
				(*(iw->v.look_sector))->cl.n->c * (*(iw->v.look_sector))->cl.z;
			else if (!iw->v.changing_fc && (*(iw->v.look_sector))->fr.n != 0)
				(*(iw->v.look_sector))->fr.n->d = -(*(iw->v.look_sector))->fr.n->a * (*(iw->v.look_sector))->fr.x -
				(*(iw->v.look_sector))->fr.n->b * (*(iw->v.look_sector))->fr.y -
				(*(iw->v.look_sector))->fr.n->c * (*(iw->v.look_sector))->fr.z;
		}
		else if (x < 140)
		{
			if (y < WINDOW_H + 150)
			{
				if (iw->v.changing_fc)
					rotate_fc(&(*(iw->v.look_sector))->cl, 0, 1);
				else
					rotate_fc(&(*(iw->v.look_sector))->fr, 0, 1);
			}
			else
			{
				if (iw->v.changing_fc)
					rotate_fc(&(*(iw->v.look_sector))->cl, 0, -1);
				else
					rotate_fc(&(*(iw->v.look_sector))->fr, 0, -1);
			}
		}
		else if (x < 210)
		{
			if (y < WINDOW_H + 150)
			{
				if (iw->v.changing_fc)
					rotate_fc(&(*(iw->v.look_sector))->cl, 1, 1);
				else
					rotate_fc(&(*(iw->v.look_sector))->fr, 1, 1);
			}
			else
			{
				if (iw->v.changing_fc)
					rotate_fc(&(*(iw->v.look_sector))->cl, 1, -1);
				else
					rotate_fc(&(*(iw->v.look_sector))->fr, 1, -1);
			}
		}
		else if (x < 230)
		{
			if (iw->v.changing_fc)
			{
				free((*(iw->v.look_sector))->cl.n);
				(*(iw->v.look_sector))->cl.n = 0;
			}
			else
			{
				free((*(iw->v.look_sector))->fr.n);
				(*(iw->v.look_sector))->fr.n = 0;
			}
		}
		else if (x > 332 && x < 430)
		{
			if (y < WINDOW_H + 135)
				draw_decor_tex_to_select(iw);
			else if (y < WINDOW_H + 170)
				draw_pickup_tex_to_select(iw);
			else
				draw_enemies_tex_to_select(iw);
		}
		else if (iw->v.submenu_mode == 1 && x > WINDOW_W - 450)
		{
			if (x < WINDOW_W - 430)
			{
				iw->v.sector_anim->type = 1;
				iw->v.submenu_mode = 2;
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 395)
			{
				iw->v.sector_anim->type = 2;
				iw->v.submenu_mode = 2;
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 355)
			{
				iw->v.sector_anim->type = 0;
				iw->v.submenu_mode = 2;
				draw_submenu(iw);
			}
			else if (x > WINDOW_W - 335 && x < WINDOW_W - 280)
				exit_editing_sector_animation(iw);
		}
		else if (iw->v.submenu_mode == 2 && x > WINDOW_W - 450)
		{
			if (x < WINDOW_W - 390)
			{
				do_sector_animation_step(iw, iw->v.sector_anim, 100);
				iw->v.sector_anim->dy += 100;
			}
			else if (x < WINDOW_W - 330)
			{
				do_sector_animation_step(iw, iw->v.sector_anim, -100);
				iw->v.sector_anim->dy -= 100;
			}
			else if (x < WINDOW_W - 280)
			{
				iw->v.submenu_mode = 3;
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 220)
				exit_editing_sector_animation(iw);
		}
		else if (iw->v.submenu_mode == 3 && x > WINDOW_W - 340)
		{
			if (x < WINDOW_W - 320)
			{
				iw->v.sector_anim->speed += ((iw->v.sector_anim->speed < 9) ? 1 : 0);
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 295)
			{
				iw->v.sector_anim->speed -= ((iw->v.sector_anim->speed > 1) ? 1 : 0);
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 245)
			{
				do_sector_animation_step(iw, iw->v.sector_anim, -iw->v.sector_anim->dy);
				iw->v.sector_anim->next = iw->sector_animations;
				iw->v.submenu_mode = 0;
				draw_submenu(iw);
				iw->v.sector_anim->prev_clock = clock();
				iw->sector_animations = iw->v.sector_anim;
				iw->v.sector_anim = 0;
			}
			else if (x < WINDOW_W - 200)
				exit_editing_sector_animation(iw);
		}
		else if (iw->v.submenu_mode == 4 && x > WINDOW_W - 450)
		{
			if (x < WINDOW_W - 375)
			{
				iw->v.f_button_mode = 2;
				iw->v.submenu_mode = 0;
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 300)
			{
				iw->v.f_button_mode = 3;
				iw->v.submenu_mode = 0;
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 240)
			{
				iw->v.submenu_mode = 0;
				iw->v.f_button_pointer = 0;
				draw_submenu(iw);
			}
		}
		else if (iw->v.submenu_mode == 5 && x > WINDOW_W - 450)
		{
			if (x < WINDOW_W - 380)
			{
				iw->v.wall_anim->moving_type = 0;
				iw->v.submenu_mode = 6;
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 305)
			{
				iw->v.wall_anim->moving_type = 1;
				iw->v.submenu_mode = 6;
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 225)
			{
				iw->v.wall_anim->moving_type = 2;
				iw->v.submenu_mode = 6;
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 165)
				exit_editing_wall_animation(iw);
		}
		else if (iw->v.submenu_mode == 6 && x > WINDOW_W - 450)
		{
			if (x < WINDOW_W - 380)
			{
				iw->v.wall_anim->priority = 0;
				iw->v.submenu_mode = 7;
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 300)
			{
				iw->v.wall_anim->priority = 1;
				iw->v.submenu_mode = 7;
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 225)
				exit_editing_wall_animation(iw);
		}
		else if (iw->v.submenu_mode == 7 && x > WINDOW_W - 450)
		{
			if (x < WINDOW_W - 390)
			{
				iw->v.wall_anim->dx += 100;
				do_wall_animation_step_dx(iw, iw->v.wall_anim, 100);
			}
			else if (x < WINDOW_W - 330)
			{
				iw->v.wall_anim->dx -= 100;
				do_wall_animation_step_dx(iw, iw->v.wall_anim, -100);
			}
			else if (x < WINDOW_W - 280)
			{
				iw->v.submenu_mode = 8;
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 220)
				exit_editing_wall_animation(iw);
		}
		else if (iw->v.submenu_mode == 8 && x > WINDOW_W - 450)
		{
			if (x < WINDOW_W - 390)
			{
				iw->v.wall_anim->dy += 100;
				do_wall_animation_step_dy(iw, iw->v.wall_anim, 100);
			}
			else if (x < WINDOW_W - 330)
			{
				iw->v.wall_anim->dy -= 100;
				do_wall_animation_step_dy(iw, iw->v.wall_anim, -100);
			}
			else if (x < WINDOW_W - 280)
			{
				iw->v.submenu_mode = 9;
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 220)
				exit_editing_wall_animation(iw);
		}
		else if (iw->v.submenu_mode == 9 && x > WINDOW_W - 340)
		{
			if (x < WINDOW_W - 320)
			{
				iw->v.wall_anim->speed += ((iw->v.wall_anim->speed < 9) ? 1 : 0);
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 295)
			{
				iw->v.wall_anim->speed -= ((iw->v.wall_anim->speed > 1) ? 1 : 0);
				draw_submenu(iw);
			}
			else if (x < WINDOW_W - 245)
			{
				do_wall_animation_step_dx(iw, iw->v.wall_anim, -iw->v.wall_anim->dx);
				do_wall_animation_step_dy(iw, iw->v.wall_anim, -iw->v.wall_anim->dy);
				iw->v.wall_anim->next = iw->wall_animations;
				iw->v.submenu_mode = 0;
				draw_submenu(iw);
				iw->v.wall_anim->prev_clock = clock();
				iw->wall_animations = iw->v.wall_anim;
				iw->v.wall_anim = 0;
			}
			else if (x < WINDOW_W - 200)
				exit_editing_wall_animation(iw);
		}
	}
	else if (y > WINDOW_H + 200 && y < WINDOW_H + 300 && iw->v.mouse_mode == 0)
	{
		if (iw->v.sprites_select_mode == 0)
		{
			i = x / 100 + iw->v.scroll_decor_sprites;
			if (i < DECOR_TEXTURES_COUNT)
			{
				iw->v.selected_sprite_type = 0;
				iw->v.selected_sprite = i;
			}
		}
		else if (iw->v.sprites_select_mode == 1)
		{
			i = x / 100 + iw->v.scroll_pickup_sprites;
			if (i < PICK_UP_TEXTURES_COUNT)
			{
				iw->v.selected_sprite_type = 1;
				iw->v.selected_sprite = i;
			}
		}
		else if (iw->v.sprites_select_mode == 2)
		{
			i = x / 100;
			if (i < COUNT_ENEMIES)
			{
				iw->v.selected_sprite_type = 2;
				iw->v.selected_sprite = i;
			}
		}
	}
	else if (iw->v.sprite_editing && iw->v.mouse_mode == 1 && iw->v.look_sprite != 0)
	{
		if (iw->v.selected_sprite_type == 0)
		{
			iw->v.look_sprite->type = 0;
			iw->v.look_sprite->t_numb = iw->v.selected_sprite;
			iw->v.look_sprite->t = iw->t_decor[iw->v.selected_sprite];
			iw->v.look_sprite->t_kernel = &iw->k.m_t_decor[iw->v.selected_sprite];
			if (iw->v.selected_sprite == 1)
				add_checkpoint(iw, iw->v.look_sprite);
		}
		else if (iw->v.selected_sprite_type == 1)
		{
			iw->v.look_sprite->type = 1;
			iw->v.look_sprite->t_numb = iw->v.selected_sprite;
			iw->v.look_sprite->t = iw->t_pickup[iw->v.selected_sprite];
			iw->v.look_sprite->t_kernel = &iw->k.m_t_pickup[iw->v.selected_sprite];
		}
		else if (iw->v.selected_sprite_type == 2)
		{
			iw->v.look_sprite->type = 2;
			iw->v.look_sprite->e.enemy_numb = iw->v.selected_sprite;
			iw->v.look_sprite->e.status = 0;
			if (iw->v.selected_sprite == 0)
			{
				iw->v.look_sprite->e.health = ENEMY_HEALTH0;
				iw->v.look_sprite->t_numb = 0;
				iw->v.look_sprite->t = iw->t_enemies[0];
				iw->v.look_sprite->t_kernel = &iw->k.m_t_enemies[0];
			}
			else if (iw->v.selected_sprite == 1)
			{
				iw->v.look_sprite->e.health = ENEMY_HEALTH1;
				iw->v.look_sprite->t_numb = 8;
				iw->v.look_sprite->t = iw->t_enemies[8];
				iw->v.look_sprite->t_kernel = &iw->k.m_t_enemies[8];
			}
			else if (iw->v.selected_sprite == 2)
			{
				iw->v.look_sprite->e.health = ENEMY_HEALTH2;
				iw->v.look_sprite->t_numb = 20;
				iw->v.look_sprite->t = iw->t_enemies[20];
				iw->v.look_sprite->t_kernel = &iw->k.m_t_enemies[20];
			}
		}
	}
	else if (iw->v.mouse_mode == 1 && *(iw->v.look_picture) != 0
		&& *(iw->v.look_wall) != 0 && !iw->v.game_mode)
	{
		(*(iw->v.look_picture))->t = iw->v.tex_to_fill;
		calculate_picture(iw, *(iw->v.look_wall), *(iw->v.look_picture));
	}
	else if (iw->v.mouse_mode == 1 && *(iw->v.look_wall) != 0 && !iw->v.game_mode)
	{
		if (iw->v.look_portal == 0 || iw->v.look_portal->glass < 0)
			(*(iw->v.look_wall))->t = iw->v.tex_to_fill;
		else
			iw->v.look_portal->glass = -1;
	}
}