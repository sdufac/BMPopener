gcc src/main.c src/bmp.c -o BMPopener $(pkg-config --cflags --libs sdl3)
