/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_floor_ceil_z.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 13:04:28 by dbolilyi          #+#    #+#             */
/*   Updated: 2019/03/05 13:04:29 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../guardians.h"

int		get_floor_z(t_sdl *iw, int x, int y)
{
	if (iw->sectors[iw->d.cs].fr.n == 0)
		return (iw->sectors[iw->d.cs].fr.z);
	else
		return ((iw->sectors[iw->d.cs].fr.n->a * x +
			iw->sectors[iw->d.cs].fr.n->b * y +
			iw->sectors[iw->d.cs].fr.n->d) /
			(-iw->sectors[iw->d.cs].fr.n->c));
}

int		get_ceil_z(t_sdl *iw, int x, int y)
{
	if (iw->sectors[iw->d.cs].cl.n == 0)
		return (iw->sectors[iw->d.cs].cl.z);
	else
		return ((iw->sectors[iw->d.cs].cl.n->a * x + iw->sectors[iw->d.cs].cl.
		n->b * y + iw->sectors[iw->d.cs].cl.n->d) /
		(-iw->sectors[iw->d.cs].cl.n->c));
}

int		get_floor_z_sec(t_sdl *iw, int x, int y, int sector)
{
	if (iw->sectors[sector].fr.n == 0)
		return (iw->sectors[sector].fr.z);
	else
		return ((iw->sectors[sector].fr.n->a * x + iw->sectors[sector].fr.n->b *
			y + iw->sectors[sector].fr.n->d) / (-iw->sectors[sector].fr.n->c));
}

int		get_ceil_z_sec(t_sdl *iw, int x, int y, int sector)
{
	if (iw->sectors[sector].cl.n == 0)
		return (iw->sectors[sector].cl.z);
	else
		return ((iw->sectors[sector].cl.n->a * x + iw->sectors[sector].cl.n->b *
			y + iw->sectors[sector].cl.n->d) / (-iw->sectors[sector].cl.n->c));
}

void	rotate_fc(t_sector_fc *fc, int xy, int pl)
{
	if (fc->n == 0)
	{
		fc->n = (t_vector *)malloc(sizeof(t_vector));
		fc->n->c = INCLINED_FC_Z;
		fc->n->a = 0;
		fc->n->b = 0;
	}
	if (xy && ((pl > 0 && fc->n->b < MAX_INCLINED_FC_XY)
		|| (pl < 0 && fc->n->b > -MAX_INCLINED_FC_XY)))
		fc->n->b += pl;
	else if (!xy && ((pl > 0 && fc->n->a < MAX_INCLINED_FC_XY)
		|| (pl < 0 && fc->n->a > -MAX_INCLINED_FC_XY)))
		fc->n->a += pl;
	if (fc->n->a == 0 && fc->n->b == 0)
	{
		free(fc->n);
		fc->n = 0;
	}
	else
		fc->n->d = -fc->n->a * fc->x - fc->n->b * fc->y - fc->n->c * fc->z;
}
