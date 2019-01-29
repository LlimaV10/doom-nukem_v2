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

typedef struct	s_picture
{
	//left shift in units
	int		left_plus;
	// //top shift in units
	// int		top_plus;
	// Z Coordinate of top of the picture
	int		zu;
	//picture width
	int		tw;
	//texture
	int		t;

	int		zd;
	int		x0;
	int		x1;
	int		y0;
	int		y1;
	struct s_picture	*next;
}				t_picture;

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
	// Glass texture (-1 if just a portal)
	int				glass;
	// Pictures above the main texture
	t_picture		*p;
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
	// // Sprites inside of this sector
	// t_sprite	*s;
	int			visited;
}				t_sector;

typedef struct	s_anim_portal_rotate
{
	int		start_wall;
	int		angle;
	// 0 - in standart, 1 - rotated
	int		anim_status;
	// -1 - static, else - begin time
	int		time;
	//struct s_anim_portal_rotate *next;
}				t_anim_portal_rotate;

typedef struct	s_player
{
	int		x;
	int		y;
	int		z;
	float	rot;
	int		introt;
	int		rotup;
}				t_player;

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

typedef struct	s_draw_wall_tex_kernel
{
	/*float	tx;
	float	ty;
	float	dty;*/
	t_point2d	lv;
	t_point2d	rv;
	/*float	zu;
	float	zd;
	float	left_len;*/
	float	len_lr;
}				t_draw_wall_tex_kernel;

typedef struct	s_draw_floor_tex
{
	float	sing;
	float	ang;
	float	dang;
	float	lenpl;
	t_point2d	lv;
	t_point2d	rv;
	float	zu;
	float	zd;
	float	left_len;
	float	len_lr;

	float	wall_dist;
	float	weight;
	float		k;
	float		pl;
	t_point2d	floor;
	t_point2d	r;
	float		coef;
	float		px;
	float		py;
}				t_draw_floor_tex;

typedef struct	s_draw_wall_floor_ceil_tex
{
	float	sing;
	float	ang;
	float	dang;
	float	lenpl;
	t_point2d	lv;
	t_point2d	rv;
	float	zu;
	float	zd;
	float	left_len;
	float	len_lr;

	float		wall_dist;
	float		weight;
	float		k;
	float		frpl;
	float		clpl;
	t_point2d	floor;
	t_point2d	r;
	int		frcoef;
	int		clcoef;
	float		px;
	float		py;
	float		zudiff;
	float		zddiff;
	float		tx;
	float		ty;
	float		dty;

	float 		rot;
	float 		dx;
	float 		dy;
	float 		sky_x;
	float 		sky_y;
}				t_draw_wall_floor_ceil_tex;

typedef struct	s_draw_wall_floor_ceil_tex_kernel
{
	float	ang;
	t_point2d	lv;
	t_point2d	rv;
	float	len_lr;
	int		zu;
	int		zd;
}				t_draw_wall_floor_ceil_tex_kernel;

typedef struct	s_draw_floor_tex_kernel
{
	float	ang;
	t_point2d	lv;
	t_point2d	rv;
	float	zu;
	float	zd;
	float	len_lr;
}				t_draw_floor_tex_kernel;

typedef struct	s_draw_skybox
{
	float 		rot;
	float 		dx;
	float 		dy;
	float 		sky_x;
	float 		sky_y;
}				t_draw_skybox;

typedef struct	s_draw_glass
{
	float	sing;
	float	ang;
	float	dang;
	float	lenpl;
	t_point2d	lv;
	t_point2d	rv;
	float	zu;
	float	zd;
	float	left_len;
	float	len_lr;
	float		zudiff;
	float		zddiff;
	float		tx;
	float		ty;
	float		dty;
	int		pixel;

	int		nleft_zu;
	int		nright_zu;
	int		nleft_zd;
	int		nright_zd;
	/*
	float		wall_dist;
	float		weight;
	float		k;
	float		frpl;
	float		clpl;
	t_point2d	floor;
	t_point2d	r;
	int		frcoef;
	int		clcoef;
	float		px;
	float		py;
	
	

	float 		rot;
	float 		dx;
	float 		dy;
	float 		sky_x;
	float 		sky_y;*/
}				t_draw_glass;
typedef struct	s_draw_picture
{
	float lang;
	float rang;
	float plen;
	int rx0;
	int rx1;
	int ry0_up;
	int ry0_down;
	int ry1_up;
	int ry1_down;
	float dy_down;
	float dy_up;
	float down;
	float up;
	float dx;
	float pic_x;
	float pic_y;
	float	dy_plus;
}				t_draw_picture;

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

typedef struct	s_draw_info
{
	char	*s;
	char	*s2;
	char	*s3;
	SDL_Surface	*stext;
	SDL_Color	col;
	SDL_Rect	rect;
}				t_draw_info;

// typedef struct	s_save_wall_pairs_closest
// {
// 	t_save_wall_pairs	*tmp;
// 	// 0 - left, 1 - right
// 	int		lr;
// }				t_save_wall_pairs_closest;

