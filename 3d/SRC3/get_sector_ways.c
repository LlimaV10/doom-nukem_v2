#include "../guardians.h"

void	free_way(t_sector_ways **way)
{
	t_sector_way	*tmp;
	t_sector_way	*tmp2;

	if (*way == 0)
		return;
	tmp = (*way)->way_start;
	while (tmp)
	{
		tmp2 = tmp;
		tmp = tmp->next;
		free(tmp2);
	}
	free(*way);
	*way = 0;
}

int		sector_in_way(t_sdl *iw, t_sector_ways *way, int sector)
{
	t_sector_way	*tmp;

	if (way == 0)
		return (0);
	tmp = way->way_start;
	while (tmp)
	{
		if (iw->walls[tmp->portal].nextsector == sector)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

void	add_portal_in_way2(t_sector_ways *tmp, int portal)
{
	t_sector_way	*nw;
	t_sector_way	*w;

	nw = (t_sector_way *)malloc(sizeof(t_sector_way));
	nw->portal = portal;
	nw->next = 0;
	if (tmp->way_start == 0)
		tmp->way_start = nw;
	else
	{
		w = tmp->way_start;
		while (w->next)
			w = w->next;
		w->next = nw;
	}
}

t_sector_ways	*add_portal_in_way(t_sector_ways *current_way, int portal)
{
	t_sector_ways	*tmp;
	t_sector_way	*way;

	tmp = (t_sector_ways *)malloc(sizeof(t_sector_ways));
	tmp->way_start = 0;
	if (current_way == 0)
	{
		tmp->way_start = (t_sector_way *)malloc(sizeof(t_sector_way));
		tmp->way_start->next = 0;
		tmp->way_start->portal = portal;
		return (tmp);
	}
	way = current_way->way_start;
	while (way)
	{
		add_portal_in_way2(tmp, way->portal);
		way = way->next;
	}
	add_portal_in_way2(tmp, portal);
	return (tmp);
}

void	go_in_sector_way(t_sdl *iw, t_get_sectors_ways *g,
	t_sector_ways *current_way)
{
	int				wall;
	int				save;

	if (g->current == g->to)
	{
		current_way->next = g->ways;
		g->ways = current_way;
		return;
	}
	wall = iw->sectors[g->current].sw - 1;
	while (++wall < iw->sectors[g->current].sw + iw->sectors[g->current].nw)
	{
		if (iw->walls[wall].nextsector == -1 ||
			sector_in_way(iw, current_way, iw->walls[wall].nextsector) ||
			iw->walls[wall].nextsector == g->from)
			continue;
		save = g->current;
		g->current = iw->walls[wall].nextsector;
		go_in_sector_way(iw, g, add_portal_in_way(current_way, wall));
		g->current = save;
	}
	free_way(&current_way);
}

void	get_sectors_ways2(t_sdl *iw)
{
	t_get_sectors_ways	g;

	g.from = -1;
	while (++g.from < iw->v.sc)
	{
		g.to = -1;
		while (++g.to < iw->v.sc)
		{
			if (g.from == g.to)
				continue;
			g.ways = 0;
			g.current = g.from;
			go_in_sector_way(iw, &g, 0);
			iw->ways[g.from][g.to] = g.ways;
		}
	}
}

void	get_sectors_ways(t_sdl *iw)
{
	int		i;
	int		j;

	iw->ways = (t_sector_ways ***)malloc(iw->v.sc * sizeof(t_sector_ways **));
	i = -1;
	while (++i < iw->v.sc)
	{
		iw->ways[i] = (t_sector_ways **)malloc(iw->v.sc * sizeof(t_sector_ways *));
		j = -1;
		while (++j < iw->v.sc)
			iw->ways[i][j] = 0;
	}
	get_sectors_ways2(iw);
}