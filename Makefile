# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msidqi <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/23 00:08:25 by msidqi            #+#    #+#              #
#    Updated: 2018/11/25 22:04:07 by msidqi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

LIBFT = libft.a
LIBFTL = src/libft/libft.a
EXE = fractol
SRC = src/ft_event_system.c src/ft_events_keyboard.c  src/main.c src/ft_events.c  src/ft_image_setup.c
INCLUDE = Include
OBJ = ft_events_keyboard.o ft_image_setup.o main.o ft_event_system.o  ft_events.o 
GLIB = libgl.a
CC = gcc -O3 -Wall -Wextra -Werror 
FLAGS = -lmlx -framework OpenGl -framework Appkit
OPENCL = -std=c99 -DUNIX -g -DDEBUG -DMAC -arch x86_64   -framework OpenCL
all : $(EXE)

$(GLIB) :
	$(CC) -c $(SRC) -I $(INCLUDE)
	ar rc $(GLIB) $(OBJ)
	ranlib $(GLIB)
$(LIBFTL) : 
	make -C src/libft 
$(EXE) : $(LIBFTL) $(GLIB)
	$(CC) -o $(EXE) $(SRC) $(GLIB) src/libft/libft.a -I $(INCLUDE) $(FLAGS) $(OPENCL)

clean : 
	rm -rf $(OBJ) $(GLIB) $(LIBFTL)
	make clean -C src/libft
fclean : clean
	rm -rf $(EXE)
	make fclean -C src/libft
re : fclean all
