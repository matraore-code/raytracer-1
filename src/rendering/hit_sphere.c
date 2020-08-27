/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_sphere.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/05 01:58:17 by mboivin           #+#    #+#             */
/*   Updated: 2020/08/25 22:58:45 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/*
** This function solves quadratic equation for a sphere
*/

static bool	solve_quadratic_sphere(t_ray *ray, t_vec3 quad_coef)
{
	double	root1;
	double	root2;

	if (get_quad_roots(&root1, &root2, quad_coef) == true)
	{
		if ((root1 <= 0.0 && root2 <= 0.0)
		|| (root1 > ray->t_nearest && root2 > ray->t_nearest))
			return (false);
		if (root2 <= 0.0)
			root2 = root1;
		if ((root1 <= 0.0) || (root1 > root2))
			root1 = root2;
		ray->t_nearest = root1;
		return (true);
	}
	return (false);
}

/*
** This function handles intersection with a sphere
** If a sphere is intersected, t_nearest is updated and true is returned
*/

bool		hit_sphere(t_sphere *sphere, t_ray *ray)
{
	t_vec3	quad_coef;
	t_vec3	dist;

	dist = sub_vec3(ray->origin, sphere->center);
	quad_coef = get_quad_coef(ray->dir, dist, sphere->radius);
	return (solve_quadratic_sphere(ray, quad_coef));
}