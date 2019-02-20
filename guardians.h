#ifndef __GUARDIANS_H
# define __GUARDIANS_H

# define _CRT_SECURE_NO_WARNINGS
//# include "SDL2/SDL2.framework/Headers/SDL.h"
#ifdef __linux__
    # include <SDL2/SDL.h>
    # include <SDL2/SDL_ttf.h>
#else
    # include <SDL.h>
    # include <SDL_ttf.h>
#endif

#ifdef __APPLE__
# include <OpenCL/cl.h>
# include <unistd.h>
#elif __linux__
# include <CL/cl.h>
# include <unistd.h>
#else
# include <CL/cl.h>
# include <io.h>
# define open _open
# define close _close
# define read _read
# define write _write
# define HAVE_STRUCT_TIMESPEC
#endif

# include "libft/libft.h"
# include <fcntl.h>
// # include <pthread.h>
# include <math.h>
# include <time.h>
# include "struct.h"

void	get_map(t_sdl *iw);
void	get_map2(t_sdl *iw);
//void	get_animation(t_sdl *iw);
void	load_kernel(t_kernel *k, t_sdl *iw);
int		get_ceil_z(t_sdl *iw, int x, int y);
int		get_floor_z(t_sdl *iw, int x, int y);
void	draw_inclined_wall_floor_ceil_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len);
void	draw_wall_floor_ceil_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len);
void	draw_inclined_floor_ceil_betw_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len);
void	draw_floor_ceil_betw_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len);
//void	draw_inclined_floor_ceil_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len);
//void	draw_floor_ceil_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len);
void	draw_skybox_kernel(t_sdl *iw);
void	draw_glass_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len);
void	draw_pictures_kernel(t_sdl *iw, t_save_wall *left);
float	get_vectors_angle(float x1, float y1, float x2, float y2);
void	calculate_picture(t_sdl *iw, t_wall *wall, t_picture *pic);
void	draw_sprites_kernel(t_sdl *iw);
void	draw_submenu(t_sdl *iw);
void	calculate_picture(t_sdl *iw, t_wall *wall, t_picture *pic);
void	get_wall_line2(t_wall *wall);
void	draw_sprite(t_sdl *iw, t_sprite *sprite);
void	draw_glass_sprites_kernel(t_sdl *iw);
void	sort_sprites(t_sdl *iw);
void	draw_gun_kernel(t_sdl *iw);
int		get_ceil_z_sec(t_sdl *iw, int x, int y, int sector);
int		get_floor_z_sec(t_sdl *iw, int x, int y, int sector);
void	get_kernels(t_sdl *iw);

void add_sprite(t_sdl *iw, int x, int y, int z, int t, int num, int type, float scale);///// delete

int		ft_max(int p1, int p2);
int		ft_min(int p1, int p2);
void	set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

void	print_brez_m(t_brez *b, int d, int d1, int d2);
void	bresen(t_sdl *iw, t_draw_line line, int color);
void    draw_minimap(t_sdl *iw);
void	ft_scaled_blit(t_packaging_texture *tex, SDL_Surface *winsur, SDL_Rect *rect);
void	draw_miss(t_sdl *iw);
void	image_loop(t_sdl *iw, t_packaging_texture *tex);
void	get_birth_def(t_sdl *iw);

#endif