typedef struct	s_sprite
{
	int		x;
	//int		t;
	SDL_Surface	*t;
	cl_mem		*t_kernel;
	int		y;
	int		z;
	int		x_s;
	float	plen;
	float	pplen;
	float	dist;
	int		spritewidth;
	int		spriteheight;
	int		sy;
	int		sx;
	int		ey;
	int		ex;
	int		i;
	int		num_sec;
	int		top[WINDOW_W + 1];
	int		bottom[WINDOW_W + 1];

	float	scale;
	struct s_sprite	*next;

	int		draweble;
	int		type;
}           t_sprite;

typedef struct	s_draw
{
	int			top[WINDOW_W + 1];
	int			bottom[WINDOW_W + 1];
	int			top_save[WINDOW_W + 1];
	int			bottom_save[WINDOW_W + 1];
	t_intpoint2d	view_dir;
	t_line2d	screen;
	float		screen_len;
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
	t_wall		*prev_sector_wall;
	int			screen_left;
	int			screen_right;
	int			*save_bot_betw;
	int			*save_top_betw;
}				t_draw;

typedef struct	s_kernel
{
	char	*source_str;
	size_t	source_size;
	cl_context context;
	cl_device_id device_id;
	cl_uint	ret_num_devices;
	cl_uint	ret_num_platforms;
	cl_platform_id	*platforms;
	cl_int	ret;
	cl_command_queue command_queue;
	cl_program program;
	cl_kernel kernel;
	cl_mem		m_sur;
	cl_mem		m_t[TEXTURES_COUNT];
	cl_mem		m_top;
	cl_mem		m_bottom;
	cl_mem	m_wallTop;
	cl_mem	m_wallBot;
	cl_mem	m_cint;
	cl_mem	m_cfloat;
	cl_mem	m_top_betw;
	cl_mem	m_bot_betw;
	cl_mem		m_save_top;
	cl_mem		m_save_bottom;
	cl_mem		m_save_top2;
	cl_mem		m_save_bottom2;
	
	cl_mem		m_t_decor[DECOR_TEXTURES_COUNT];
	cl_mem		m_t_enemies[ENEMIES_TEXTURES_COUNT];
	cl_mem		m_t_pickup[PICK_UP_TEXTURES_COUNT];
}				t_kernel;

typedef struct	s_variables
{
	// Sectors Count
	int		sc;
	// Last Sector
	int		ls;
	// FOV angle / 2
	float	angle;
	// 1 - kernel-ON
	int		kernel;
	// move
	int		front;
	int		back;
	int		left;
	int		right;
	// rotate
	int		rot_left;
	int		rot_right;
	int		rot_up;
	int		rot_down;
	int		fall;
	// acceleration (meters per second^2)
	float	accel;
	int		plrzu;
	int		plrzd;
	int		jump;
	int		jump_time;
	int		fps;
	int		tex_to_fill;
	int		scroll_first_tex;
	int		mouse_mode;
	int		mouse_x;
	int		mouse_y;
	SDL_Rect	scroll_tex_rect;
	t_wall		**look_wall;
	t_wall		*look_portal;
	t_sector	**look_sector;
	t_picture	**look_picture;
	// 0 - floor, 1 - ceil
	int		changing_fc;
	SDL_Rect	chang_fc_rect;
	int		count_portal_rot_anim;
	// 0 - Walls texture editing, 1 - Pictures editing
	// int		edit_mode;
	t_picture	*picture_changing;
	t_wall		*wall_picture_changing;
}				t_variables;

typedef struct	s_level
{
	int		skybox;
}				t_level;

// typedef struct	s_visited_portal
// {
// 	t_wall	*wall;
// 	struct	s_visited_portal	*next;
// }				t_visited_portal;

typedef struct	s_visited_sector
{
	int		sec;
	struct	s_visited_sector	*next;
}				t_visited_sector;

typedef struct	s_sdl
{
	SDL_Window	*win;
	SDL_Surface	*sur;
	SDL_Rect	winrect;
	char		quit;
	t_wall		*walls;
	t_sector	*sectors;
	t_anim_portal_rotate	*anim;
	// Player
	t_player	p;
	t_variables	v;
	t_level		l;
	t_draw		d;
	// textures
	SDL_Surface *(t)[TEXTURES_COUNT];
	// textures compression
	float		tsz[TEXTURES_COUNT];
	t_kernel	k;
	int			loop_update_time;
	TTF_Font	*arial_font;
	// t_visited_portal	*visited_portals;
	t_visited_sector	*visited_sectors;
	// Sprites
	t_sprite 	**sprite;
	// Sprite textures
	//SDL_Surface	*(st)[SPRITES_TEXTURES_COUNT];
	SDL_Surface	*(t_decor)[DECOR_TEXTURES_COUNT];
	SDL_Surface	*(t_enemies)[ENEMIES_TEXTURES_COUNT];
	SDL_Surface	*(t_pickup)[PICK_UP_TEXTURES_COUNT];
}				t_sdl;

#endif