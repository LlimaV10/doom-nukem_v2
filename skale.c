
#include <SDL.h>

# define FOOTX 500
# define FOOTY 500


typedef struct		s_col
{
	int				x1;
	int				y1;
	int				x2;
	int				y2;
	int				xmod;
	int				ymod;
	int				sect;
}					t_col;


	typedef struct	s_xy
{
	int				x;
	int				y;
}					t_xy;
				

typedef struct		s_doom
{
	SDL_Window		*window;
	SDL_Surface		*bmp;
	SDL_Surface		*enot;
	SDL_Rect		rect;
	int				color;
	int				heals;
	int				shell;
	int				i;
	t_xy			mas[300];
	t_xy			mas2[300];
	int				rad;
	int				koord;

}					t_doom;

typedef struct		s_brz
{
	int				dx;
	int				dy;
	int				lengthx;
	int				lengthy;
	int				length;
	int				x;
	int				y;
	int				d;
	int				color;
}					t_brz;

void    set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    if (x >= 0 && x < 1000 && y >= 0 && y < 1000 + 300)
    {
        Uint8 *target_pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * 4;
        *(Uint32 *)target_pixel = pixel;
    }
}


void	ft_line_x(t_doom *den, t_brz *tip, int x1, int y1)
{
	tip->x = x1;
	tip->y = y1;
	tip->d = -tip->lengthx;
	tip->length++;
	while (tip->length--)
	{
		if (tip->x >= 0 && tip->y >= 0)
			set_pixel(den->bmp, tip->x, tip->y, tip->color);
		tip->x += tip->dx;
		tip->d += 2 * tip->lengthy;
		if (tip->d > 0)
		{
			tip->d -= 2 * tip->lengthx;
			tip->y += tip->dy;
		}
	}
}

void	ft_line_y(t_doom *den, t_brz *tip, int x1, int y1)
{
	tip->x = x1;
	tip->y = y1;
	tip->d = -tip->lengthy;
	tip->length++;
	while (tip->length--)
	{
		if (tip->y < 1000 && tip->x >= 0 && tip->y >= 0)
			set_pixel(den->bmp, tip->x, tip->y, tip->color);
		tip->y += tip->dy;
		tip->d += 2 * tip->lengthx;
		if (tip->d > 0)
		{
			tip->d -= 2 * tip->lengthy;
			tip->x += tip->dx;
		}
	}
}

void	ft_line(t_doom *den, t_col *vec)
{
	t_brz	tip;
	int		x2;
	int		x1;
	int		y2;
	int		y1;

	x1 = vec->x1;
	x2 = vec->x2;
	y1 = vec->y1;
	y2 = vec->y2;
	tip.color = den->color;
	tip.dx = (x2 - x1 >= 0 ? 1 : -1);
	tip.dy = (y2 - y1 >= 0 ? 1 : -1);
	tip.lengthx = abs(x2 - x1);
	tip.lengthy = abs(y2 - y1);
	tip.length = (tip.lengthx > tip.lengthy ? tip.lengthx : tip.lengthy);
	if (tip.length == 0 && y2 < 1000 && x1 >= 0 && y2 >= 0 )
		set_pixel(den->bmp, x1, y2, tip.color);
	if (tip.lengthy <= tip.lengthx)
		ft_line_x(den, &tip, x1, y1);
	else
		ft_line_y(den, &tip, x1, y1);
}


Uint32	get_pixel2(SDL_Surface *sur, const int x, const int y)
{
	uint8_t *v;
	int		bpp;

	if (x < 0 || x >= sur->w || y < 0 || y >= sur->h)
		return (0);
	bpp = sur->format->BytesPerPixel;
	v = (uint8_t *)sur->pixels + y * sur->pitch + x * bpp;
	return (v[0] | v[1] << 8 | v[2] << 16);
}

