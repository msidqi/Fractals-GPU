/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_keyboard_events.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msidqi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/02 04:38:36 by msidqi            #+#    #+#             */
/*   Updated: 2019/05/02 04:38:38 by msidqi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgl.h"

static void		key_press_color(int key_code, t_data *data)
{
	static int c = 0;
	if (key_code == 8)
	{
		if (c == 0)
		{
			data->color.red = 235;
			data->color.green = 206;
			data->color.blue = 135;
			c++;
		}
		else if (c == 1)
		{
			data->color.red = 206;
			data->color.green = 135;
			data->color.blue = 235;
			c++;
		}
		else
		{
			data->color.red = 135;
			data->color.green = 235;
			data->color.blue = 206;
			c = 0;
		}
	}
}

static void		key_press_set(int key_code, t_data *data)
{
	if (key_code == 1)
	{
		if (data->set == 'm')
			data->set = 'j';
		else if (data->set == 'j')
			data->set = 'b';
		else if (data->set == 'b')
			data->set = 't';
		else
			data->set = 'm';
	}
}

int				key_press(int key_code, t_data *data)
{
	if (key_code == 53)
	{
		mlx_destroy_image(data->mlx, data->img_ptr);
		free(data->results);
		clReleaseMemObject(data->output);
		clReleaseProgram(data->program);
		clReleaseKernel(data->kernel);
		clReleaseCommandQueue(data->commands);
		clReleaseContext(data->context);
		exit(0);
	}
	ft_refresh_image(data);
	if (key_code == LEFT_ARROW)
		data->movex += -data->winwidth / 10 / (data->zoom);
	else if (key_code == RIGHT_ARROW)
		data->movex -= -data->winwidth / 10 / (data->zoom);
	if (key_code == UP_ARROW)
		data->movey += -data->winheight / 10 / (data->zoom);
	else if (key_code == DOWN_ARROW)
		data->movey -= -data->winheight / 10 / (data->zoom);
	key_press_set(key_code, data);
	key_press_color(key_code, data);
	if (key_code == 37)
		data->islocked = data->islocked ? false : true;
	if (key_code == 15)
		ft_init_fractal(data);
	// ft_fractal_choice(data);
	gpu_frac(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img_ptr, 0, 0);
	return (0);
}
