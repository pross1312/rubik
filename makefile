CC=g++
CFLAGS=-Wall -Wextra `pkg-config --cflags sdl2 opengl glew` -ggdb -fsanitize=address,undefined
LIBS=`pkg-config --libs opengl glew sdl2`

default:
	$(CC) src/*.cpp -o rubik $(CFLAGS) $(LIBS)

