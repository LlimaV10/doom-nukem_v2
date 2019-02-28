#include "../guardians.h"

void	drawCircle(t_hud *den, int xc, int yc, int x, int y)
{ 
	den->koord += 4;
	den->mas[den->i].x = xc - y;
	den->mas[den->i].y = yc - x;
	den->mas[150 - den->i].x = xc - x;
	den->mas[150 - den->i].y = yc - y;
	den->mas[den->i + 151].x = xc + x;
	den->mas[den->i + 151].y = yc - y;
	den->mas[300 - den->i].x = xc + y;
	den->mas[300 - den->i].y = yc - x;
	den->mas2[den->i].x = xc-y;
	den->mas2[den->i].y = yc+x;
	den->mas2[150 - den->i].x = xc-x;
	den->mas2[150 - den->i].y = yc+y;
	den->mas2[den->i + 150].x = xc+x;
	den->mas2[den->i + 150].y = yc+y;
	den->mas2[300 - den->i].x = xc+y;
	den->mas2[300 - den->i].y = yc+x;
	den->i += 1;
}

void	circle(t_hud *den, int xc, int yc)
{
	den->koord = 0;
	den->i = 0;
	int	r;

	den->rect.x = FOOTX - den->rad + 20;
	den->rect.y = FOOTY - den->rad + 2;
	den->rect.h = den->rad * 2 - 5;
	den->rect.w = den->rad * 2 - 5;
	r = den->rad;
    int x = 0, y = r; 
    int d = 3 - 2 * r; 
    drawCircle(den, xc, yc, x, y); 
    while (y >= x) 
    { 
        x++; 
        if (d > 0) 
        { 
            y--;  
            d = d + 4 * (x - y) + 10; 
        } 
        else
            d = d + 4 * x + 6; 
        drawCircle(den, xc, yc, x, y);  
    }
}

void	ft_line2(t_sdl *iw, int x, int y, int color)
{
	int		i;

	i = x - 1;
	while (++i < x + 30)
		set_pixel2(iw->sur, i, y, color);
}

void	make_health(t_hud *den, t_sdl *iw)
{
	int		i;
	char	*s;

	i = -1;
	while (++i < (3 * iw->p.health))
		ft_line2(iw, den->mas[i].x, den->mas[i].y, 0xAA0000);
	i = -1;
	while (++i < (3 * den->shell))
		ft_line2(iw, den->mas2[i].x, den->mas2[i].y, 0x0000AA);
	//SDL_BlitSurface(den->enot, NULL, iw->sur, &den->rect);
	ft_scaled_blit(den->enot, iw->sur, &den->rect);
	draw_text_blue(iw, "/", WINDOW_W - 50, 185);
	if (iw->guns.bullets_in_stock[iw->guns.gun_in_hands] > 0)
	{
		draw_text_blue(iw, (s = ft_itoa(iw->guns.bullets_in_stock[iw->guns.gun_in_hands])), WINDOW_W - 40, 185);
		free(s);
	}
}