void			scale_image(SDL_Surface *bmp, SDL_Surface *icon, float scale)
{
	int 		    y;
    int             x;
	float 		    new_y;
    float           new_x;
	float		    final_x;
	float			final_y;
	float 		    dy;
    float           dx;

	new_y = 0;
	new_x = 0;
    x = -1;

    final_x = icon->w * scale; 
	dx =  (float)icon->w / final_x;
    final_y = icon->h * scale;
	dy = (float)icon->h / final_y;

    if (scale < 0.0)
    {
        final_x = icon->w; 
        final_y = icon->h;
    }
	while (++x < final_x )
	{
        new_y = 0;
		y = -1;
		while (++y < final_y)
		{
			set_pixel(bmp, x, y, get_pixel2(icon, (int)new_x , (int)new_y));
			new_y += dy;
		}
		new_x += dx;
	}
}

void drawCircle(t_doom *den, int xc, int yc, int x, int y) 
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



	// set_pixel(den->bmp, xc-y, yc-x, 0xff0000);  
	// set_pixel(den->bmp, xc-x, yc-y, 0xff0000);
    // set_pixel(den->bmp, xc+x, yc-y, 0xff0000);
	// set_pixel(den->bmp, xc+y, yc-x, 0xff0000);


	den->mas2[den->i].x = xc-y;
	den->mas2[den->i].y = yc+x;

	den->mas2[150 - den->i].x = xc-x;
	den->mas2[150 - den->i].y = yc+y;

	den->mas2[den->i + 150].x = xc+x;
	den->mas2[den->i + 150].y = yc+y;

	den->mas2[300 - den->i].x = xc+y;
	den->mas2[300 - den->i].y = yc+x;

	// set_pixel(den->bmp, xc+x, yc+y, 0xff0000); 
   	// set_pixel(den->bmp, xc-x, yc+y, 0xff0000); 
   	// set_pixel(den->bmp, xc-y, yc+x, 0xff0000);
	// set_pixel(den->bmp, xc+y, yc+x, 0xff0000); 


	den->i += 1;

}

void circle(t_doom *den, int xc, int yc)
{
	den->koord = 0;
	den->i = 0;
	int	r;

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


void	load_image_foot(t_doom *den)
{
	den->enot = SDL_LoadBMP("icon/enot.bmp");
	
	den->rect.x = 412;
	den->rect.y = 402;
	den->rect.h = 20;
	den->rect.w = 20;
	SDL_BlitSurface(den->enot, NULL, den->bmp, &den->rect);

}

void	make_health(t_doom *den)
{
	int		i;
	t_col	vec;	

	load_image_foot(den);
	den->color = 0xAA0000;
	i = 0;
	while (i < (3 * den->heals))
	{
		vec.x1 = den->mas[i].x;
		vec.x2 = den->mas[i].x + 30;
		vec.y1 = den->mas[i].y;
		vec.y2 = den->mas[i].y;
		ft_line(den, &vec);
		i++;
	}
	i = 0;
	den->color = 0x0000AA;
	while (i < (3 * den->shell))
	{
		vec.x1 = den->mas2[i].x;
		vec.x2 = den->mas2[i].x + 30;
		vec.y1 = den->mas2[i].y;
		vec.y2 = den->mas2[i].y;
		ft_line(den, &vec);
		i++;
	}
}

int		main()
{
    SDL_Event 		e;
    const Uint8		*state;
    int				quit;
	t_doom			den;

    SDL_Init(SDL_INIT_EVERYTHING);


	den.rad = 105;
	den.heals = 100;
	den.shell = 100;

    quit = 0;
	den.window = SDL_CreateWindow("doom_nuken",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);
	den.bmp = SDL_GetWindowSurface(den.window);
    

	circle(&den, FOOTX, FOOTY);
	make_health(&den);
    state = SDL_GetKeyboardState(NULL);

    SDL_UpdateWindowSurface(den.window);

    while (!quit)
	{	
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				quit = 1;
			if (e.type == SDL_KEYDOWN)
			{
				if (state[SDL_SCANCODE_ESCAPE])
					quit = 1;
            }
        }
	
	}
	SDL_DestroyWindow(den.window);
	SDL_Quit();

	return (0);
}