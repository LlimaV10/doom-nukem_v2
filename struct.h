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
	// Texture number
	int				t;
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
	// Texture
	int		t;
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
	int		introt;
	float	rotup;
}				t_player;

typedef struct	s_variables
{
	// Sectors Count
	int		sc;
	// Last Sector
	int		ls;
	// FOV angle / 2
	float	angle;
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

typedef struct	s_draw_wall_tex
{
	float	tx;
	float	ty;
	float	dty;
	float	sing;
	float	ang;
	float	dang;
	float	lenpl;
	t_point2d	lv;
	t_point2d	rv;
	float		zudiff;
	float	zddiff;
	float	zu;
	float	zd;
	float	left_len;
	float	len_lr;
}				t_draw_wall_tex;

typedef struct	s_save_wall
{
	int		x;
	float	len;
	float	plen;
	float	olen;
	t_intpoint2d	p;
	t_wall	*wall;
	int		zu;
	int		zd;
	struct s_save_wall	*next;
}				t_save_wall;

typedef struct	s_save_wall_pairs
{
	t_save_wall		*left;
	t_save_wall		*right;
	struct	s_save_wall_pairs *next;
}				t_save_wall_pairs;

typedef struct	s_draw
{
	int			top[WINDOW_W + 1];
	int			bottom[WINDOW_W + 1];
	t_intpoint2d	view_dir;
	t_line2d	screen;
	t_line2d	left_line;
	t_line2d	right_line;
	t_point2d	left_point;
	t_point2d	right_point;
	// Current Sector
	int			cs;
	// Visible Walls
	t_save_wall	*vw;
	// Left Right Walls to Draw
	t_save_wall_pairs	*vwp;
	int			*wallTop;
	int			*wallBot;
	int			prev_sector;
	int			screen_left;
	int			screen_right;
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
	SDL_Surface *(t)[TEXTURES_COUNT]; // textures
	float		tsz[TEXTURES_COUNT]; // textures compression
}				t_sdl;

#endif