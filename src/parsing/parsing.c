/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/22 15:01:06 by mboivin           #+#    #+#             */
/*   Updated: 2020/10/25 22:41:55 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include "libft_ctype.h"
#include "libft_str.h"
#include "minirt.h"

/*
** Struct: Element identifiers associated to parsing functions
*/

static t_parsers	g_parsers[] =
{
	{ SPHERE_ID, ID_OBJ_LEN, &get_sphere },
	{ PLANE_ID, ID_OBJ_LEN, &get_plane },
	{ SQUARE_ID, ID_OBJ_LEN, &get_square },
	{ CYLINDER_ID, ID_OBJ_LEN, &get_cylinder },
	{ TRIANGLE_ID, ID_OBJ_LEN, &get_triangle },
	{ RES_ID, ID_LEN, &get_resolution },
	{ AMBIENT_ID, ID_LEN, &get_ambient },
	{ CAM_ID, ID_LEN, &get_camera },
	{ LIGHT_ID, ID_LEN, &get_light }
};

/*
** This function calls the appropriate function to parse a line
*/

static void		handle_scene_elem(t_minirt *env, char **input)
{
	int			i;

	i = 0;
	while (i < MAX_PARS_FUNC)
	{
		if (!ft_strncmp(g_parsers[i].elem_id, *input, g_parsers[i].id_len))
		{
			(*g_parsers[i].func)(env, input);
			return ;
		}
		i++;
	}
	exit_error(env, ID_ERR);
}

/*
** This function gets the entire scene description
*/

static char		*read_scene_file(t_minirt *env, const char *filepath)
{
	char		*result;
	char		buffer[BUFFER_SIZE + 1];
	int			fd;
	int			bytes_read;

	result = NULL;
	fd = open(filepath, O_RDONLY | O_NOFOLLOW);
	if (!fd)
		exit_error(env, ERRNO_TO_STR);
	while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
	{
		buffer[bytes_read] = '\0';
		result = ft_strjoindelone(result, buffer);
		if (!result)
			exit_error(env, ERRNO_TO_STR);
	}
	close(fd);
	return (result);
}

/*
** This function checks whether the scene description is valid
*/

static void		check_scene(t_minirt *env)
{
	if (!env->res.is_declared)
		exit_error(env, NO_RES);
	if (!env->ambient.is_declared)
		exit_error(env, NO_AMB);
	if (!env->cams)
		exit_error(env, NO_CAM);
	if (!env->lights)
		exit_error(env, NO_LIGHT);
	if (!env->objs)
		exit_error(env, NO_OBJ);
}

/*
** This function iterates over the input to call functions
*/

void			parse_scene(t_minirt *env, const char *filepath)
{
	const char	*ids = "RAclspt";
	char		*input;
	char		*head;

	input = read_scene_file(env, filepath);
	if (!input)
		exit_error(env, ERRNO_TO_STR);
	head = input;
	while (*input)
	{
		if (*input == NEWLINE)
			input++;
		else if (ft_ischarset(*input, ids))
			handle_scene_elem(env, &input);
		else
			exit_error(env, SCENE_FMT);
	}
	ft_strdel(&head);
	check_scene(env);
	check_max_display(env);
}
