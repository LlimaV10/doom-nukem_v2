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
	iw->v.sc = 2;
	iw->sectors = (t_sector *)malloc(iw->v.sc * sizeof(t_sector));
	iw->sectors[0].sw = 0;
	iw->sectors[0].nw = 6;
	iw->sectors[0].fr.n = (t_sector_fc *)malloc(sizeof(t_sector_fc));
	iw->sectors[0].fr.n->a = 1;
	iw->sectors[0].fr.n->b = 0;
	iw->sectors[0].fr.n->c = -15;
	iw->sectors[0].fr.x = 5000;
	iw->sectors[0].fr.y = 0;
	iw->sectors[0].fr.z = 0;
	iw->sectors[0].cl.n = (t_sector_fc *)malloc(sizeof(t_sector_fc));
	iw->sectors[0].cl.n->a = 1;
	iw->sectors[0].cl.n->b = 0;
	iw->sectors[0].cl.n->c = -15;
	iw->sectors[0].cl.x = 5000;
	iw->sectors[0].cl.y = 0;
	iw->sectors[0].cl.z = 1000;

	iw->sectors[1].sw = 6;
	iw->sectors[1].nw = 3;
	iw->sectors[1].fr.n = 0;
	iw->sectors[1].fr.x = 5000;
	iw->sectors[1].fr.y = 0;
	iw->sectors[1].fr.z = 200;
	iw->sectors[1].cl.n = 0;
	iw->sectors[1].cl.x = 5000;
	iw->sectors[1].cl.y = 0;
	iw->sectors[1].cl.z = 900;

	iw->walls = (t_wall *)malloc((6 + 3) * sizeof(t_wall));
	int i = 0;
	new_wall(&(iw->walls[i++]), 0, 0 ,-1, -1);
	new_wall(&(iw->walls[i]), 0, 5000, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 5000, 5000, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];

	i++;
	new_wall(&(iw->walls[i]), 5000, 3000, 1, 6);
	iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 5000, 2000, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 5000, -5000, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];

	iw->walls[i].next = &iw->walls[0];

	i++;
	new_wall(&(iw->walls[i]), 5000, 2000, 0, 0);
	//iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 5000, 3000, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 7000, 2500, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];

	iw->walls[i].next = &iw->walls[6];
}