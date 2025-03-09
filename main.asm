;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 4.2.0 #13081 (MINGW64)
;--------------------------------------------------------
	.module main
	.optsdcc -mstm8
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _main
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area DATA
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area INITIALIZED
;--------------------------------------------------------
; Stack segment in internal ram
;--------------------------------------------------------
	.area	SSEG
__start__stack:
	.ds	1

;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area DABS (ABS)

; default segment ordering for linker
	.area HOME
	.area GSINIT
	.area GSFINAL
	.area CONST
	.area INITIALIZER
	.area CODE

;--------------------------------------------------------
; interrupt vector
;--------------------------------------------------------
	.area HOME
__interrupt_vect:
	int s_GSINIT ; reset
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area HOME
	.area GSINIT
	.area GSFINAL
	.area GSINIT
__sdcc_init_data:
; stm8_genXINIT() start
	ldw x, #l_DATA
	jreq	00002$
00001$:
	clr (s_DATA - 1, x)
	decw x
	jrne	00001$
00002$:
	ldw	x, #l_INITIALIZER
	jreq	00004$
00003$:
	ld	a, (s_INITIALIZER - 1, x)
	ld	(s_INITIALIZED - 1, x), a
	decw	x
	jrne	00003$
00004$:
; stm8_genXINIT() end
	.area GSFINAL
	jp	__sdcc_program_startup
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area HOME
	.area HOME
__sdcc_program_startup:
	jp	_main
;	return from main will return to caller
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CODE
;	main.c: 14: static inline void delay_ms(uint16_t ms) {
;	-----------------------------------------
;	 function delay_ms
;	-----------------------------------------
_delay_ms:
	sub	sp, #10
	ldw	(0x05, sp), x
;	main.c: 16: for (i = 0; i < ((F_CPU / 18000UL) * ms); i++)
	clrw	x
	ldw	(0x09, sp), x
	ldw	(0x07, sp), x
00103$:
	ldw	x, (0x05, sp)
	pushw	x
	ldw	x, #0x006f
	call	___muluint2ulong
	addw	sp, #2
	ldw	(0x03, sp), x
	ldw	(0x01, sp), y
	ldw	x, (0x09, sp)
	cpw	x, (0x03, sp)
	ld	a, (0x08, sp)
	sbc	a, (0x02, sp)
	ld	a, (0x07, sp)
	sbc	a, (0x01, sp)
	jrnc	00105$
;	main.c: 17: __asm__("nop");
	nop
;	main.c: 16: for (i = 0; i < ((F_CPU / 18000UL) * ms); i++)
	ldw	x, (0x09, sp)
	incw	x
	ldw	(0x09, sp), x
	jrne	00103$
	ldw	x, (0x07, sp)
	incw	x
	ldw	(0x07, sp), x
	jra	00103$
00105$:
;	main.c: 18: }
	addw	sp, #10
	ret
;	main.c: 20: void main() {
;	-----------------------------------------
;	 function main
;	-----------------------------------------
_main:
;	main.c: 21: PB_DDR |= (1 << LED_PIN); // configure PD4 as output
	bset	0x5007, #5
;	main.c: 22: PB_CR1 |= (1 << LED_PIN); // push-pull mode
	bset	0x5008, #5
;	main.c: 24: while (1) {
00102$:
;	main.c: 26: PB_ODR ^= (1 << LED_PIN);
	bcpl	0x5005, #5
;	main.c: 16: for (i = 0; i < ((F_CPU / 18000UL) * ms); i++)
	clrw	y
	clrw	x
00107$:
	cpw	y, #0xb198
	ld	a, xl
	sbc	a, #0x01
	ld	a, xh
	sbc	a, #0x00
	jrnc	00102$
;	main.c: 17: __asm__("nop");
	nop
;	main.c: 16: for (i = 0; i < ((F_CPU / 18000UL) * ms); i++)
	incw	y
	jrne	00107$
	incw	x
	jra	00107$
;	main.c: 27: delay_ms(1000);
;	main.c: 29: }
	ret
	.area CODE
	.area CONST
	.area INITIALIZER
	.area CABS (ABS)
