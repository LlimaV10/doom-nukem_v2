#ifndef __GUARDIANS_H
# define __GUARDIANS_H

# define _CRT_SECURE_NO_WARNINGS
//# include "SDL2/SDL2.framework/Headers/SDL.h"
#ifdef __linux__
    # include <SDL2/SDL.h>
    # include <SDL2/SDL_ttf.h>
    # include <SDL2/SDL_mixer.h>
#else
 # include <SDL.h>
 # include <SDL_ttf.h>
 # include <SDL_mixer.h>
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
# include <sys/stat.h>
# define S_IRUSR _S_IREAD
# define S_IWUSR _S_IWRITE
#endif

# include "../libft/libft.h"
# include <fcntl.h>
# include <math.h>
# include <time.h>
# include "struct.h"

void	set_pixel2(SDL_Surface *surface, int x, int y, Uint32 pixel);
int		get_light_color(int color, t_picture *light);
Uint32	get_pixel(t_packaging_texture *sur, const int x, const int y);
Uint32	get_pixel_surface(SDL_Surface *sur, const int x, const int y);
int		in_sec_xy(t_sdl *iw, int sector, int x, int y);
int		in_sec(int sector, t_sdl *iw);
void	exit_x(t_sdl *iw);
void	draw_crosshair(t_sdl *iw);
int		get_sector_by_pointer(t_sdl *iw, t_sector *sec);
int		get_wall_by_pointer(t_sdl *iw, t_sector *sec, t_wall *w);
void	add_sector_animation(t_sdl *iw);
void	do_sector_animation_step(t_sdl *iw, t_sector_animation *a, int dz);
void	draw_selected_walls_to_be_animated(t_sdl *iw);
void	add_wall_to_wall_animation2(t_sdl *iw, int add_wall);
void	add_wall_to_wall_animation(t_sdl *iw);
void	calculate_pictures_list(t_sdl *iw, t_wall *wall, t_picture *p);
void	do_wall_animation_step_dx(t_sdl *iw, t_wall_animation *a, int dx);
void	do_wall_animation_step_dy(t_sdl *iw, t_wall_animation *a, int dy);
void	exit_editing_sector_animation(t_sdl *iw);
void	exit_editing_wall_animation(t_sdl *iw);
void	add_picture(t_sdl *iw, t_wall *wall);
void	delete_wall_animation(t_sdl *iw, t_picture *pic);
void	delete_light_and_animations(t_sdl *iw, t_picture *pic);
void	delete_picture(t_wall *wall, t_picture *pic, t_sdl *iw);
void	draw_text_number(t_sdl *iw, t_draw_info *d, const char *s, int numb);
void	draw_text(t_sdl *iw, const char *s, int x, int y);
void	draw_text_blue(t_sdl *iw, const char *s, int x, int y);
void	draw_some_info(t_sdl *iw);
void	draw_menu_sphere(t_sdl *iw, int a, const char *t);
void	draw_up_down_menu_arrows(t_sdl *iw, int x);
void	draw_menu(t_sdl *iw);
void	ft_scaled_blit(t_packaging_texture *tex, SDL_Surface *winsur, SDL_Rect *rect);
void	ft_scaled_blit2(t_packaging_texture *tex, SDL_Surface *winsur, SDL_Rect *rect);
void	draw_tex_to_select(t_sdl *iw);
void	clear_sprites_tex_to_select(t_sdl *iw);
void	draw_decor_tex_to_select(t_sdl *iw);
void	draw_pickup_tex_to_select(t_sdl *iw);
void	draw_enemies_tex_to_select(t_sdl *iw);
void	draw_selected_tex(t_sdl *iw);
void	draw_selected_sprite(t_sdl *iw);
void	draw_submenu(t_sdl *iw);
void	change_sector_animation_status(t_sdl *iw, t_picture *p);
void	change_wall_animation_status(t_sdl *iw, t_picture *p);
void	drawcircle(t_hud *den, t_draw_circle *c, int x, int y);
void	circle(t_hud *den, int xc, int yc);
void	ft_line2(t_sdl *iw, int x, int y, int color);
void	make_health(t_hud *den, t_sdl *iw);
void	draw_gun(t_sdl *iw);
void	draw_icon_bag(t_sdl *iw);
void	draw_frame(t_sdl *iw, SDL_Surface *winsur, SDL_Rect *rect);
void	drop_item(t_sdl *iw);
void	delete_used_sprite(t_sdl *iw, t_sprite *s);
void	use_item_delete(t_sdl *iw);
void	use_item(t_sdl *iw);
void	add_item(t_sdl *iw);
//void    draw_button(t_sdl	*iw, SDL_Rect	*rect, t_packaging_texture *texture);
void    draw_butttons(t_sdl	*iw, SDL_Rect	*rect);
void	draw_selected_item(t_packaging_texture *tex, SDL_Surface *winsur, SDL_Rect *rect, t_sdl *iw);
void	draw_items(t_sdl *iw);
void	transparency(t_sdl	*iw);
void	draw_input_on_screen(t_sdl *iw, t_keyb_inp *ki);
t_keyb_inp  *input_loop(t_sdl *iw);
void	add_checkpoint(t_sdl *iw, t_sprite *s);
void	draw_checkpoint_text(t_sdl *iw);
void	update(t_sdl *iw);
int		get_picture_dist(t_sdl *iw, t_picture *pic);
t_sprite	*get_card_from_bag(t_sdl *iw, int t_numb);
void	button_f_up_cards(t_sdl *iw);
void	button_f_up(t_sdl *iw);
void	key_up(int code, t_sdl *iw);
void	key_down(int code, t_sdl *iw);
void	key_down_repeat(int code, t_sdl *iw);
void	mouse_move(int xrel, int yrel, t_sdl *iw);
void	rotate_fc(t_sector_fc *fc, int xy, int pl);
void	mouse_buttonleft_up(int x, int y, t_sdl *iw);
void	mouse_buttonright_up(int x, int y, t_sdl *iw);
void	mouse_wheel(SDL_Event *e, t_sdl *iw);
t_wall	*is_wall_portal(t_sdl *iw, int dx, int dy);
void	move_collisions(t_sdl *iw, int dx, int dy, int tmp);
int		check_moving_in_portal_z(t_sdl *iw, int dx, int dy, t_wall *sw);
void	move_in_portal(t_sdl *iw, t_move *d);
void	move(t_sdl *iw, int pl, clock_t *time);
void	get_wall_line2(t_wall *wall);
void	do_sector_animations(t_sdl *iw);
void	do_wall_animations(t_sdl *iw);
int		check_walls_collisions_on_line_segment(t_sdl *iw, int wall, int len);
void	check_walls_collisions(t_sdl *iw);
int		esp_check_walls(t_sdl *iw, t_enemy_sees_player *esp);
int		esp_check_portal(t_sdl *iw, t_enemy_sees_player *esp, int portal);
void	esp_get_new_player_coordinates(t_sdl *iw, t_sector_way *way, t_enemy_sees_player *esp, t_sprite *s);
int		enemy_sees_player(t_sdl *iw, t_sprite *s);
int		move_enemy_in_portal(t_sdl *iw, t_sprite *s, t_intpoint2d *vect);
int		move_enemy(t_sdl *iw, t_sprite *s);
void	sprite_physics(t_sdl *iw, t_sprite *s);
void	enemy_intelligence0(t_sdl *iw, t_sprite *s);
void	enemy_intelligence1(t_sdl *iw, t_sprite *s);
void	enemy_intelligence2(t_sdl *iw, t_sprite *s, int i);
void	check_enemies(t_sdl *iw);
void	draw_miss(t_sdl *iw);
void	draw_save(t_sdl *iw);
void	damaging_enemy(t_sdl *iw, int damage, int max_distance);
void	attack(t_sdl *iw);
void	reload_gun(t_sdl *iw);
void	guns_loop(t_sdl *iw);
void	guns_movements(t_sdl *iw);
void	death(t_sdl *iw);
void	set_checkpoint(t_sdl *iw, t_sprite *s);
void	check_checkpoints(t_sdl *iw);
void	sound_loop(t_sdl *iw);
void	environment_loop(t_sdl *iw);
void	loop(t_sdl *iw);
void	main_loop(t_sdl *iw);
void	get_wall_line(t_sdl *iw, int wall);
void	create_map(t_sdl *iw);
int		get_sector(t_sdl *iw);
void	set_top_bottom(t_sdl *iw);
float	get_k_angle(float rot);
void	get_left_right_lines_points(t_sdl *iw);
void	get_screen_line(t_sdl *iw);
void	get_direction(t_sdl *iw);
void	free_walls(t_sdl *iw);
void	free_pairs(t_sdl *iw);
void	add_wall(t_sdl *iw, t_save_wall *tmp);
int		get_floor_z(t_sdl *iw, int x, int y);
int		get_ceil_z(t_sdl *iw, int x, int y);
int		get_floor_z_sec(t_sdl *iw, int x, int y, int sector);
int		get_ceil_z_sec(t_sdl *iw, int x, int y, int sector);
float	get_vectors_angle(float x1, float y1, float x2, float y2);
void	get_visible_walls2(t_sdl *iw, int wall, float lang);
void	get_visible_walls(t_sdl *iw);
int		cross_two_lines(t_line2d *l1, t_line2d *l2, t_intpoint2d *p);
int		ft_min(int p1, int p2);
int		ft_max(int p1, int p2);
int		point_in_front_and_on_wall(t_sdl *iw, t_intpoint2d *p, int wall);
int		visible_wall(t_sdl *iw, int wall);
int		if_not_in_vw(t_sdl *iw, t_wall *wall);
void	add_lr_wall(t_sdl *iw, t_intpoint2d *p, t_wall *wall, int x);
void	get_all_intersection_line(t_sdl *iw, t_line2d *nl, int right);
void	get_left_right_visible_walls(t_sdl *iw);
t_save_wall	*find_next_vis_wall(t_sdl *iw, t_save_wall *left);
void	put_wall_pixel(t_brez *b, int x, int y);
void	print_brez(t_brez *b, int d, int d1, int d2);
void	brez_line(int *wall_y, t_draw_line line);
void	draw_between_sectors_bot_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int *tmp);
void	draw_between_sectors_top_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int *tmp);
void	draw_floor_ceil_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len);
void	draw_inclined_floor_ceil_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len);
void	draw_wall_floor_ceil_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len);
void	draw_inclined_wall_floor_ceil_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len);
void	draw_between_sectors_walls(t_sdl *iw, t_save_wall *left, t_save_wall *right);
int		*get_between_sectors_walls(t_sdl *iw, t_save_wall *left, t_save_wall *right, int **top);
void	fill_portal(t_sdl *iw, t_save_wall *left, t_save_wall *right, t_sdl *iw2);
void	fill_top_skybox(t_sdl *iw2, t_save_wall *left, int len);
void	fill_bot_by_wallTop(t_sdl *iw, t_save_wall *left, int len);
void	fill_tb_by_slsr(t_sdl *iw);
void	change_saved_top_bot_between_lines(t_sdl *iw, int len);
void	draw_glass_tex(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len);
int		sector_visited(t_sdl *iw, int sec);
void	draw_glass_sprites(t_sdl *iw);
void	draw_next_sector(t_sdl *iw, t_save_wall *left, t_save_wall *right);
void	draw_next_sector_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len);
int		draw_picture(t_sdl *iw, t_picture *pic);
void	draw_pictures(t_sdl *iw, t_save_wall *left);
void	draw_all(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len);
void	draw_all_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len);
void	draw_left_right(t_sdl *iw, t_save_wall *left, t_save_wall *right);
void	add_pair(t_sdl *iw, t_save_wall *left, t_save_wall *right);
t_save_wall_pairs	*get_closest_between_pair(t_save_wall_pairs	*pair);
int		check_look_picture(t_sdl *iw);
void	new_sort_pairs(t_sdl *iw);
void	draw_skybox(t_sdl *iw);
void	draw_sprite(t_sdl *iw, t_sprite *sprite);
void	draw_sprites(t_sdl *iw);
int		find_point(t_save_wall_pairs *tmp, t_sprite *tmp1);
void	calculate_sprites_once(t_sdl *iw);
void	get_sprites_top_bottom(t_sdl *iw, t_save_wall_pairs	*tmp);
void	switch_nexts_sprites(t_sprite *s1, t_sprite *s2);
void	sort_sprites(t_sdl *iw);
void	save_walls(t_sdl *iw);
void	draw_start(t_sdl *iw);
void	draw(t_sdl *iw);
void	read_textures(t_sdl *iw);
void	read_sprites_textures(t_sdl *iw);
void	read_weapons_textures(t_sdl *iw);
void	get_birth_def(t_sdl *iw);
void	get_def_new(t_sdl *iw);
void	get_kernel_mem(t_sdl *iw);
void	calculate_not_squared_wall_picture(t_wall *wall, t_picture *pic);
void	calculate_picture(t_sdl *iw, t_wall *wall, t_picture *pic);
void	add_picture1(t_sdl *iw);
void	free_way(t_sector_ways **way);
int		sector_in_way(t_sdl *iw, t_sector_ways *way, int sector);
void	add_portal_in_way2(t_sector_ways *tmp, int portal);
t_sector_ways	*add_portal_in_way(t_sector_ways *current_way, int portal);
void	go_in_sector_way(t_sdl *iw, t_get_sectors_ways *g, t_sector_ways *current_way);
void	get_sectors_ways2(t_sdl *iw);
void	get_sectors_ways(t_sdl *iw);
void	get_packaging_texture(t_packaging_texture **pack, SDL_Surface *sur);
void	get_packaging_textures(t_sdl *iw);
void	get_sprite_texture(t_sdl *iw, t_sprite *s);
void	set_sprites_z(t_sdl *iw);
void	get_guns_center(t_sdl *iw, int i, int scale);
void	get_guns_center_down(t_sdl *iw, int i, int scale);
void	get_guns(t_sdl *iw);
void	game_start_menu(t_sdl *iw);
void	menu_loop(t_sdl *iw);
void	image_loop(t_sdl *iw, t_packaging_texture *tex);
void	free_sector_ways(t_sdl *iw);
void	undo_wall_animation(t_sdl *iw, t_picture *pic);
void	undo_animations2(t_sdl *iw, t_picture *pic);
void	undo_animations(t_sdl *iw);
void	get_music(t_sdl *iw, int i, const char *file);
void	get_sounds_game(t_sdl *iw);
void	get_sounds(t_sdl *iw);

void	get_kernels(t_sdl *iw);
void	load_kernel(t_kernel *k, t_sdl *iw);
void	draw_inclined_wall_floor_ceil_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len);
void	draw_wall_floor_ceil_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len);
void	draw_inclined_floor_ceil_betw_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len);
void	draw_floor_ceil_betw_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len);
void	draw_skybox_kernel(t_sdl *iw);
void	draw_glass_tex_kernel(t_sdl *iw, t_save_wall *left, t_save_wall *right, int len);
int		draw_picture_kernel(t_sdl *iw, t_picture *pic);
void	draw_pictures_kernel(t_sdl *iw, t_save_wall *left);
void	draw_pictures_kernel(t_sdl *iw, t_save_wall *left);
void	draw_sprites_kernel(t_sdl *iw);
void	draw_glass_sprites_kernel(t_sdl *iw);
void	draw_gun_kernel(t_sdl *iw);

int		check_all_validation(t_sdl *iw);



#endif
