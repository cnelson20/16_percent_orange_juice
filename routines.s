.importzp tmp1, tmp2, tmp3
.importzp ptr1, ptr2

.struct board_tile
	xx .byte
	yy .byte
	type .byte
	
	next .res 6
	
	has_neighbor_ne .byte
	has_neighbor_nw .byte
	has_neighbor_se .byte
	has_neighbor_sw .byte
.endstruct 

BOARD_TILE_X = 0
BOARD_TILE_Y = 1
BOARD_TILE_TYPE = 2
BOARD_TILE_NEXT = 3
BOARD_TILE_HAS_NE = 9
BOARD_TILE_HAS_NW = 10
BOARD_TILE_HAS_SE = 11
BOARD_TILE_HAS_SW = 12

FILL_IN_TILE = 22
FULL_TILE = 23

X_OFFSET = 6

.import _board_tile_chars_array

;
; void __fastcall__ plot_tile(struct board_tile *tile);
; AX - pointer to tile 
; X - x pos
; Y - y pos
;
.export _plot_tile
_plot_tile:
		sta ptr1
		stx ptr1 + 1
		ldy #BOARD_TILE_X
		lda (ptr1), Y
		
		sta tmp1
		tax 
		
		ldy #BOARD_TILE_Y
		lda (ptr1), Y
		
		sta tmp2
		
		txa ; bring XX back into A
		cmp tmp2 ; compare to YY 		
		lda #X_OFFSET ; for use in a second 
		bcc :+ 
		; xx > yy ;
		clc 
		adc tmp1
		sec 
		sbc tmp2
		
		bra :++
		:
		clc 
		adc tmp1
		sec 
		sbc tmp2
		
		:
		asl A
		asl A
		asl A
		sta $9F20
		tax
		
		lda tmp1
		clc
		adc tmp2 ; XX + YY
		adc #2
		asl A
		asl A
		sta $9F21
		
		stx tmp1 ; save $9F20 inital pos for later
		
		lda #$10
		sta $9F22
		
		ldy #BOARD_TILE_TYPE
		lda (ptr1), Y
		asl
		sec 
		adc (ptr1), Y
		;inc A
		sta tmp2
		
		ldy #BOARD_TILE_HAS_NW
		lda (ptr1), Y
		beq :+
		lda $9F20
		sec 
		sbc #4
		sta $9F20 
		lda #FILL_IN_TILE
		sta $9F23
		lda #8 + 4
		sta $9F23
		lda #FULL_TILE
		sta $9F23
		stz $9F23
		sta $9F23
		stz $9F23
		lda tmp2
		inc A
		sta $9F23
		lda #4
		sta $9F23
		bra :++
		:
		lda $9F20
		clc 
		adc #2
		sta $9F20
		lda tmp2
		sta $9F23
		lda #4
		sta $9F23
		:
		
		ldy #BOARD_TILE_HAS_NE
		lda (ptr1), Y
		beq :+
		lda tmp2
		inc A
		sta $9F23
		stz $9F23
		lda #FULL_TILE
		sta $9F23 
		stz $9F23
		sta $9F23
		stz $9F23
		lda #FILL_IN_TILE
		sta $9F23
		lda #8
		sta $9F23		
		bra :++
		:
		lda tmp2
		sta $9F23
		stz $9F23
		:
		
		inc $9F21
		lda tmp1
		sta $9F20
		
		ldy #BOARD_TILE_HAS_NW
		ldx tmp2
		lda (ptr1), Y
		beq :+
		
		dec $9F20
		dec $9F20
		lda #FILL_IN_TILE
		sta $9F23
		lda #8 + 4
		sta $9F23
		inx
		:
		stx $9F23
		lda #4
		sta $9F23
		
		lda tmp2
		clc 
		adc #2
		sta $9F23 
		stz $9F23
		sta $9F23 
		stz $9F23
		
		ldx tmp2
		ldy #BOARD_TILE_HAS_NE
		lda (ptr1), Y
		beq :+
		inx
		:
		stx $9F23
		stz $9F23
		
		cmp #0
		beq :+
		lda #FILL_IN_TILE
		sta $9F23
		lda #8
		sta $9F23
		:
		
		inc $9F21
		lda tmp1
		sta $9F20
		
		ldx tmp2
		ldy #BOARD_TILE_HAS_SW
		lda (ptr1), Y
		beq :+
		dec $9F20
		dec $9F20
		lda #FILL_IN_TILE
		sta $9F23
		lda #4
		sta $9F23
		
		inx
		:
		stx $9F23
		ldx #8 + 4
		stx $9F23
		
		lda tmp2 
		clc 
		adc #2
		sta $9F23
		stz $9F23
		sta $9F23
		stz $9F23
		
		ldx tmp2
		ldy #BOARD_TILE_HAS_SE
		lda (ptr1), Y
		beq :+
		inx
		:
		stx $9F23
		ldx #8
		stx $9F23
		
		cmp #0
		beq :+
		lda #FILL_IN_TILE
		sta $9F23
		stz $9F23
		:
		
		inc $9F21
		lda tmp1
		sta $9F20
		
		ldy #BOARD_TILE_HAS_SW
		lda (ptr1), Y
		beq :+
		lda $9F20
		sec 
		sbc #4
		sta $9F20
		
		lda #FILL_IN_TILE
		sta $9F23
		lda #4
		sta $9F23
		
		lda #FULL_TILE
		sta $9F23
		stz $9F23
		
		sta $9F23
		stz $9F23
		ldx tmp2
		inx
		stx $9F23
		lda #8 + 4
		sta $9F23
		bra :++
		:
		lda tmp1
		clc 
		adc #2
		sta $9F20
		ldx tmp2
		stx $9F23
		lda #8 + 4
		sta $9F23
		:
		
		ldy #BOARD_TILE_HAS_SE
		lda (ptr1), Y
		beq :+
		ldx tmp2 
		inx 
		stx $9F23
		lda #8
		sta $9F23
		lda #FULL_TILE
		sta $9F23
		stz $9F23
		sta $9F23
		stz $9F23
		lda #FILL_IN_TILE
		sta $9F23
		stz $9F23
		
		bra :++
		:
		ldx tmp2
		stx $9F23
		lda #8
		sta $9F23
		:
		rts 
		
