/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites_calculation.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 13:06:22 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 13:06:23 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	get_sprites_top_bottom1(t_sdl *iw, t_save_wall_pairs *tmp,
	t_sprite *tmp1)
{
	int		j;
	int		jend;

	if ((tmp1->sx >= tmp->left->x && tmp1->sx <= tmp->right->x)
		|| (tmp1->ex >= tmp->left->x && tmp1->ex <= tmp->right->x)
		|| (tmp1->sx <= tmp->left->x && tmp1->ex >= tmp->right->x))
		if (find_point(tmp, tmp1) == 1)
		{
			j = ft_max(tmp1->sx, ft_max(tmp->left->x, iw->d.screen_left)) - 1;
			jend = ft_min(tmp1->ex, ft_min(tmp->right->x, iw->d.screen_right));
			while (++j <= jend)
				if (tmp1->top[j] == -1)
				{
					tmp1->top[j] = iw->d.top[j];
					tmp1->bottom[j] = iw->d.bottom[j];
				}
		}
}

void	get_sprites_top_bottom(t_sdl *iw, t_save_wall_pairs *tmp)
{
	t_sprite	*tmp1;

	if (iw->v.kernel)
	{
		clEnqueueReadBuffer(iw->k.command_queue, iw->k.m_top, CL_TRUE, 0,
			(WINDOW_W + 1) * sizeof(int), iw->d.top, 0, NULL, NULL);
		clEnqueueReadBuffer(iw->k.command_queue, iw->k.m_bottom, CL_TRUE, 0,
			(WINDOW_W + 1) * sizeof(int), iw->d.bottom, 0, NULL, NULL);
	}
	tmp1 = *iw->sprite;
	while (tmp1 != 0)
	{
		if (!iw->sectors[tmp1->num_sec].visited ||
			!tmp1->draweble || iw->d.cs != tmp1->num_sec)
		{
			tmp1 = tmp1->next;
			continue;
		}
		get_sprites_top_bottom1(iw, tmp, tmp1);
		tmp1 = tmp1->next;
	}
}

void	switch_nexts_sprites(t_sprite *s1, t_sprite *s2)
{
	t_sprite	*tmp;
	t_sprite	*tmp2;

	if (!s1 || !s2)
		return ;
	if (s2->next == s1)
	{
		s2->next = s2->next->next;
		s1->next = s1->next->next;
		s2->next->next = s1;
	}
	else
	{
		tmp = s1->next->next;
		s1->next->next = s2->next->next;
		tmp2 = s2->next;
		s2->next = s1->next;
		s1->next = tmp2;
		tmp2->next = tmp;
	}
}

void	sort_sprites2(t_sdl *iw, t_sprite *tmp1)
{
	t_sprite	*max;
	t_sprite	*tmp2;

	max = 0;
	tmp2 = tmp1->next;
	while (tmp2->next != 0)
	{
		if (iw->sectors[tmp2->next->num_sec].visited && tmp2->next->draweble)
			if (tmp2->next->plen > tmp1->next->plen &&
				(max == 0 || max->next->plen < tmp2->next->plen))
				max = tmp2;
		tmp2 = tmp2->next;
	}
	switch_nexts_sprites(max, tmp1);
}

void	sort_sprites(t_sdl *iw)
{
	t_sprite	head;
	t_sprite	*tmp1;

	if (!*iw->sprite)
		return ;
	head.next = *iw->sprite;
	tmp1 = &head;
	while (tmp1->next->next != 0)
	{
		if (!iw->sectors[tmp1->next->num_sec].visited || !tmp1->next->draweble)
		{
			tmp1 = tmp1->next;
			continue;
		}
		sort_sprites2(iw, tmp1);
		tmp1 = tmp1->next;
	}
	*iw->sprite = head.next;
}
