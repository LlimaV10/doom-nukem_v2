#ifndef __STRUCT_H
# define __STRUCT_H

# include "defines.h"

typedef struct	s_line2d
{
	// Ax + By + C = 0. This is A
	float	a;
	// Ax + By + C = 0. This is B
	float	b;
	// Ax + By + C = 0. This is C
	float	c;
}				t_line2d;

typedef struct	s_wall
{
	int				x;
	int				y;
	// Wall line
	t_line2d		l;
	// Index of the next sector
	int				nextsector;
	// Index of the same wall in the next sector
	int				nextsector_wall;
	struct s_wall	*next;
}				t_wall;

typedef struct	s_vector
{
	// Ax + By + Cz + D = 0 this is A
	int		a;
	// Ax + By + Cz + D = 0 this is B
	int		b;
	// Ax + By + Cz + D = 0 this is C
	int		c;
	// Ax + By + Cz + D = 0 this is D
	int		d;
}				t_vector;

typedef struct	s_sector_fc
{
	// X coordinate of floor|ceil
	int		x;
	// Y coordinate of floor|ceil
	int		y;
	// Z coordinate of floor|ceil
	int		z;
	// Normal vector (null if no slope)
	t_vector	*n;
}				t_sector_fc;
typedef struct	s_sector
{
	// Start Wall
	int			sw;
	// Number Walls
	int			nw;
	// Sector Floor
	t_sector_fc	fr;
	// Sector Ceil
	t_sector_fc	cl;
}				t_sector;

typedef struct	s_player
{
	int		x;
	int		y;
	int		z;
	float	rot;
	float	rotup;
}				t_player;

typedef struct	s_variables
{
	// Sectors Count
	int		sc;
	// Last Sector
	int		ls;
}				t_variables;

typedef struct	s_point2d
{
	float	x;
	float	y;
}				t_point2d;

typedef struct	s_intpoint2d
{
	int		x;
	int		y;
}				t_intpoint2d;

typedef struct	s_draw_line
{
	int			x0;
	int			y0;
	int			x1;
	int			y1;
}				t_draw_line;

typedef struct	s_brez
{
	int			x;
	int			y;
	int			dx;
	int			dy;
	int			sx;
	int			sy;
	int			k;
	int			*wall_y;
	int			prev_x;
}				t_brez;

typedef struct	s_save_wall
{
	int		x;
	float	len;
	float	plen;
	t_wall	*wall;
	int		zu;
	int		zd;
	struct s_save_wall	*next;
}				t_save_wall;

typedef struct	s_draw
{
	int			top[WINDOW_W];
	int			bottom[WINDOW_W];
	t_line2d	view;
	t_intpoint2d	view_dir;
	t_line2d	screen;
	t_point2d	screen_point;
	// l / 2, where l - screen length
	float		screen_length;
	// Player Screen Side
	float		pss;
	// Current Sector
	int			cs;
	// Visible Walls
	t_save_wall	*vw;
	int			*wallTop;
	int			*wallBot;
}				t_draw;

typedef struct	s_sdl
{
	SDL_Window	*win;
	SDL_Surface	*sur;
	char		quit;
	t_wall		*walls;
	t_sector	*sectors;
	// Player
	t_player	p;
	t_variables	v;
	t_draw		d;
}				t_sdl;

#endif