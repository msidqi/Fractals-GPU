/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_event_system.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msidqi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/02 03:12:45 by msidqi            #+#    #+#             */
/*   Updated: 2019/05/02 03:12:46 by msidqi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgl.h"

static int	red_button(t_data *data)
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

void		ft_refresh_image(t_data *data)
{
	mlx_destroy_image(data->mlx, data->img_ptr);
	data->img_ptr = mlx_new_image(data->mlx, data->winwidth, data->winheight);
	data->image = (int *)mlx_get_data_addr(data->img_ptr,
								&data->bpp, &data->size_line, &data->endian);
}

void		ft_mlx_hooks(t_data *data)
{
	mlx_hook(data->win, MOUSE_MOVE_CODE, 0, &mouse_move, (void *)data);
	mlx_hook(data->win, KEY_PRESS_CODE, 0, &key_press, (void *)data);
	mlx_hook(data->win, MOUSE_PRESS_CODE, 0, &mouse_press, (void *)data);
	mlx_hook(data->win, RED_BUTTON_CODE, 0, &red_button, (void *)data);
	mlx_loop(data->mlx);
}
