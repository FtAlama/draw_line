/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alama <alama@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 15:00:13 by alama             #+#    #+#             */
/*   Updated: 2024/06/03 13:02:14 by alama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minilibx/mlx.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct s_data
{
	void	*mlx;
	void	*win;
	void	*img;
	int		x;
	int 	y;
}	t_data;

int	out_prog(t_data *param)
{
	int	x;
	int	y;

	mlx_clear_window(param->mlx, param->win);
	mlx_destroy_window(param->mlx, param->win);
	free(param->mlx);
	param->mlx = NULL;
	param->win = NULL;
	free(param);
	param = NULL;
	if (!param)
		exit(EXIT_SUCCESS);
	else
		exit(EXIT_FAILURE);
	return (0);
}

int	draw_line(void *mlx, void *win, double  begin_x, double begin_y, int end_x, int end_y, int color)
{
	double	delta_x;
	double	delta_y;
	int		pixels;

	delta_x = end_x - begin_x;
	delta_y = end_y - begin_y;
	pixels = sqrt((delta_x * delta_x) + (delta_y * delta_y));
	delta_x /= pixels;
	delta_y /= pixels;
	while (pixels)
	{
		mlx_pixel_put(mlx, win, begin_x, begin_y, color);
		begin_x += delta_x;
		begin_y += delta_y;
		--pixels;
	}
	return (0);
}

int	key_event(int keycode, t_data *param)
{
	if (keycode == 53)
		out_prog(param);
	//draw_line(param->mlx, param->win, 640, 360, 0, 0, 0xFFFFFF);
	return (1);
}

int	mouse_event(int button, int x, int y, t_data *param)
{
	printf("%d\n", button);
	printf("%d %d\n", x, y);
	if (param->x == 0 && button == 1)
	{
		param->x = x;
		param->y = y;
	}
	else if (param->x != 0 && button == 1)
	{
		draw_line(param->mlx, param->win, param->x, param->y, x, y, 0xFFFFFF);
		param->x = 0;
	}
	return (1);
}

int	main(void)
{
	t_data	*data;
	int		pixel_bits;
	int		line_bytes;
	int		endian;
	char	*buffer;
	int		color;
	int		x;
	int		y;
	int		pixel;

	color = 0xABCDEF;
	data = malloc(sizeof(t_data));
	data->mlx = mlx_init();
	data->win = mlx_new_window(data->mlx, 640, 360, "test");
	data->img = mlx_new_image(data->mlx, 640, 360);
	buffer = mlx_get_data_addr(data->img, &pixel_bits, &line_bytes, &endian);
	if (pixel_bits != 32)
		color = mlx_get_color_value(data->mlx, color);
	y = 0;
	while (y < 360)
	{
		x = 0;
		while (x < 640)
		{
			pixel = (y * line_bytes) + (x * 4);
			if (endian == 1)
			{
				buffer[pixel + 0] = (color >> 24);
				buffer[pixel + 1] = (color >> 16) & 0xFF;
				buffer[pixel + 2] = (color >> 8) & 0xFF;
				buffer[pixel + 3] = (color) & 0xFF;
			}
			else if (endian == 0)
			{
				buffer[pixel + 0] = (color) & 0xFF;
				buffer[pixel + 1] = (color >> 8) & 0xFF;
				buffer[pixel + 2] = (color >> 16) & 0xFF;
				buffer[pixel + 3] = (color >> 24);
			}
			x++;
		}
		y++;
	}
	data->x = 0;
	data->y = 0;
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	mlx_mouse_hook(data->win, &mouse_event, data);
	mlx_key_hook(data->win, &key_event, data);
	mlx_hook(data->win, 17, 0, &out_prog, data);
	mlx_hook(data->win, 5, 0, &mouse_event, data);
	mlx_loop(data->mlx);
	return (0);
}
