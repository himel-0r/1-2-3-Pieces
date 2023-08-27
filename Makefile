CC=g++
LFLAGS=-lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

SOURCE_FILES=main.cpp player.cpp platform.cpp

all: main

main: $(SOURCE_FILES)
	$(CC) $(SOURCE_FILES) $(LFLAGS) -o main

clean:
	rm -f main
