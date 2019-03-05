/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_pairs_sort.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 13:06:40 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 13:06:42 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

void	new_sort_pairs(t_sdl *iw)
{
	t_save_wall_pairs	start;
	t_save_wall_pairs	*tmp;
	t_save_wall_pairs	*tmp2;
	t_save_wall_pairs	*after;
	int					i;

	start.next = iw->d.vwp;
	tmp = &start;
	i = 0;
	while (tmp->next != 0)
	{
		i++;
		if (i > 1000)
			break;
		if ((after = get_closest_between_pair(tmp->next)) != 0)
		{
			tmp2 = tmp->next;
			tmp->next = tmp->next->next;
			tmp2->next = after->next;
			after->next = tmp2;
		}
		else
			tmp = tmp->next;
	}
	iw->d.vwp = start.next;
}

void	add_pair(t_sdl *iw, t_save_wall *left, t_save_wall *right)
{
	t_save_wall_pairs	*tmp;
	t_save_wall_pairs	*tmp2;

	tmp = (t_save_wall_pairs *)malloc(sizeof(t_save_wall_pairs));
	tmp->left = left;
	tmp->right = right;
	tmp->next = 0;
	if (iw->d.vwp == 0)
		iw->d.vwp = tmp;
	else
	{
		tmp2 = iw->d.vwp;
		while (tmp2->next != 0)
			tmp2 = tmp2->next;
		tmp2->next = tmp;
	}
}

t_save_wall_pairs	*get_closest_between_pair(t_save_wall_pairs	*pair)
{
	t_save_wall_pairs	*save;
	t_save_wall_pairs	*tmp;

	save = 0;
	tmp = pair->next;
	while (tmp != 0)
	{
		if (tmp != pair)
			if ((tmp->left->x >= pair->left->x && tmp->left->x < pair->right->x &&
				((pair->left->wall->x - pair->right->wall->x) *
				(tmp->left->p.y - pair->right->wall->y) -
				(pair->left->wall->y - pair->right->wall->y) *
					(tmp->left->p.x - pair->right->wall->x) > 0)) ||
					(tmp->right->x > pair->left->x && tmp->right->x <= pair->right->x &&
				((pair->left->wall->x - pair->right->wall->x) *
						(tmp->right->p.y - pair->right->wall->y) -
						(pair->left->wall->y - pair->right->wall->y) *
					(tmp->right->p.x - pair->right->wall->x) > 0)))
				save = tmp;
		tmp = tmp->next;
	}
	return (save);
}

void	free_pairs(t_sdl *iw)
{
	t_save_wall_pairs	*tmp;
	t_save_wall_pairs	*tmp2;

	tmp = iw->d.vwp;
	while (tmp != 0)
	{
		tmp2 = tmp;
		tmp = tmp->next;
		free(tmp2);
	}
	iw->d.vwp = 0;
}
