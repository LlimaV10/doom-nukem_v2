#include "../guardians.h"

void	death(t_sdl *iw)
{
	if (iw->v.game_mode && (iw->d.cs < 0 || iw->p.health < 1))
	{
		image_loop(iw, iw->hud.dead);
		get_birth_def(iw);
	}
}

void	set_checkpoint(t_sdl *iw, t_sprite *s)
{
	t_keyb_inp	*tmp;

	Mix_PlayChannel(2, iw->sounds.env[15], 0);
	tmp = iw->checkpoints;
	while (tmp)
	{
		if (tmp->sprite == s)
			break;
		tmp = tmp->next;
	}
	if (!tmp)
		return;
	iw->l.start_x = s->x;
	iw->l.start_y = s->y;
	iw->v.last_checkpoint = s;
	iw->v.last_to_write = tmp;
	iw->hud.saved_time = clock();
}

void	check_checkpoints(t_sdl *iw)
{
	t_sprite	*tmp;

	tmp = *iw->sprite;
	while (tmp)
	{
		if (tmp->type == 0 && tmp->t_numb == 1 &&
			tmp->draweble && tmp->plen < CHECKPOINT_DIST)
		{
			if (tmp != iw->v.last_checkpoint)
				set_checkpoint(iw, tmp);
			return;
		}
		tmp = tmp->next;
	}
}