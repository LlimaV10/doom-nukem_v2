#include "guardians.h"

void	new_wall(t_wall *w, int x, int y, int ns, int nsw)
{
	w->next = 0;
	w->x = x;
	w->y = y;
	w->nextsector = ns;
	w->nextsector_wall = nsw;
}

void	get_map(t_sdl *iw)
{
	iw->v.sc = 1;
	iw->sectors = (t_sector *)malloc(iw->v.sc * sizeof(t_sector));
	iw->sectors[0].sw = 0;
	iw->sectors[0].nw = 4;
	iw->sectors[0].fr.n = 0;
	iw->sectors[0].fr.x = 0;
	iw->sectors[0].fr.y = 0;
	iw->sectors[0].fr.z = 0;
	iw->sectors[0].cl.n = 0;
	iw->sectors[0].cl.x = 0;
	iw->sectors[0].cl.y = 0;
	iw->sectors[0].cl.z = 1000;
	iw->walls = (t_wall *)malloc(4 * sizeof(t_wall));
	int i = 0;
	new_wall(&(iw->walls[i++]), 0, 0 ,-1, -1);
	new_wall(&(iw->walls[i]), 0, 5000, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 5000, 5000, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 5000, 0, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];
	iw->walls[i].next = &iw->walls[0];
}