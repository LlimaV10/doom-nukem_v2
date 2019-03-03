#include "../guardians.h"

void	damaging_enemy(t_sdl *iw, int damage, int max_distance)
{
	if (iw->v.look_portal != 0 && iw->v.look_portal->glass != -1
		&& iw->guns.gun_in_hands != 0)
		iw->v.look_portal->glass = -1;
	if (iw->v.look_sprite == 0 || iw->v.look_sprite->type != 2
		|| iw->v.look_sprite->e.status >= 4)
		return;
	/*if ((int)sqrtf(powf(iw->p.x - iw->v.look_sprite->x, 2.0f) +
			powf(iw->p.y - iw->v.look_sprite->y, 2.0f) +
			powf(iw->p.z - iw->v.look_sprite->z, 2.0f)) < max_distance)*/
	if (iw->v.look_sprite->plen < max_distance)
		iw->v.look_sprite->e.health -= damage;
	else
		iw->hud.miss_time = clock();
}

void	attack2(t_sdl *iw)
{
	if (iw->guns.gun_in_hands == 1 && iw->guns.status == 0
		&& clock() - iw->guns.prev_update_time > CLKS_P_S / 15)
	{
		damaging_enemy(iw, 3, 5000);
		Mix_PlayChannel(3, iw->sounds.env[6], 0);
		iw->guns.status = 1;
		iw->guns.t = 1;
		iw->guns.prev_update_time = clock();
		iw->guns.bullets[1] -= 1;
		iw->hud.shell = 100 * iw->guns.bullets[1] / iw->guns.max_bullets[1];
	}
	else if (iw->guns.gun_in_hands == 2 && iw->guns.status == 0 && clock()
		- iw->guns.prev_update_time > CLKS_P_S / 5)
	{
		iw->guns.status = 1;
		iw->guns.t = 8;
		iw->guns.prev_update_time = clock();
		iw->guns.bullets[2] -= 1;
		iw->hud.shell = 100 * iw->guns.bullets[2] / iw->guns.max_bullets[2];
	}
}

void	attack(t_sdl *iw)
{
	if (iw->guns.gun_in_hands == 0 && iw->guns.status == 0 &&
		clock() - iw->guns.prev_update_time > CLKS_P_S / 15)
	{
		damaging_enemy(iw, 3, 1000);
		iw->guns.status = 1;
		iw->guns.t = 18;
		iw->guns.prev_update_time = clock();
		iw->guns.bullets[0] -= 1;
		iw->hud.shell = 100 * iw->guns.bullets[0] / iw->guns.max_bullets[0];
	}
	else
		attack2(iw);
}

void	reload_gun1(t_sdl *iw)
{
	if (iw->guns.bullets_in_stock[1] < iw->guns.max_bullets[1])
	{
		iw->guns.gun_in_hands = 0;
		iw->guns.status = 3;
	}
	else if (iw->guns.t == 0)
	{
		Mix_PlayChannel(3, iw->sounds.env[7], 0);
		iw->guns.t = 2;
	}
	else if (iw->guns.t < 6)
		iw->guns.t++;
	else
	{
		iw->guns.bullets_in_stock[1] -= iw->guns.max_bullets[1]
			- iw->guns.bullets[1];;
		iw->guns.bullets[iw->guns.gun_in_hands] =
			iw->guns.max_bullets[iw->guns.gun_in_hands];
		iw->hud.shell = 100;
		iw->guns.status = 0;
		iw->guns.t = 0;
		iw->v.weapon_change_y_hide = iw->guns.t_rect[iw->guns.t].h / 2;
	}
}

void	reload_gun2(t_sdl *iw)
{
	if (iw->guns.bullets_in_stock[2] < iw->guns.max_bullets[2])
	{
		iw->guns.gun_in_hands = 0;
		iw->guns.status = 3;
	}
	else if (iw->guns.t == 7 || iw->guns.t == 8 || iw->guns.t == 9)
	{
		Mix_PlayChannel(3, iw->sounds.env[8], 0);
		iw->guns.t = 10;
	}
	else if (iw->guns.t < 16)
		iw->guns.t++;
	else
	{
		iw->guns.bullets_in_stock[2] -= iw->guns.max_bullets[2]
			- iw->guns.bullets[2];
		iw->guns.bullets[iw->guns.gun_in_hands] =
			iw->guns.max_bullets[iw->guns.gun_in_hands];
		iw->hud.shell = 100;
		iw->guns.status = 0;
		iw->guns.t = 7;
		iw->v.weapon_change_y_hide = iw->guns.t_rect[iw->guns.t].h / 2;
	}
}

