/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/21 21:32:12 by mboivin           #+#    #+#             */
/*   Updated: 2020/07/25 00:47:29 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/*
** Render image
**
** generate_image() :  Generate the rendered image
*/

void	generate_image(t_scene *scene)
{
	g_app->img = malloc_image(g_app->win_x, g_app->win_y);
	fill_image(create_color(0, 0, 255)); // Tmp test
	if (scene->res.size_x < 400 || scene->res.size_y < 400)
		put_warn_res();
}