/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libgl.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msidqi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/22 21:43:22 by msidqi            #+#    #+#             */
/*   Updated: 2018/11/24 04:59:23 by msidqi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBGL_H
# define LIBGL_H

# include <mlx.h>
# include <stdlib.h>
# include <math.h>
# include <stdio.h>
# include <fcntl.h>
# include <pthread.h>
# include <time.h>

# include <string.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <OpenCL/opencl.h>

# define RIGHT_ARROW 124
# define LEFT_ARROW 123
# define UP_ARROW 126
# define DOWN_ARROW 125
# define ZOOM_OUT 78
# define ZOOM_IN 69
# define SPACE 49
# define LEFT_CLICK 1
# define RIGHT_CLICK 2
# define SCROLL_UP 4
# define SCROLL_DOWN 5
# define MOUSE_PRESS_CODE 4
# define MOUSE_MOVE_CODE 6
# define KEY_PRESS_CODE 2
# define RED_BUTTON_CODE 17
# define MAX_ITER 30
# define ABS(Value) (Value >= 0) ? (Value) : -(Value)

typedef struct		s_point
{
	double			x;
	double			y;
}					t_point;

typedef struct		s_startend
{
	t_point			start;
	t_point			end;
}					t_startend;

typedef struct		s_color
{
	unsigned char	red;
	unsigned char	green;
	unsigned char	blue;
}					t_color;

// typedef enum		e_bool
// {
// 	false, true
// }					t_bool;

typedef	struct		s_data
{//gpu related vars
 	cl_device_id device_id;             // compute device id 
    cl_context context;                 // compute context
    cl_command_queue commands;          // compute command queue
    cl_program program;                 // compute program
    cl_kernel kernel;                   // compute kernel
    
	cl_mem			output;
	int				*results; 



	t_point			juliapos;
	t_point			winpos;
	t_color			color;
	t_startend		thread_range;
	int				islocked;
	int				winwidth;
	int				winheight;
	void			*mlx;
	void			*win;
	void			*img_ptr;
	int				*image;
	double			zoom;
	double			movex;
	double			movey;
	char			set;
	int				max_iter;
	int				endian;
	int				size_line;
	int				bpp;
}					t_data;

int					gpu_frac(t_data *data);
void				*draw_fractal(void *param);
void				ft_fractal_choice(t_data *data);
int					ft_window_setup(t_data *data, char *win_name,
											int winheight, int winwidth);
int					ft_image_setup(t_data *data);
void				ft_palette_rotation(t_data *data);
void				ft_image_fill(t_data *data, int x, int y, int color);
double				lerp(double x1, double x2, double lerp);
double				ft_get_world_pos(double screen_coord,
								double widthheight, double zoom);
void				ft_fractal_choice(t_data *data);
void				*draw_fractal(void *param);
void				ft_refresh_image(t_data *data);
void				ft_mlx_hooks(t_data *data);
int					key_press(int key_code, t_data *data);
int					mouse_press(int button, int x, int y, t_data *data);
int					mouse_move(int x, int y, t_data *data);
int					mandelbrot_set(t_data *data, double c_x, double c_y);
void				sierpinski_triangle(t_data *data, int max_iter);
int					julia_set(t_data *data, double real, double imaginary);
int					burning_ship(t_data *data, double c_x, double c_y);
int					tricorn(t_data *data, double c_x, double c_y);
void				ft_init_fractal(t_data *data);
void				ft_init_sierpinski(t_data *data, t_point *points);
void				ft_multi_thread(t_data *data, int n_threads);
void				ft_image_fill(t_data *data, int x, int y, int color);

#endif
