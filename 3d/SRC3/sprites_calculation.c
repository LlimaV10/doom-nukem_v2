#include "../guardians.h"

int		find_point(t_save_wall_pairs *tmp, t_sprite *tmp1)
{
	int		i;
	int ax = tmp->right->p.x - tmp->left->p.x;
	int ay = tmp->right->p.y - tmp->left->p.y;
	int bx = tmp1->x - tmp->left->p.x;
	int by = tmp1->y - tmp->left->p.y;
	i = ax * by - ay * bx;
	if (i > 0)
		return (0);
	else
		return (1);
}

void	calculate_sprites_once2(t_sdl *iw, t_sprite *tmp1)
{
	float lang;
	float rang;

	lang = get_vectors_angle(iw->d.left_point.x -
		(float)iw->p.x, iw->d.left_point.y - (float)iw->p.y,
		(float)(tmp1->x - iw->p.x), (float)(tmp1->y - iw->p.y));
	rang = get_vectors_angle(iw->d.right_point.x -
		(float)iw->p.x, iw->d.right_point.y - (float)iw->p.y,
		(float)(tmp1->x - iw->p.x), (float)(tmp1->y - iw->p.y));
	tmp1->x_s = (int)(lang * (float)WINDOW_W / (2.0f * iw->v.angle));
	if (rang > 2.0f * iw->v.angle)
		tmp1->x_s = -tmp1->x_s;
	tmp1->plen = sqrtf(powf((float)(iw->p.x - tmp1->x), 2.0f)
		+ powf((float)(iw->p.y - tmp1->y), 2.0f)) + 1.0f;
	tmp1->pplen = fabsf(iw->d.screen.a * (float)tmp1->x +
		iw->d.screen.b * (float)tmp1->y + iw->d.screen.c) /
		sqrtf(iw->d.screen.a * iw->d.screen.a +
			iw->d.screen.b * iw->d.screen.b) + 1.0f;
	if (tmp1->pplen / tmp1->plen >= 0.5f)
		tmp1->spritewidth = (int)(fabsf((float)(WINDOW_W * tmp1->t->w)
			/ tmp1->pplen) * tmp1->scale);
	else
		tmp1->spritewidth = (int)(fabsf((float)(WINDOW_W * tmp1->t->w)
			/ tmp1->plen) * tmp1->scale);
	tmp1->sx = tmp1->x_s - tmp1->spritewidth;
}

void	calculate_sprites_once(t_sdl *iw)
{
	t_sprite	*tmp1;

	tmp1 = *iw->sprite;
	while (tmp1 != 0)
	{
		if (!iw->sectors[tmp1->num_sec].visited || tmp1->draweble == 1)
		{
			tmp1 = tmp1->next;
			continue;
		}
		calculate_sprites_once2(iw, tmp1);
		tmp1->ex = tmp1->x_s + tmp1->spritewidth;
		if (!(tmp1->sx > iw->d.screen_right || tmp1->ex < iw->d.screen_left))
			tmp1->draweble = 1;
		tmp1 = tmp1->next;
	}
}

void	get_sprites_top_bottom1(t_sdl *iw, t_save_wall_pairs *tmp, t_sprite *tmp1)
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
		return;
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
		return;
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