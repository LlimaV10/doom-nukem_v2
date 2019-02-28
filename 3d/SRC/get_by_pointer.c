#include "../guardians.h"

int		get_sector_by_pointer(t_sdl *iw, t_sector *sec)
{
	int		i;

	i = -1;
	while (++i < iw->v.sc)
		if (&iw->sectors[i] == sec)
			return (i);
	return (0);
}

int		get_wall_by_pointer(t_sdl *iw, t_sector *sec, t_wall *w)
{
	int		i;

	i = sec->sw - 1;
	while (++i < sec->sw + sec->nw)
		if (iw->walls[i].next == w)
			return (i);
	return (0);
}