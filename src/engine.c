/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasyush <mmasyush@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 10:26:18 by mmasyush          #+#    #+#             */
/*   Updated: 2019/02/14 10:26:19 by mmasyush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/wolf3d.h"

void	dda_alg(t_dda *dda, t_core *core)
{
	while (dda->wall_hitt == 0)
	{
		if (dda->sideX < dda->sideY)
		{
			dda->sideX += dda->deltaX;
			core->ray->mapX += dda->stepSideX;
			dda->s = 0;
		}
		else
		{
			dda->sideY += dda->deltaY;
			core->ray->mapY += dda->stepSideY;
			dda->s = 1;
		}
		if (core->map[core->ray->mapX][core->ray->mapY] > 0)
			dda->wall_hitt = 1;
	}
}

void	rays_calc(t_core *core)
{
	if (core->dda->s == 0)
		core->distance = (core->ray->mapX - core->ray->rayX + \
			(1 - core->dda->stepSideX) / 2) / core->ray->rayDirX;
	else
		core->distance = (core->ray->mapY - core->ray->rayY + \
			(1 - core->dda->stepSideY) / 2) / core->ray->rayDirY;
}

void	game_calc(t_core *core)
{
	core->win_pixX = 0;
	while (core->win_pixX < core->w)
	{
		core->ray->winX = 2 * core->win_pixX / (double)core->w - 1;
		rays_init(core->player, core->ray);
		dda_init(core->dda, core->ray);
		dda_alg(core->dda, core);
		rays_calc(core);
		core->wall_h = (int)(core->h / core->distance);
		core->pStart = -core->wall_h / 2 + core->h / 2;
		if (core->pStart < 0)
			core->pStart = 0;
		core->pEnd = core->wall_h / 2 + core->h / 2;
		if (core->pEnd >= core->h)
			core->pEnd = core->h - 1;
		paint(core);
		core->win_pixX++;
	}
}

void	game_loop(t_core *core)
{
	structs_init(core);
	create_win(core);
	player_init(core->player);
	texture_init(core->texture, core);
	core->keys->exit = 0;
	while (!(core->keys->exit))
	{
		keyses(core);
		game_calc(core);
		display_core(core);
	}
}
