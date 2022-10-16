all: OJ.PRG
	

OJ.PRG: main.c main.h routines.s
	cl65 -t cx16 -Cl main.c waitforjiffy.s routines.s -o OJ.PRG

build: OJ.PRG
	./scripts/mount_sd.sh oj.img
	sudo cp OJ.PRG mnt/
	sudo cp SPRITES.CHR TILES.CHR mnt/
	sudo cp CHARACTERS/* mnt/
	./scripts/close_sd.sh
	