void	reload_gun(t_sdl *iw)
{
	if (iw->guns.gun_in_hands == 0)
	{
		iw->guns.bullets[iw->guns.gun_in_hands] =
			iw->guns.max_bullets[iw->guns.gun_in_hands];
		iw->hud.shell = 100;
		iw->guns.status = 0;
	}
	else if (iw->guns.gun_in_hands == 1)
		reload_gun1(iw);
	else if (iw->guns.gun_in_hands == 2)
		reload_gun2(iw);
	iw->guns.prev_update_time = clock();
}

void	guns_loop2(t_sdl *iw)
{
	if (iw->guns.t == 8)
	{
		damaging_enemy(iw, 2, 10000);
		Mix_PlayChannel(3, iw->sounds.env[5], 0);
		iw->guns.t = 9;
	}
	else
	{
		if (!iw->v.left_mouse_pressed ||
			iw->guns.bullets[iw->guns.gun_in_hands] <= 0)
		{
			iw->guns.status = 0;
			iw->guns.t = 7;
		}
		else
		{
			iw->guns.t = 8;
			iw->guns.bullets[iw->guns.gun_in_hands]--;
			iw->hud.shell = 100 * iw->guns.bullets[iw->guns.gun_in_hands] /
				iw->guns.max_bullets[iw->guns.gun_in_hands];
		}
		if (iw->guns.bullets[iw->guns.gun_in_hands] <= 0)
			iw->guns.status = 2;
	}
	iw->guns.prev_update_time = clock();
}

void	guns_loop(t_sdl *iw)
{
	if (iw->guns.status == 1 && iw->guns.gun_in_hands == 0
		&& clock() - iw->guns.prev_update_time > CLKS_P_S / 15)
	{
		iw->guns.status = 0;
		iw->guns.t = 17;
		if (iw->guns.bullets[iw->guns.gun_in_hands] <= 0)
			iw->guns.status = 2;
		iw->guns.prev_update_time = clock();
	}
	else if (iw->guns.status == 1 && iw->guns.gun_in_hands == 1
		&& clock() - iw->guns.prev_update_time > CLKS_P_S / 5)
	{
		iw->guns.status = 0;
		iw->guns.t = 0;
		if (iw->guns.bullets[iw->guns.gun_in_hands] <= 0)
			iw->guns.status = 2;
		iw->guns.prev_update_time = clock();
	}
	else if (iw->guns.status == 1 && iw->guns.gun_in_hands == 2
		&& clock() - iw->guns.prev_update_time > CLKS_P_S / 10)
		guns_loop2(iw);
}

void	guns_movements2(t_sdl *iw, int t)
{
	if (iw->v.fps != 0)
		t = 150 / iw->v.fps;
	else
		t = 10;
	iw->v.weapon_change_x += iw->v.sprint *
		((iw->v.weapon_change_xdir > 0) ? t : -t);
	if (abs(iw->v.weapon_change_x) > WEAPONS_MOVING_CHANGE_VALUE)
	{
		iw->v.weapon_change_xdir *= -1;
		iw->v.weapon_change_x = ((iw->v.weapon_change_x > 0) ?
			WEAPONS_MOVING_CHANGE_VALUE : -WEAPONS_MOVING_CHANGE_VALUE);
	}
	iw->v.weapon_change_y += iw->v.sprint *
		((iw->v.weapon_change_ydir > 0) ? t : -t);
	if (iw->v.weapon_change_y > WEAPONS_MOVING_CHANGE_VALUE
		|| iw->v.weapon_change_y < 10)
		iw->v.weapon_change_ydir *= -1;
	if (iw->v.weapon_change_y > WEAPONS_MOVING_CHANGE_VALUE)
		iw->v.weapon_change_y = WEAPONS_MOVING_CHANGE_VALUE;
	if (iw->v.weapon_change_y < 10)
		iw->v.weapon_change_y = 10;
}

void	guns_movements(t_sdl *iw)
{
	if (iw->v.game_mode &&
		(iw->v.front != 1 || iw->v.back != 1 || iw->v.left != 1 || iw->v.right != 1))
		guns_movements2(iw, 0);
	if (iw->guns.status == 3)
	{
		iw->v.weapon_change_y_hide += 500 / iw->v.fps;
		if (iw->v.weapon_change_y_hide >= iw->guns.t_rect[iw->guns.t].h)
		{
			if (iw->guns.gun_in_hands == 0)
				iw->guns.t = 17;
			else if (iw->guns.gun_in_hands == 1)
				iw->guns.t = 0;
			else if (iw->guns.gun_in_hands == 2)
				iw->guns.t = 7;
			iw->guns.status = 0;
		}
	}
	else if (iw->v.weapon_change_y_hide > 0)
	{
		iw->v.weapon_change_y_hide -= 1000 / iw->v.fps;
		if (iw->v.weapon_change_y_hide < 0)
			iw->v.weapon_change_y_hide = 0;
	}
}