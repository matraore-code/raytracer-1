/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_bmp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/20 13:16:21 by mboivin           #+#    #+#             */
/*   Updated: 2020/10/07 20:40:57 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/*
** This function fills the file header structure
*/

static t_bmp_h	create_bmpfileheader(int size)
{
	t_bmp_h		result;

	ft_bzero(&result, sizeof(t_bmp_h));
	result.bmp_type[0] = 'B';
	result.bmp_type[1] = 'M';
	result.file_size = HEADER_BYTES + size;
	result.reserved1 = UNUSED;
	result.reserved2 = UNUSED;
	result.offset = HEADER_BYTES;
	return (result);
}

/*
** This function fills the DIB header structure
*/

static t_dib_h	create_bmpdibheader(t_minirt *env, int size)
{
	t_dib_h		result;
	int			ppm;

	ppm = DEFAULT_DPI * PPM_CONV_FACTOR;
	ft_bzero(&result, sizeof(t_dib_h));
	result.size_header = sizeof(t_dib_h);
	result.width = env->res.size_x;
	result.height = env->res.size_y;
	result.planes = DEFAULT_BIPLANES;
	result.bit_count = TRUE_COLOR;
	result.compr = UNUSED;
	result.img_size = HEADER_BYTES + size;
	result.ppm_x = ppm;
	result.ppm_y = ppm;
	result.clr_used = UNUSED;
	result.clr_important = UNUSED;
	return (result);
}

/*
** This function writes headers to the file
*/

static void		write_bmpheaders(t_minirt *env, int fd)
{
	t_bmp_h		file_header;
	t_dib_h		dib_header;
	int			size;

	size = env->res.size_x * env->res.size_y * RGB_LEN;
	file_header = create_bmpfileheader(size);
	dib_header = create_bmpdibheader(env, size);
	write(fd, &(file_header.bmp_type), 2);
	write(fd, &(file_header.file_size), 4);
	write(fd, &(file_header.reserved1), 2);
	write(fd, &(file_header.reserved2), 2);
	write(fd, &(file_header.offset), 4);
	write(fd, &(dib_header.size_header), 4);
	write(fd, &(dib_header.width), 4);
	write(fd, &(dib_header.height), 4);
	write(fd, &(dib_header.planes), 2);
	write(fd, &(dib_header.bit_count), 2);
	write(fd, &(dib_header.compr), 4);
	write(fd, &(dib_header.img_size), 4);
	write(fd, &(dib_header.ppm_x), 4);
	write(fd, &(dib_header.ppm_y), 4);
	write(fd, &(dib_header.clr_used), 4);
	write(fd, &(dib_header.clr_important), 4);
}

/*
** This function writes pixels to the file
*/

static void		write_bmpdata(t_minirt *env, int fd)
{
	int			x;
	int			y;
	int			*pixel;
	int			i;
	int			progress;

	y = env->res.size_y - 1;
	while (y > -1)
	{
		x = 0;
		while (x < env->res.size_x)
		{
			i = (x + env->res.size_x * y) * PIXEL_LEN;
			pixel = (int *)(env->imgs->pixels + i);
			if (write(fd, pixel, RGB_LEN) < 0)
				exit_error(env, DEFAULT_ERR);
			x++;
		}
		progress = ft_percent((env->res.size_y - y), env->res.size_y);
		ft_printf("\rSaving rendered image in BMP format... %d%%", progress);
		y--;
	}
	ft_printf("\nImage saved as 'minirt.bmp' in working dir.\n");
}

/*
** This function carries out file saving to BMP format
*/

void			save_bmp(t_minirt *env, const char *filename)
{
	int			fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, FILE_PERMISSIONS);
	if (!fd)
		exit_error(env, DEFAULT_ERR);
	write_bmpheaders(env, fd);
	write_bmpdata(env, fd);
	close(fd);
	exit_success(env);
}
