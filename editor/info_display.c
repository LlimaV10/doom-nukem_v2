/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   info_display.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddehtyar <ddehtyar@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 17:05:35 by ddehtyar          #+#    #+#             */
/*   Updated: 2019/02/28 17:05:36 by ddehtyar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nuken.h"

void	info_display_three(t_doom *den, char *infor, SDL_Color color_text)
{
	den->rect.x = 1920;
	den->rect.y = 94;
	den->TTF_TextSolid = TTF_RenderText_Blended_Wrapped(den->font,
		den->blouk == 1 ? "YES" : "NO", color_text, 200);
	SDL_BlitSurface(den->TTF_TextSolid, NULL, den->bmp, &den->rect);
	SDL_FreeSurface(den->TTF_TextSolid);
	if (den->change == 1 || den->change == 2)
		change_wall(den, color_text);
	free(infor);
	board_display(den);
	SDL_UpdateWindowSurface(den->window);
}

void	info_display_two(t_doom *den, char *infor, SDL_Color color_text)
{
	SDL_BlitSurface(den->TTF_TextSolid, NULL, den->bmp, &den->rect);
	SDL_FreeSurface(den->TTF_TextSolid);
	free(infor);
	den->rect.x = 1860;
	den->rect.y = 37;
	infor = ft_itoa(den->walls + 1);
	den->TTF_TextSolid = TTF_RenderText_Blended_Wrapped(den->font,
		infor, color_text, 200);
	SDL_BlitSurface(den->TTF_TextSolid, NULL, den->bmp, &den->rect);
	SDL_FreeSurface(den->TTF_TextSolid);
	free(infor);
	den->rect.x = 1860;
	den->rect.y = 67;
	sum_sprite(den);
	infor = ft_itoa(den->sprites);
	den->TTF_TextSolid = TTF_RenderText_Blended_Wrapped(den->font,
		infor, color_text, 200);
	SDL_BlitSurface(den->TTF_TextSolid, NULL, den->bmp, &den->rect);
	SDL_FreeSurface(den->TTF_TextSolid);
	info_display_three(den, infor, color_text);
}

void	info_display(t_doom *den)
{
	char		*cmp;
	char		*infor;
	SDL_Color	color_text;

	color_text.r = 255;
	color_text.g = 255;
	color_text.b = 255;
	den->rect.x = 1770;
	den->rect.y = 10;
	den->rect.h = 50;
	den->rect.w = 30;
	cmp = "sectors:\nwalls:\nsprites:\nclosed sector:\ndedicated wall\n"
	"start_x:\nstar_y:\nfifnish_x:\nfinish_y:\nsector: ";
	den->TTF_TextSolid = TTF_RenderText_Blended_Wrapped(den->font, cmp,
	color_text, 200);
	SDL_BlitSurface(den->TTF_TextSolid, NULL, den->bmp, &den->rect);
	SDL_FreeSurface(den->TTF_TextSolid);
	den->rect.x = 1860;
	den->rect.y = 10;
	infor = ft_itoa(den->sec);
	den->TTF_TextSolid = TTF_RenderText_Blended_Wrapped(den->font, infor,
	color_text, 200);
	info_display_two(den, infor, color_text);
}

void	change_wall_two(t_doom *den, SDL_Color ct, char *in)
{
	den->rect.y = 204;
	in = ft_itoa(den->find_tmp->x1);
	den->TTF_TextSolid = TTF_RenderText_Blended_Wrapped(den->font, in, ct, 200);
	SDL_BlitSurface(den->TTF_TextSolid, NULL, den->bmp, &den->rect);
	SDL_FreeSurface(den->TTF_TextSolid);
	free(in);
	den->rect.y = 231;
	in = ft_itoa(den->find_tmp->y1);
	den->TTF_TextSolid = TTF_RenderText_Blended_Wrapped(den->font, in, ct, 200);
	SDL_BlitSurface(den->TTF_TextSolid, NULL, den->bmp, &den->rect);
	SDL_FreeSurface(den->TTF_TextSolid);
	free(in);
	den->rect.y = 258;
	in = ft_itoa(den->find_tmp->sec);
	den->TTF_TextSolid = TTF_RenderText_Blended_Wrapped(den->font, in, ct, 200);
	SDL_BlitSurface(den->TTF_TextSolid, NULL, den->bmp, &den->rect);
	SDL_FreeSurface(den->TTF_TextSolid);
	free(in);
}

void	change_wall(t_doom *den, SDL_Color ct)
{
	char	*in;

	den->rect.x = 1880;
	den->rect.y = 150;
	den->rect.h = 50;
	den->rect.w = 30;
	in = ft_itoa(den->find_tmp->x);
	den->TTF_TextSolid = TTF_RenderText_Blended_Wrapped(den->font, in, ct, 200);
	SDL_BlitSurface(den->TTF_TextSolid, NULL, den->bmp, &den->rect);
	SDL_FreeSurface(den->TTF_TextSolid);
	free(in);
	den->rect.y = 177;
	in = ft_itoa(den->find_tmp->y);
	den->TTF_TextSolid = TTF_RenderText_Blended_Wrapped(den->font, in, ct, 200);
	SDL_BlitSurface(den->TTF_TextSolid, NULL, den->bmp, &den->rect);
	SDL_FreeSurface(den->TTF_TextSolid);
	free(in);
	change_wall_two(den, ct, in);
}
