CC=g++
CFLAGS=-Wall -Wextra `pkg-config --cflags sdl2 opengl glew` -ggdb -fsanitize=address,undefined
LIBS=`pkg-config --libs opengl glew sdl2`
SRC=src/*.cpp

rubik: $(SRC)
	$(CC) $(SRC) -o rubik $(CFLAGS) $(LIBS)

