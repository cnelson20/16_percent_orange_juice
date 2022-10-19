.SETCPU "65c02"

RDTIM = $FFDE

.export _waitforjiffy
_waitforjiffy:
jsr RDTIM
sta tempbyte
:
jsr RDTIM
cmp tempbyte
beq :-
rts
tempbyte:
	.byte 0
