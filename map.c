#include "guardians.h"

void	new_wall(t_wall *w, int x, int y, int ns, int nsw)
{
	w->next = 0;
	w->x = x;
	w->y = y;
	w->nextsector = ns;
	w->nextsector_wall = nsw;
	w->glass = -1;
	w->t = 2;
	w->p = 0;
}

void	get_map(t_sdl *iw)
{
	iw->v.sc = 3;
	iw->sectors = (t_sector *)malloc(iw->v.sc * sizeof(t_sector));
	iw->sectors[0].sw = 0;
	iw->sectors[0].nw = 6 + 4 + 4;/////
	//iw->sectors[0].s = 0;
	iw->sectors[0].fr.n = 0;/*(t_vector *)malloc(sizeof(t_vector));
	iw->sectors[0].fr.n->a = 15;
	iw->sectors[0].fr.n->b = 0;
	iw->sectors[0].fr.n->c = 30;*/
	iw->sectors[0].fr.x = 5000;
	iw->sectors[0].fr.y = 0;
	iw->sectors[0].fr.z = -300;
	iw->sectors[0].fr.t = 3;
	iw->sectors[0].cl.n = (t_vector *)malloc(sizeof(t_vector));
	iw->sectors[0].cl.n->a = 15;
	iw->sectors[0].cl.n->b = 0;
	iw->sectors[0].cl.n->c = 30;
	iw->sectors[0].cl.x = 5000;
	iw->sectors[0].cl.y = 0;
	iw->sectors[0].cl.z = 2000;
	iw->sectors[0].cl.t = -1;
	iw->sectors[0].light = 0;

	iw->sectors[1].sw = 14;///////
	iw->sectors[1].nw = 8 + 4;
	//iw->sectors[1].s = 0;
	iw->sectors[1].fr.n = 0;
	iw->sectors[1].fr.x = 5000;
	iw->sectors[1].fr.y = 0;
	iw->sectors[1].fr.z = 200;
	iw->sectors[1].fr.t = 3;
	iw->sectors[1].cl.n = 0;
	iw->sectors[1].cl.x = 5000;
	iw->sectors[1].cl.y = 0;
	iw->sectors[1].cl.z = 1200;
	iw->sectors[1].cl.t = -1;
	iw->sectors[1].light = 0;

	iw->sectors[2].sw = 26;///////
	iw->sectors[2].nw = 4;
	//iw->sectors[2].s = 0;
	iw->sectors[2].fr.n = 0;/*(t_vector *)malloc(sizeof(t_vector));
	iw->sectors[2].fr.n->a = 0;
	iw->sectors[2].fr.n->b = 1;
	iw->sectors[2].fr.n->c = 2;*/
	iw->sectors[2].fr.x = 8000;
	iw->sectors[2].fr.y = 4000;
	iw->sectors[2].fr.z = 550;
	iw->sectors[2].fr.t = 2;
	iw->sectors[2].cl.n = 0;
	iw->sectors[2].cl.x = 5000;
	iw->sectors[2].cl.y = 0;
	iw->sectors[2].cl.z = 900;
	iw->sectors[2].cl.t = 0;
	iw->sectors[2].light = 0;

	iw->walls = (t_wall *)malloc((iw->sectors[0].nw + iw->sectors[1].nw + iw->sectors[2].nw) * sizeof(t_wall));
	int i = 0;
	new_wall(&(iw->walls[i++]), 0, 0 ,-1, -1);
	new_wall(&(iw->walls[i]), 0, 5000, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 5000, 5000, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];

	i++;
	new_wall(&(iw->walls[i]), 5000, -1000, 1, 14);
	iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 5000, -2000, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 5000, -5000, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];

	iw->walls[i].next = &iw->walls[0];

	i++;
	new_wall(&(iw->walls[i]), 1000, 1000, -1, -1);
	//iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 1000, 850, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 1150, 850, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 1150, 1000, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];

	iw->walls[i].next = &iw->walls[6];


	i++;
	new_wall(&(iw->walls[i]), 1300, 2000, -1, -1);
	//iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 1300, 850, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 2600, 850, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 2600, 2000, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];

	iw->walls[i].next = &iw->walls[10];


	i++;
	new_wall(&(iw->walls[i]), 5000, 2000, 0, 3);
	//iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 5000, 3000, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 7000, 5000, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 7000, 6000, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];

	i++;
	new_wall(&(iw->walls[i]), 10000, 6000, -1, -1);
	iw->walls[i].t = 7;
	printf("i %d\n", i);
	iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 10000, -1000, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 7000, -1000, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 7000, 2000, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];

	iw->walls[i].next = &iw->walls[14];

	i++;
	new_wall(&(iw->walls[i]), 8000, 4000, 2, 29);
	//iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 8000, 3900, 2, 28);
	iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 8500, 3900, 2, 27);
	iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 8500, 4000, 2, 26);
	iw->walls[i - 1].next = &iw->walls[i];

	iw->walls[i].next = &iw->walls[22];

	i++;
	new_wall(&(iw->walls[i]), 8000, 4000, 1, 25);
	//iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 8500, 4000, 1, 24);
	iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 8500, 3900, 1, 23);
	iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 8000, 3900, 1, 22);
	iw->walls[i - 1].next = &iw->walls[i];

	iw->walls[i].next = &iw->walls[26];
}

