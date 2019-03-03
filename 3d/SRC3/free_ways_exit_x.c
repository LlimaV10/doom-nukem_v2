#include "../guardians.h"

void	free_sector_ways(t_sdl *iw)
{
	int		i;
	int		j;
	t_sector_ways	*tmp;
	t_sector_way	*tmp2;
	t_sector_ways	*tmp_f;
	t_sector_way	*tmp2_f;

	i = -1;
	while (++i < iw->v.sc)
	{
		j = -1;
		while (++j < iw->v.sc)
		{
			tmp = iw->ways[i][j];
			while (tmp)
			{
				tmp2 = tmp->way_start;
				while (tmp2)
				{
					tmp2_f = tmp2;
					tmp2 = tmp2->next;
					free(tmp2_f);
				}
				tmp_f = tmp;
				tmp = tmp->next;
				free(tmp_f);
			}
		}
		free(iw->ways[i]);
	}
	free(iw->ways);
}

void	exit_x(t_sdl *iw)
{
	SDL_FreeSurface(iw->sur);
	SDL_DestroyWindow(iw->win);
	TTF_Quit();
	SDL_Quit();
	system("leaks doom-nukem");
	exit(0);
}