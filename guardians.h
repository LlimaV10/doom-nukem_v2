#ifndef __GUARDIANS_H
# define __GUARDIANS_H

// Visual C
# define _CRT_SECURE_NO_WARNINGS
# include <SDL.h>
# include <CL/cl.h>
# include <io.h>
# define open _open
# define close _close
# define read _read
# define write _write
# define HAVE_STRUCT_TIMESPEC
// Visual C
// Unix
//# include <unistd.h>
//# include "SDL2/SDL2.framework/Headers/SDL.h"
// Unix
# include "libft/libft.h"
# include <fcntl.h>
// # include <pthread.h>
# include <math.h>
# include "struct.h"

void	get_map(t_sdl *iw);

#endif
