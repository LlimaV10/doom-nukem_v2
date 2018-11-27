#ifndef __GUARDIANS_H
# define __GUARDIANS_H

# define _CRT_SECURE_NO_WARNINGS
//# include "SDL2/SDL2.framework/Headers/SDL.h"
# include <SDL.h>
// Visual C -------
// # include <CL/cl.h>
// # include <io.h>
// # define open _open
// # define close _close
// # define read _read
// # define write _write
// # define HAVE_STRUCT_TIMESPEC
// Visual C -------
// Unix -----
# include <OpenCL/cl.h>
# include <unistd.h>
// Unix -----
# include "libft/libft.h"
# include <fcntl.h>
// # include <pthread.h>
# include <math.h>
# include "struct.h"

void	get_map(t_sdl *iw);
void	get_map2(t_sdl *iw);
void	load_kernel(t_kernel *k);
int		get_ceil_z(t_sdl *iw, int x, int y);
int		get_floor_z(t_sdl *iw, int x, int y);
void	draw_inclined_wall_floor_ceil_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len);
void	draw_wall_floor_ceil_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len);
void	draw_inclined_floor_ceil_betw_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len);
void	draw_floor_ceil_betw_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len);
void	draw_inclined_floor_ceil_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len);
void	draw_floor_ceil_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len);

#endif
