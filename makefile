all: OJ.PRG

CC = cl65
FLAGS = -tcx16 -Cl -m oj.map -o OJ.PRG

OJ.PRG: main.c main.h routines.s waitforjiffy.s
	$(CC) $(FLAGS) main.c waitforjiffy.s routines.s

game:
	mkdir game/

build: OJ.PRG game
	cp OJ.PRG game/
	cp SPRITES.CHR TILES.CHR game/
	cp CHARACTERS/* game/

sd: OJ.PRG
	./scripts/mount_sd.sh oj.img
	sudo cp OJ.PRG mnt/
	sudo cp SPRITES.CHR TILES.CHR mnt/
	sudo cp CHARACTERS/* mnt/
	./scripts/close_sd.sh
