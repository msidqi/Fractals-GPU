/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_events.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msidqi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/02 03:13:04 by msidqi            #+#    #+#             */
/*   Updated: 2020/01/16 10:37:29 by msidqi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgl.h"

int			mouse_move(int x, int y, t_data *data)
{
	ft_refresh_image(data);
	if (!data->islocked)
	{
		data->juliapos.x = (double)(x * 4.5) / data->winwidth - 2.6;
		data->juliapos.y = (double)(y * 4.5) / data->winheight - 2.6;
	}
	data->winpos.x = (double)x;
	data->winpos.y = (double)y;
	gpu_frac(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img_ptr, 0, 0);
	return (0);
}

static void	mouse_press_scroll(int button, t_data *data)
{
	double	oldzoom;

	oldzoom = data->zoom;
	if (button == SCROLL_UP)
	{
		data->max_iter += 3;
		data->zoom *= 1.3;
		data->movex += (data->winpos.x - data->winwidth / 2) / data->zoom
					- (data->winpos.x - data->winwidth / 2) / (oldzoom);
		data->movey += (data->winpos.y - data->winheight / 2) / data->zoom
					- (data->winpos.y - data->winheight / 2) / (oldzoom);
	}
	else if (button == SCROLL_DOWN)
	{
		data->max_iter -= 3;
		if (data->zoom <= 1)
			data->zoom = 1;
		else
		{
			data->zoom *= 0.7;
			data->movex -= -(data->winpos.x - data->winwidth / 2) / data->zoom
						+ (data->winpos.x - data->winwidth / 2) / (oldzoom);
			data->movey -= -(data->winpos.y - data->winheight / 2) / data->zoom
						+ (data->winpos.y - data->winheight / 2) / (oldzoom);
		}
	}
}

int			mouse_press(int button, int x, int y, t_data *data)
{
	(void)x;
	(void)y;
	ft_refresh_image(data);
	if (button == LEFT_CLICK)
		data->max_iter += 10;
	else if (button == RIGHT_CLICK)
		data->max_iter -= 10;
	mouse_press_scroll(button, data);
	gpu_frac(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img_ptr, 0, 0);
	return (0);
}

void		ft_init_sierpinski(t_data *data, t_point *points)
{
	points[0].x = data->winwidth / 2;
	points[0].y = 0;
	points[1].x = 0;
	points[1].y = data->winheight;
	points[2].x = data->winheight;
	points[2].y = data->winheight;
	points[3].x = points[0].x;
	points[3].y = data->winheight / 2;
}