.export _clear_layer1
_clear_layer1:
	stz $9F20
	stz $9F21
	
	lda #$10
	sta $9F22

	ldy #$80 / 2
	ldx #0
	:
	stz $9F23
	stz $9F23
	dex
	bne :-
	dey 
	bne :-
	rts 

.export _clear_layer1_blue
_clear_layer1_blue:
	stz $9F20
	stz $9F21
	
	ldx #$10
	stx $9F22

	ldy #$40 / 2
	ldx #0
	:
	lda #$20
	sta $9F23
	lda #$61
	sta $9F23
	dex
	bne :-
	dey 
	bne :-
	rts 
	
;
; void __fastcall__ print_str(char *s);
;
.export _print_str
_print_str:
	sta ptr1
	stx ptr1 + 1
	ldy #0
@loop:
	lda (ptr1), Y
	beq @end
	cmp #'_'
	bne :+
	lda #$20
	:
	sta $9F23
	lda #$61
	sta $9F23
	iny
	bra @loop
@end:
	rts

;
; char __fastcall__ keyboard_get();
;
.export _keyboard_get
_keyboard_get:
	jsr $FFE4
	rts
	
;
; char __fastcall__ dig_to_ascii(char c);
;
.export _dig_to_ascii
_dig_to_ascii:
	cmp #10
	bcs :+
	adc #$30
	rts 
	:
	clc 
	adc #$41 - 10
	rts

.import popa

;
; char __fastcall__ sabs(char a, char b);
;
.export _sabs
_sabs:
	sta @temp
	jsr popa
	cmp @temp
	bcc :+
	sbc @temp 
	rts
	
	:
	sta tmp1
	lda @temp
	sec 
	sbc tmp1
	rts 
@temp:
	.word 0
	
.import _rand

;
; char __fastcall__ rand_byte();
;
.export _rand_byte
_rand_byte:
	jsr _rand
	stx tmp1 
	eor tmp1 
	ldx #0
	rts

;
; char __fastcall__ roll_die();
;
.export _roll_die
_roll_die:
	:
	jsr _rand_byte
	and #7
	cmp #6
	bcs :-
	inc A 
	rts 
	
.export _draw_horiz_band
_draw_horiz_band:
	lda $9F38
	sta tmp1
	lda $9F37
	lsr tmp1 
	ror A
	lsr tmp1
	ror A 
	lsr tmp1
	ror A 
	and #%11111110
	sta tmp1
	
	lda $9F3A
	sta tmp2
	lda $9F39
	lsr tmp2
	ror A 
	lsr tmp2
	ror A
	lsr A 
	clc
	adc #10
	sta $9F21
	
	lda #$10
	sta $9F22
	
	ldy #10
	:
	lda tmp1 
	sta $9F20
	ldx #20
	:
	lda $9F21
	pha
	lda #18
	sta $9F23
	lda #$30
	sta $9F23
	
	pla 
	sta $9F21
	
	dex 
	bne :-
	inc $9F21
	dey
	bne :--	
	
	rts
	