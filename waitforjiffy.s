RDTIM = $FFDE

.importzp ptr1

.export _waitforjiffy
_waitforjiffy:
jsr RDTIM
sta @byte
:
jsr RDTIM
cmp #$FF
@byte = * - 1
beq :-

rts
