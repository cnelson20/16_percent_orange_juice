all: OJ.PRG
	

OJ.PRG: main.c main.h routines.s
	cl65 -t cx16 main.c waitforjiffy.s routines.s -o OJ.PRG
