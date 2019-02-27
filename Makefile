# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/06/16 16:42:45 by dbolilyi          #+#    #+#              #
#    Updated: 2018/09/23 13:17:55 by dbolilyi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = doom-nukem

SRC =	3d/main3d.c \
				3d/CLrender.c \
				3d/minimap.c \
				3d/input_validation.c \
				main.c \
				save_list.c \
				display.c \
				draw_pixels.c \
				draw_line.c \
				map_redactor.c \
				save_new_list.c \
				write_list.c \
				find_walls.c \
				sector_in_sector.c \
				delete.c \
				sort_sector.c \
				sprite.c \
				delete_memory.c \
				portal.c \
				player.c \
				sector.c \
				check_option.c \
				walls.c \
				save.c \
				load.c

OBJ = $(SRC:.c=.o)

#INC =	-I./SDL2/SDL2.framework/Headers\
#		-I./SDL2/SDL2_ttf.framework/Headers\

ERRFL = -Wall -Wextra -g # -Werror

FL = `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_mixer -lOpenCL -lm #-F SDL2/ -framework SDL2 -framework SDL2_ttf -framework OpenCL

all: $(NAME)

$(OBJ): %.o: %.c
	@echo "\033[93mCompiling \033[0m$@"
	@cc $(ERRFL) -c -o $@ $<

$(NAME): $(OBJ)
	@echo "\033[93mCompiling libft\033[0m"
	@make -C ./libft
	@echo "\033[93mCompiling $(NAME)\033[0m"
	@gcc $(ERRFL) -o $@ $(OBJ) -L libft/ ./libft/libft.a $(FL)
	@echo "\033[92mDONE\033[0m"
	

clean:
	@echo "\033[91mDeleting libft objects\033[0m"
	@make -C libft/ clean
	@echo "\033[91mDeleting $(NAME) objects\033[0m"
	@rm -f $(OBJ)

fclean: clean
	@echo "\033[91mDeleting libft.a\033[0m"
	@make -C libft/ fclean
	@echo "\033[91mDeleting $(NAME)\033[0m"
	@rm -f $(NAME)

re: fclean \
	all

.PHONY: all clean fclean re

.NOTPARALLEL: all clean fclean re
