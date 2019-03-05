#ifndef __DEFINES_H
# define __DEFINES_H

# define WINDOW_W 2000
# define WINDOW_H 1100
# define MAX_SOURCE_SIZE 0x100000
# define TEXTURES_COUNT 38
# define DECOR_TEXTURES_COUNT 2
# define ENEMIES_TEXTURES_COUNT 30
# define PICK_UP_TEXTURES_COUNT 11
# define WEAPONS_TEXTURES_COUNT 19
# define COUNT_ENEMIES 3
# define G1 0.01745329f
# define G90 1.5707963f
# define G180 3.1415926f
# define G360 6.2831852f
# define MOVING_SPEED_PER_HALF_SEC 1000.0f
# define ROTATION_SPEED_PER_HALF_SEC 80.0f
# define MOUSE_SENSIVITY 0.002f
# define MOUSE_UP_DOWN_SENSIVITY 2

# define PLAYER_HEIGHT 400
# define PLAYER_HEAD_SIZE 200
# define MAX_CLIMB_HEIGHT 300
# define JUMP_HEIGHT 1000
//# define PL_COL_SZ 10
# define COLLISION_SIZE 400
# define COLLISION_SIZE2 200
# define BUTTON_PRESS_DIST 1000
# define MENU_COLOR 0x00FF00
# define INCLINED_FC_Z 30
# define MAX_INCLINED_FC_XY 15
# define ANIMATION_TIME_SEC 1
# define COUNT_WALLS_TO_ANIM 10
# define FLY_SPEED	2000.0f

# define FOOTX WINDOW_W - 150
# define FOOTY 120

# define SPRITE_HEIGHT 400

# define WEAPONS_COUNT	3
# define WEAPONS_MOVING_CHANGE_VALUE WINDOW_W / 20

# define INPUT_STRING_LEN 32
# define INPUT_LINE_LEN 200


# define USELESS1 (float)(clock() - iw->v.fly_up)
# define USELESS2 (float)(clock() - iw->v.rot_left)
# define USELESS3 (float)(clock() - iw->v.rot_right)
# define USELESS4 WINDOW_W * sizeof(int)

#ifdef __APPLE__
# define WRITE_MAP O_WRONLY | O_TRUNC
# define READ_MAP O_RDONLY
# define MAX_FPS 250
# define CLKS_P_S (CLOCKS_PER_SEC * 2)
#elif __linux__
# define WRITE_MAP O_WRONLY | O_TRUNC
# define READ_MAP O_RDONLY
# define MAX_FPS 45
# define CLKS_P_S (CLOCKS_PER_SEC / 3)
#else
# define WRITE_MAP O_WRONLY | _O_BINARY | O_TRUNC
# define READ_MAP O_RDONLY | _O_BINARY
# define MAX_FPS 500
# define CLKS_P_S CLOCKS_PER_SEC
#endif

# define HUD_MISS_TIME CLKS_P_S / 2
# define HUD_SAVED_TIME CLKS_P_S

# define ENEMY_HEALTH0 10
# define ENEMY_DAMAGE0 3
# define ENEMY_HEALTH1 20
# define ENEMY_DAMAGE1 10
# define ENEMY_HEALTH2 12
# define ENEMY_DAMAGE2 2

# define JETPACK_TIME CLKS_P_S * 10
# define CHECKPOINT_DIST 2000
# define MAP_SCALE 400

# define MUSIC_COUNT 6
# define ENV_COUNT 22

# define MAX_MUSIC_SIZE 10000000
# define CROUCH_SIZE 200
#endif