void	get_map2(t_sdl *iw)
{
	iw->v.sc = 4;
	iw->sectors = (t_sector *)malloc(iw->v.sc * sizeof(t_sector));
	iw->sectors[0].sw = 0;
	iw->sectors[0].nw = 4;
	iw->sectors[0].fr.n = 0;
	iw->sectors[0].fr.x = 5000;
	iw->sectors[0].fr.y = 0;
	iw->sectors[0].fr.z = 0;
	iw->sectors[0].fr.t = 3;
	iw->sectors[0].cl.n = 0;
	iw->sectors[0].cl.x = 5000;
	iw->sectors[0].cl.y = 0;
	iw->sectors[0].cl.z = 2000;
	iw->sectors[0].cl.t = 0;
	
	iw->sectors[1].sw = 4;
	iw->sectors[1].nw = 4;
	iw->sectors[1].fr.n = 0;
	iw->sectors[1].fr.x = 5000;
	iw->sectors[1].fr.y = 0;
	iw->sectors[1].fr.z = 200;
	iw->sectors[1].fr.t = 3;
	iw->sectors[1].cl.n = 0;
	iw->sectors[1].cl.x = 5000;
	iw->sectors[1].cl.y = 0;
	iw->sectors[1].cl.z = 2000;
	iw->sectors[1].cl.t = 0;

	iw->sectors[2].sw = 8;
	iw->sectors[2].nw = 4;
	iw->sectors[2].fr.n = 0;
	iw->sectors[2].fr.x = 5000;
	iw->sectors[2].fr.y = 0;
	iw->sectors[2].fr.z = 400;
	iw->sectors[2].fr.t = 3;
	iw->sectors[2].cl.n = 0;
	iw->sectors[2].cl.x = 5000;
	iw->sectors[2].cl.y = 0;
	iw->sectors[2].cl.z = 2000;
	iw->sectors[2].cl.t = 0;

	iw->sectors[3].sw = 12;
	iw->sectors[3].nw = 4;
	iw->sectors[3].fr.n = 0;
	iw->sectors[3].fr.x = 5000;
	iw->sectors[3].fr.y = 0;
	iw->sectors[3].fr.z = 600;
	iw->sectors[3].fr.t = 3;
	iw->sectors[3].cl.n = 0;
	iw->sectors[3].cl.x = 5000;
	iw->sectors[3].cl.y = 0;
	iw->sectors[3].cl.z = 2000;
	iw->sectors[3].cl.t = 0;

	iw->walls = (t_wall *)malloc((iw->sectors[0].nw + iw->sectors[1].nw + iw->sectors[2].nw+ iw->sectors[3].nw) * sizeof(t_wall));
	int i = 0;
	new_wall(&(iw->walls[i++]), 0, 0 ,-1, -1);
	new_wall(&(iw->walls[i]), 0, 5000, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 5000, 5000, 1, 4);
	iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 5000, 0, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];

	iw->walls[i].next = &iw->walls[0];

	i++;
	new_wall(&(iw->walls[i++]), 5000, 0, 0, 2);
	new_wall(&(iw->walls[i]), 5000, 5000, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 5200, 5000, 2, 8);
	iw->walls[i - 1].next = &iw->walls[i];

	i++;
	new_wall(&(iw->walls[i]), 5200, 0, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];

	iw->walls[i].next = &iw->walls[4];

	i++;
	new_wall(&(iw->walls[i++]), 5200, 0 , 1, 6);
	new_wall(&(iw->walls[i]), 5200, 5000, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 5400, 5000, 3, 12);
	iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 5400, 0, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];

	iw->walls[i].next = &iw->walls[8];

	i++;
	new_wall(&(iw->walls[i++]), 5400, 0 , 2, 10);
	new_wall(&(iw->walls[i]), 5400, 5000, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 5600, 5000, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];
	i++;
	new_wall(&(iw->walls[i]), 5600, 0, -1, -1);
	iw->walls[i - 1].next = &iw->walls[i];

	iw->walls[i].next = &iw->walls[12];
	printf("asfdasga\n\n\n\n");
}
//
//void	get_animation(t_sdl *iw)
//{
//	iw->v.count_portal_rot_anim = 1;
//	iw->anim = (t_anim_portal_rotate *)malloc(sizeof(t_anim_portal_rotate));
//	iw->anim[0].start_wall = 22;
//	iw->anim[0].angle = 2;
//	iw->anim[0].anim_status = 0;
//	iw->anim[0].time = -1;
//}