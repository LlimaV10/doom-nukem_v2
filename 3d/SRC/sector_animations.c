/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector_animations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 17:29:52 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/01 17:30:53 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	add_sector_animation(t_sdl *iw)
{
	t_sector_animation	*tmp;

	tmp = (t_sector_animation *)malloc(sizeof(t_sector_animation));
	tmp->trigger = (t_picture *)iw->v.f_button_pointer;
	tmp->sector = get_sector_by_pointer(iw, *(iw->v.look_sector));
	tmp->speed = 1;
	tmp->dy = 0;
	tmp->status = 0;
	tmp->curr_dy = 0;
	iw->v.sector_anim = tmp;
}

void	do_sector_animation_step1(t_sdl *iw, t_sector_animation *a, int dz)
{
	int			w;
	t_picture	*p;

	if (a->type == 0)
	{
		iw->sectors[a->sector].fr.z += dz;
		iw->sectors[a->sector].cl.z += dz;
		w = iw->sectors[a->sector].sw - 1;
		while (++w < iw->sectors[a->sector].sw + iw->sectors[a->sector].nw)
		{
			p = iw->walls[w].p;
			while (p)
			{
				p->zu += dz;
				p->zd += dz;
				p = p->next;
			}
		}
	}
	else if (a->type == 1)
		iw->sectors[a->sector].fr.z += dz;
	else if (a->type == 2)
		iw->sectors[a->sector].cl.z += dz;
}

void	do_sector_animation_step(t_sdl *iw, t_sector_animation *a, int dz)
{
	int			i;

	do_sector_animation_step1(iw, a, dz);
	i = a->sector;
	if (iw->sectors[i].fr.n != 0)
		iw->sectors[i].fr.n->d = -iw->sectors[i].fr.n->a * iw->sectors[i].fr.x -
		iw->sectors[i].fr.n->b * iw->sectors[i].fr.y -
		iw->sectors[i].fr.n->c * iw->sectors[i].fr.z;
	if (iw->sectors[i].cl.n != 0)
		iw->sectors[i].cl.n->d = -iw->sectors[i].cl.n->a * iw->sectors[i].cl.x -
		iw->sectors[i].cl.n->b * iw->sectors[i].cl.y -
		iw->sectors[i].cl.n->c * iw->sectors[i].cl.z;
}

void	exit_editing_sector_animation(t_sdl *iw)
{
	if (iw->v.submenu_mode == 2 || iw->v.submenu_mode == 3)
		do_sector_animation_step(iw, iw->v.sector_anim, -iw->v.sector_anim->dy);
	free(iw->v.sector_anim);
	iw->v.sector_anim = 0;
	iw->v.submenu_mode = 0;
	draw_submenu(iw);
}

void	change_sector_animation_status(t_sdl *iw, t_picture *p)
{
	t_sector_animation	*tmp;

	tmp = iw->sector_animations;
	while (tmp)
	{
		if (tmp->trigger == p)
		{
			tmp->status = ((tmp->status == 0) ? 1 : 0);
			return ;
		}
		tmp = tmp->next;
	}
}
