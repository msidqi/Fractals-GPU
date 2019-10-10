#define ABS(Value) (Value >= 0) ? (Value) : -(Value)
#define MAX_ITER 30
#define MAX_ITER1 700
double		ft_get_world_pos(double screen_coord, double widheight, double zoom)
{
	return (((screen_coord - widheight / 2) / zoom));
}

int			burning_ship(unsigned int max_iter, double c_x, double c_y)
{
	double	real;
	double	imaginary;
	double	tmpreal;
	unsigned int		i;

	i = 0;
	imaginary = 0;
	real = 0;
	while (i < max_iter)
	{
		tmpreal = ABS(real * real - imaginary * imaginary + c_x);
		imaginary = ABS(2 * real * imaginary + c_y);
		real = tmpreal;
		if (real * real + imaginary * imaginary > 4)
			return (i);
		i++;
	}
	return (0);
}

int			julia_set(const double2 juliapos, unsigned int max_iter, double real, double imaginary)
{
	double	c_x;
	double	c_y;
	double	tmpreal;
	unsigned int		i;
	// int col;

	// col = 0;
	c_x = juliapos.x;
	c_y = juliapos.y;
	i = 0;
	while (i < max_iter)
	{
		tmpreal = real * real - imaginary * imaginary + c_x;
		imaginary = 2 * real * imaginary + c_y;
		real = tmpreal;
		// col += exp(ABS(-z));
		if (real * real + imaginary * imaginary > 4)
			return (i);
		i++;
	}
	return (0);
}

int			tricorn(unsigned int max_iter, double c_x, double c_y)
{
	double	real;
	double	imaginary;
	double	tmpreal;
	unsigned int		i;

	i = 0;
	imaginary = 0;
	real = 0;
	while (i < max_iter)
	{
		tmpreal = real * real - imaginary * imaginary + c_x;
		imaginary = -2 * real * imaginary + c_y;
		real = tmpreal;
		if (real * real + imaginary * imaginary > 4)
			return (i);
		i++;
	}
	return (0);
}

int			mandelbrot_set(unsigned int max_iter, double c_x, double c_y)
{
	double	real;
	double	imaginary;
	double	tmpreal;
	unsigned int		i;

	i = 0;
	imaginary = 0;
	real = 0;
	while (i < max_iter)
	{
		tmpreal = real * real - imaginary * imaginary + c_x;
		imaginary = 2 * real * imaginary + c_y;
		real = tmpreal;
		if (real * real + imaginary * imaginary > 4)
			return (i);
		i++;
	}
	return (0);
}

void	ft_color_change(int *col, uchar3 color, const double zoom, unsigned int max_iter)
{
	int				tmp;
	unsigned char	*ptr;
	tmp = *col;
	ptr = (unsigned char*)col;
	ptr[0] = (tmp) * (color.z / (MAX_ITER - 10));
	ptr[1] = (tmp) * (color.y / (MAX_ITER - 10));
	ptr[2] = (tmp) * (color.x / (MAX_ITER - 10));
}
	

int	ft_checkif_in_set(const char set, const double2 juliapos, unsigned int max_iter, double a, double b)
{
	if (set == 'j')
		return (julia_set(juliapos, max_iter, a, b));
	else if (set == 'm')
		return (mandelbrot_set(max_iter, a, b));
	else if (set == 'b')
		return (burning_ship(max_iter, a, b));
	else if (set == 't')
		return (tricorn(max_iter, a, b));
	return (0);
}

__kernel void fractol(
   __global int* results,
   const unsigned int width,
   const unsigned int height,
   const unsigned int max_iter,
   const double movex,
   const double movey,
   const double zoom,
   const double2 juliapos,
   const char set,
   uchar3 color)
{
	const int i = get_global_id(0);
	int col;
	int x = i % width;
	int y = i / width;
	double w_x = ft_get_world_pos(x, (double)width, zoom) - movex;
	double w_y = ft_get_world_pos(y, (double)height, zoom) - movey;
	if (col = ft_checkif_in_set(set, juliapos, max_iter, w_x, w_y))
	{
		ft_color_change(&col, color, zoom, max_iter);
		results[i] = col;// % 2 == 0 ? 0xFFFFFF : 0x0;
	}
	else
		results[i] = 0x0;
}
