/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom_Nuken.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/27 16:41:41 by ddehtyar          #+#    #+#             */
/*   Updated: 2018/08/27 16:41:42 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _DOOM_NUKEN_H
# define _DOOM_NUKEN_H

# include <stdlib.h>
# include <math.h>
# include <time.h>

# include "../3d/guardians.h"

# ifdef __linux__
#  include <SDL2/SDL.h>
#  include <SDL2/SDL_ttf.h>
# else
#  include <SDL.h>
#  include <SDL_ttf.h>
# endif

# define WIDTH 2000
# define HEIGHT 1100
# define START 50
# define LIM -1000000
# define XSKAPE (START + den->skape) + den->startx
# define YSKAPE (START + den->skape) + den->starty
# define SKAPE (START + den->skape);

typedef struct		s_insect
{
	long int x1; 
	long int y1;
	long int x2;
	long int y2;
	unsigned long int wallcrossed;
}					t_insect;

typedef struct		s_rotate
{
	double        hwd;
    double        hhd;
    double        hws;
    double        hhs;
    double        a; 
    double        b; 
    double        r; 
	int				y;
	int				x;
	int				i;
	int				j;

}					t_rotate;

typedef struct		s_cord
{
	int				x;
	int				y;
	int				bool_cor;
	int				inside;
}					t_cord;

typedef struct	s_sector_list
{
	// Sector Floor
	t_sector_fc	fr;
	// Sector Ceil
	t_sector_fc	cl;
	struct	s_sector_list	*next;
	int		id;
	t_picture	*light;
	int			light_wall_numb;
	int			light_numb;
}				t_sector_list;

typedef struct		s_wals
{

	int				x;
	int				y;
	int				x1;
	int				y1;
	int				sec;
	int				blouk;

	int				inside; //внутри сектора

	int				tex;

	struct s_sector_list	*next_sector;
	struct s_wals	        *nextsector_wall;

	struct s_wals	*next;

	int		id;

	int				glass;
	t_picture		*p;
	int						next_sector_numb;
	int						nextsector_wall_numb;
}					t_wals;

typedef struct		s_getmap
{
	int				w;
	int				s;
	int				i;
	int				cw;
	int				inside;
	int				inside_start;
	t_sector_list	*sl;
	t_wals			*tmp;

}					t_getmap;

typedef struct		s_sort
{
	t_line2d		l;
	t_intpoint2d	v1;
	t_intpoint2d	v2;
	float			sector_dir;
}					t_sort;

typedef struct		s_doom
{
	t_sector_list	*sectors;
	int				border;
	int				skape;
	int				retskape;
	int				startx;
	int				starty;
	int				color;
	int				b_color;
	int				button;
	int				change;
	int				walls;
	int				sprites;
	int				incede;
	t_sort			*sort;

	t_sprite		*sprite;

	t_wals			*tmp;

	int				sec;
	int				secbak;
	int				blouk;

	int				i;
	
	int				secbaktmp;

	int				noblouk;

	int				blouk3d; //  переменная для блокировки

	int				xsec;
	int				ysec;

	int				finish; //

	int				select; // 

	int				button_change;
	int				quit;
	t_col			rec;
	t_col			rec_two;

	t_wals			*find_tmp;
	t_wals			*find_tmp_one;
	t_wals			*find_tmp_two;
	t_sprite		*find_sprite;

	SDL_Window		*window;
	SDL_Surface		*bmp;
	SDL_Surface		*icon[13];

	TTF_Font		*font;
    SDL_Color		color_text;
    SDL_Surface		*TTF_TextSolid;
	SDL_Rect 		rect;

	t_cord			xy;

	t_player		player;
	SDL_Cursor 		*cursor;
	int				blockwall;

	double       	angle;			

	// 3DDDDDD
	t_sdl			iw;
	int				fd;
	char			*fname;
}					t_doom;

void			ft_line_y(t_doom *den, t_brz *tip, int x1, int y1);
void			ft_line_x(t_doom *den, t_brz *tip, int x1, int y1);
void			ft_line(t_doom *den, t_col *vec);
void			pixel_width(t_doom *den, int x, int y, int colort);
void			pixel_bigwidth(t_doom *den, int x, int y, int colort);

Uint32			get_pixel2(SDL_Surface *sur, const int x, const int y);
void			coordinate_network(int x, int y, t_doom *den, t_col *vec);
void			set_pixel(SDL_Surface *bmp, int x, int y, Uint32 pixel);
void			load_image(t_doom *den);
void			free_surface(t_doom *den);

void			map_redactor_mane2(t_doom *den);
void			map_redactor_mane(t_doom *den, t_col *vec);
void			clean_find_vec(t_doom *den);
void			map_network (t_doom *den);
void			findstart(t_doom *den);

void			board_display_three(t_doom *den);
void			board_display_two(t_doom *den);
void			board_display(t_doom *den);
void			clean_display(t_doom *den);
void			blouk_wall(t_doom *den, t_wals *tmp, t_col *vec);

void			save_list(t_doom *den, t_col *vec);
void			save_map(int x, t_doom *den, t_col *vec);
void			save_map_help(t_doom *den, t_col *vec);
void			save_list_help(t_doom *den, t_col *vec);
void			save_map_help_two(t_doom *den, t_col *vec);

void			ft_lstaddmap(t_wals **start, t_wals *new);
void			ft_lstnewmap(t_wals **new, int x, int y, t_doom *den);
void			ft_lstnewmap_sec(t_wals **new, int x, int y, t_doom *den);
void			ft_save_parm(t_wals **tmp, int x, int y, t_doom *den);
void			ft_lstnewmap_sec_help(t_doom *den);

float 			isleft_or_right(t_wals *list);
void			sort_sector(t_doom *den);
void			add_sector_dir(t_wals *tmp, t_wals *last, t_sort *sort);
void			sort_list_incide(t_doom *den);
void			check_data_sector(t_doom *den);

void			write_map_help(t_doom *den, t_col *vec);
void			write_map(t_doom *den, t_col *vec);
void			write_list_help(t_doom *den, t_wals *tmp, t_wals *rmp, t_wals *nmp);
void			write_list(t_doom *den, t_wals *tmp);
void			move_sector(t_doom *den, int y, int x);

void			find_wals(int x, int y, t_doom *den);
void			look_find_sector_two(t_doom *den);
void			clear_find(t_doom *den);
void			find_wals_help(t_doom *den);
int				point_into_segments(int x, int y, t_doom *den);

void			sector_in_sector_two(t_doom *den, t_wals *cmp, t_wals *lmp);
void			sector_in_sector(t_doom *den);
void			sort_walls_inside(t_doom *den);
int 			inside_sect(t_doom *den, t_wals **tmp, int sec);
void			sort_walls_inside_help(t_doom *den, int *walls, int i, t_wals *rmp);

int				go_delete(int sec, t_wals *rmp, t_doom *den, int k);
int				go_delete_help(int sec, t_wals *rmp, t_doom *den, int k);
void			delete_sector_one(t_doom *den, int sec);
void			delete_sector_two(t_doom *den, int sec);
void			delete_sector(t_doom *den);

void			delete_pointer(t_doom *den, void* ptr);
void			delete_sector_memory(t_doom *den);
void			delete_sector_memory_one(t_doom *den, t_sector_list *first);
void			delete_sector_memory_two(t_doom *den, t_sector_list *first);
void			delete_wall(t_doom *den, t_col *vec);

void			delete_sprite(t_doom *den, t_sprite *s);
void			check_finish_sprite(t_doom *den);
void			see_sprite(t_doom *den);
void			find_sprite(t_doom *den, int x, int y);
void			add_sprite(t_doom *den);

void			make_portal_sector2(t_doom *den, t_wals *first_element, t_wals *first_element_new);
void			portal_sector_new(t_doom *den, t_wals *rmp, t_wals *lmp);
void			make_portal_sector(t_doom *den, t_col *vec);
int				check_make_portal(t_doom *den);
void			portal_sector(t_doom *den);

void			blouk_player(t_doom *den);
void			player(t_doom *den);
void			rotate_image(t_doom *den);
void			change_way(t_doom *den, int	i, t_col *vec);
void			rotate_image_help(t_doom *den, t_rotate *rot);

void			add_new_sector(t_doom *den);
void			sort_help(int i, t_wals *rmp, t_wals *tmp, int *walls);
void			sort_walls_sec(t_wals *tmp);
t_sector_list	*get_sector_id(t_doom *den, int id);
void			change_way2(t_doom *den, t_wals *first_element, t_wals **first_element2);

void			in_all_sect(t_doom *den);
int				in_all_help(t_doom *den, t_wals *lmp, int sec);
void			blonk_on_now(t_doom *den);
void			clean_string(t_doom *den);
void			back_startwall(t_doom *den, t_col *vec);

void			change_rec(t_doom *den, t_col *rec, t_wals *tmp);
void			next_wall_change(t_doom *den, t_wals *tmp, t_wals *rmp);
void			next_wall (t_doom *den);
void			back_wall_check(t_doom *den, t_wals *rmp, t_wals *lmp);
void			back_wall(t_doom *den, t_col *vec);

void			check_sprite(t_doom *den);
void			sum_sprite(t_doom *den);
void			sprite(t_doom *den);
void			retry_write(t_doom *den, t_col *vec);
void			clear_texture(t_doom *den, t_col *vec);

void			drow_find(t_doom *den);
void			all_sector(t_doom *den, t_wals *tmp);
void			blouk_write(t_doom *den);
void 			clean_vec(t_col *vec);

void			info_display(t_doom *den);
void			info_display_two(t_doom *den, char *infor, SDL_Color color_text);
void			info_display_three(t_doom *den, char *infor, SDL_Color color_text);
void			change_wall_two(t_doom *den, SDL_Color color_text, char *infor);
void			change_wall(t_doom *den, SDL_Color color_text);

void			pres_button_two(int x, int y, t_doom *den);
void			pres_button(int x, int y, t_doom *den, t_col *vec);
int				check_picture_wall(t_doom *den, t_wals *tmp);
int				check_picture(t_doom *den);
void			blouk_delete(t_doom *den);

void			movie_button_five(t_doom *den, t_col *vec);
void			movie_button_four(t_doom *den, t_col *vec);
void			movie_button_three(t_doom *den);
void			movie_button_two(t_doom *den, t_col *vec);
void			movie_button(t_doom *den, t_col *vec);

void			mouse_button_down_sdl(t_doom *den, t_col *vec, SDL_Event e);
void			mouse_button_down_up_sdl(t_doom *den, t_col *vec, SDL_Event e);
void			key_down_sdl(t_doom *den, t_col *vec, const Uint8 *state);
void			ft_sdl_init(t_doom *den);

void			mouse_button_down(t_doom *den, t_col *vec, int x, int y);
void			ft_sdl_init_star(t_doom *den, t_col *vec);
void			key_down_but(t_doom *den, t_col *vec, const Uint8 *state);
void			mousewheel(t_doom *den, t_col *vec, int y);
void			mouse_button_down_select(t_doom *den, t_col *vec, SDL_Event e);

void			move_scan_sector(t_doom *den, const Uint8 *state);
void			move_scan_sprite(t_doom *den, const Uint8 *state);
void			move_scancode(t_doom *den, t_col *vec, const Uint8 *state);
void			mouse_button_rotate(t_doom *den, t_col *vec, int x, int y);
void			mouse_button_sprite(t_doom *den, t_col *vec, int x, int y);

int				count_walls(t_wals *tmp, t_getmap *gm, t_sdl *iw, t_doom *den);
void			get_map_one(t_sdl *iw, t_getmap *gm);
void			get_map_two(t_sdl *iw, t_getmap *gm);
void			get_map_three(t_sdl *iw, t_getmap *gm);
void			get_map(t_sdl *iw, t_doom *den);

int				main_edit(t_doom *den);
int				get_3d_def_edit(t_doom *den);
void			main3d_edit_free(t_doom *den);
void			main3d_edit(t_doom *den);

void			give_date(t_doom *den, t_sdl *iw);
void			unget_map_help(t_sdl *iw, t_wals *tmp, t_sector_list *sl);
void			unget_map(t_sdl *iw, t_doom *den, t_wals *tmp);

int				main_new(t_doom *den);
void			get_3d_def_new_init(t_doom *den);
int				get_3d_def_new(t_doom *den);

void			get_font_file(t_doom *den);
int				usage(void);
int				check_new(t_doom *den, char *fname);
int				check_load(t_doom *den, char *fname);

int				main_edit(t_doom *den);
int				get_3d_def_edit(t_doom *den);
void			main3d_edit_free(t_doom *den);
void			main3d_edit(t_doom *den);

int				get_3d_def_game(t_doom *den);
void			main3d_game(t_doom *den);
void			main3d_game_help(t_doom *den);
int				main_game(t_doom *den);
 
void			go_delete_one(t_wals *rmp, t_wals *nmp);
int				main_help(t_doom *den, char **argv);
int				main_help_two(t_doom *den, char **argv);
int				main_help_one(t_doom *den, char **argv);

int		load_map(t_doom *den);
int		load4_game(t_doom *den);


void	save_map_sec(t_doom *den);
int		count_pictures(t_picture *pic);
void	save_map_walls_pictures(t_doom *den);
void	save_map_sprites(t_doom *den);
void	save_reverse_map_wall_animations(t_doom *den, t_wall_animation *tmp);
void	save_map_sector_animations(t_doom *den);
void	save_map_wall_animations(t_doom *den);
void	save_map_checkpoints(t_doom *den);
void	write_textures(t_doom *den, int zero);
void	set_walls_sectors_ids(t_doom *den);
void	save_map_textures(t_doom *den, int zero);

int     read_check(int fd, void *dst, size_t max_inp);
int     load_map_sector_animations(t_doom *den);
int     load_map_wall_animations(t_doom *den);
int     load_map_checkpoints(t_doom *den);
int     load_map_level(t_doom *den);
int     load_map_sec(t_doom *den);
int     load_map_textures(t_doom *den);
int     connect_map_sectors(t_doom *den);

void	save_map_file(t_doom *den);

#endif
