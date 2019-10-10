/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msidqi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/04 04:51:26 by msidqi            #+#    #+#             */
/*   Updated: 2019/05/02 05:05:36 by msidqi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgl.h"
#include "libft.h"
#define DATA_SIZE (720 * 1280)
#define PROGRAM_FILE "kernel.cl"
#define KERNEL_FUNC "fractol"

void			ft_init_fractal(t_data *data)
{
	data->movex = 0;
	data->movey = 0;
	data->zoom = data->winwidth / 5;
	data->max_iter = 50;
	data->islocked = false;
	data->color.red = 135;
	data->color.green = 206;
	data->color.blue = 235;
    data->juliapos.x = 0;
    data->juliapos.y = 0;
}

double			lerp(double x1, double x2, double lerp)
{
	return (x1 * (1.0 - lerp)) + (x2 * lerp);
}

int				ft_input_check(t_data *data, int argc, char **argv)
{
	if (argc == 2)
	{
		if (!ft_strcmp(argv[1], "Julia") || ft_strcmp(argv[1], "julia"))
			data->set = 'j';
		else if (!ft_strcmp(argv[1], "Mandelbrot") || ft_strcmp(argv[1], "mandelbrot"))
			data->set = 'm';
		else if (!ft_strcmp(argv[1], "Burningship") || ft_strcmp(argv[1], "burningship"))
			data->set = 'b';
		else if (!ft_strcmp(argv[1], "Tricorn") || ft_strcmp(argv[1], "tricorn"))
			data->set = 't';
		else
			return (0);
	}
	else
		return (0);
	return (1);
}

void		ft_print_instructions(void)
{
	ft_putendl(" ___________________________________________________________");
	ft_putendl("| Arrows :      Move                                        |");
	ft_putendl("| S :           Switch fractal                              |");
	ft_putendl("| R :           Reset values                                |");
	ft_putendl("| L :           Lock Julia set                              |");
	ft_putendl("| C :           Change color                                |");
	ft_putendl("| Mouse wheel : Zoom                                        |");
	ft_putendl("| L/R click :   Increace/Decrease iteration                 |");
	ft_putendl(" -----------------------------------------------------------");
}
		
////////////////////////////////////////////////////////////////////////////////
int read_build_kernel(char *file, cl_context context, cl_program *program,  cl_kernel *kernel)
{
    // Reads render.cl and builds a program from char buffer(created form render.cl).
	FILE				*program_handle;
	char				*program_buffer;//, *program_log;
	size_t				program_size;//, log_size;
    int                 error;

	if ((program_handle = fopen(file, "r")) == NULL)
		exit(1);
	fseek(program_handle, 0, SEEK_END);
	program_size = ftell(program_handle);
	rewind(program_handle);
	program_buffer = (char *)malloc(program_size + 1);
	program_buffer[program_size] = '\0';
	fread(program_buffer, sizeof(char), program_size, program_handle);
	fclose(program_handle);

    if (!(*program = clCreateProgramWithSource(context, 1, (const char **) &program_buffer, NULL, &error))) {
        printf("Error: Failed to create compute program! %d\n", error);
        return EXIT_FAILURE;
    }
	free(program_buffer);
	if (clBuildProgram(*program, 0, NULL, NULL, NULL, NULL) != CL_SUCCESS) {
        printf("Error: Failed to build program executable!\n");
        exit(1);
    }
	if (!(*kernel = clCreateKernel(*program, "fractol", &error)) || error != CL_SUCCESS) {
        printf("Error: Failed to create compute kernel!\n");
        exit(1);
    }
    return (0);
}

int setup_device_commandqueue(cl_device_id *device_id, cl_context *context, cl_command_queue *commands)
{
    int err;
    int gpu = 1;
    if (clGetDeviceIDs(NULL, gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, device_id, NULL) != CL_SUCCESS) {
        printf("Error: Failed to create a device group!\n");
        return EXIT_FAILURE;
    }
    // Create a compute context 
    if (!(*context = clCreateContext(0, 1, device_id, NULL, NULL, &err))) {
        printf("Error: Failed to create a compute context!\n");
        return EXIT_FAILURE;
    }
    // Create a command commands
    if (!(*commands = clCreateCommandQueue(*context, *device_id, 0, &err))) {
        printf("Error: Failed to create a command commands!\n");
        return EXIT_FAILURE;
    }
    return (0);
}
int gpu_frac(t_data *fra)
{
    int err;                            // error code returned from api calls
    // unsigned int correct;               // number of correct results returned
    size_t global;                      // global domain size for our calculation
    size_t local;                       // local domain size for our calculation
    // int i = 0;
    unsigned int count = DATA_SIZE;

    // Create the input and output arrays in device memory for our calculation
    if (/*!(input = clCreateBuffer(context,  CL_MEM_READ_ONLY,  sizeof(int) * count, NULL, NULL)) ||*/
        !(fra->output = clCreateBuffer(fra->context, CL_MEM_WRITE_ONLY, sizeof(int) * count, NULL, NULL))) {
        printf("Error: Failed to allocate device memory!\n");
        exit(1);
    }
    // // Write our data set into the input array in device memory 
    // if (clEnqueueWriteBuffer(commands, input, CL_TRUE, 0, sizeof(int) * count, data, 0, NULL, NULL) != CL_SUCCESS) {
    //     printf("Error: Failed to write to source array!\n");
    //     exit(1);
    // }

    // Set the arguments to our compute kernel
    err = 0;

    // err  = clSetKernelArg(kernel, 0, sizeof(input), &input);
    err |= clSetKernelArg(fra->kernel, 0, sizeof(fra->output), &fra->output);
    err |= clSetKernelArg(fra->kernel, 1, sizeof(unsigned int), &fra->winwidth);
    err |= clSetKernelArg(fra->kernel, 2, sizeof(unsigned int), &fra->winheight);
    err |= clSetKernelArg(fra->kernel, 3, sizeof(unsigned int), (unsigned int *)&fra->max_iter);
    err |= clSetKernelArg(fra->kernel, 4, sizeof(double), &fra->movex);
    err |= clSetKernelArg(fra->kernel, 5, sizeof(double), &fra->movey);
    err |= clSetKernelArg(fra->kernel, 6, sizeof(double), &fra->zoom);
    err |= clSetKernelArg(fra->kernel, 7, sizeof(fra->juliapos), &fra->juliapos);
    err |= clSetKernelArg(fra->kernel, 8, sizeof(char), &fra->set);
    err |= clSetKernelArg(fra->kernel, 9, sizeof(fra->color), &fra->color);
    if (err != CL_SUCCESS) {
        printf("Error: Failed to set kernel arguments! %d\n", err);
        exit(1);
    }
    // Get the maximum work group size for executing the kernel on the device
    if (clGetKernelWorkGroupInfo(fra->kernel, fra->device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL) != CL_SUCCESS) {
        printf("Error: Failed to retrieve kernel work group info! %d\n", err);
        exit(1);
    }

    // Execute the kernel over the entire range of our 1d input data set
    // using the maximum number of work group items for this device
    global = count;
    if (clEnqueueNDRangeKernel(fra->commands, fra->kernel, 1, NULL, &global, &local, 0, NULL, NULL))  {
        printf("Error: Failed to execute kernel!\n");
        return EXIT_FAILURE;
    }

    

    // Read back the results from the device to verify the output
    if (clEnqueueReadBuffer(fra->commands, fra->output, CL_TRUE, 0, sizeof(float) * count, fra->results, 0, NULL, NULL ) != CL_SUCCESS) {
        printf("Error: Failed to read output array! %d\n", err);
        exit(1);
    }
    
    // Wait for the command commands to get serviced before reading back results
    clFinish(fra->commands);
    // Validate our results
    // correct = 0;
    // for(i = 0; i < (int)count; i++)
    // {
    //     // if(results[i] == data[i] * data[i])
    //     if (fra->results[i] == 0x0)
    //         correct++;
    //     if (fra->results[i] == 0xFFFFFF)
    //         correct++;
    // }
    // // Print a brief summary detailing the results
    // printf("Computed '%d/%d' correct values!\n", correct, count);

    ft_memcpy((void *)fra->image, (void *)fra->results, sizeof(int) * (size_t)DATA_SIZE);
    return (0);    
}

int main(int argc, char** argv)
{
  
	t_data	data;

	srand(time(NULL));
	if (!ft_input_check(&data, argc, argv))
	{
		ft_putendl("Usage: ./fractol [Mandelbrot][Julia][Sierp][Burning][Tric]");
		return (0);
	}
	if (!(ft_window_setup(&data, "Fract'ol", 720, 1280)) || !ft_image_setup(&data))
		return (0);
	ft_init_fractal(&data);
	// ft_fractal_choice(&data);
    data.results = (int *)malloc(sizeof(int) * DATA_SIZE);
    setup_device_commandqueue(&data.device_id, &data.context, &data.commands);
    read_build_kernel(PROGRAM_FILE, data.context, &data.program, &data.kernel);
    gpu_frac(&data);
	mlx_put_image_to_window(data.mlx, data.win, data.img_ptr, 0, 0);
	ft_print_instructions();
	ft_mlx_hooks(&data);
}
