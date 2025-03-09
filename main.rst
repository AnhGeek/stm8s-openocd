                                      1 ;--------------------------------------------------------
                                      2 ; File Created by SDCC : free open source ANSI-C Compiler
                                      3 ; Version 4.2.0 #13081 (MINGW64)
                                      4 ;--------------------------------------------------------
                                      5 	.module main
                                      6 	.optsdcc -mstm8
                                      7 	
                                      8 ;--------------------------------------------------------
                                      9 ; Public variables in this module
                                     10 ;--------------------------------------------------------
                                     11 	.globl _main
                                     12 ;--------------------------------------------------------
                                     13 ; ram data
                                     14 ;--------------------------------------------------------
                                     15 	.area DATA
                                     16 ;--------------------------------------------------------
                                     17 ; ram data
                                     18 ;--------------------------------------------------------
                                     19 	.area INITIALIZED
                                     20 ;--------------------------------------------------------
                                     21 ; Stack segment in internal ram
                                     22 ;--------------------------------------------------------
                                     23 	.area	SSEG
      000001                         24 __start__stack:
      000001                         25 	.ds	1
                                     26 
                                     27 ;--------------------------------------------------------
                                     28 ; absolute external ram data
                                     29 ;--------------------------------------------------------
                                     30 	.area DABS (ABS)
                                     31 
                                     32 ; default segment ordering for linker
                                     33 	.area HOME
                                     34 	.area GSINIT
                                     35 	.area GSFINAL
                                     36 	.area CONST
                                     37 	.area INITIALIZER
                                     38 	.area CODE
                                     39 
                                     40 ;--------------------------------------------------------
                                     41 ; interrupt vector
                                     42 ;--------------------------------------------------------
                                     43 	.area HOME
      008000                         44 __interrupt_vect:
      008000 82 00 80 07             45 	int s_GSINIT ; reset
                                     46 ;--------------------------------------------------------
                                     47 ; global & static initialisations
                                     48 ;--------------------------------------------------------
                                     49 	.area HOME
                                     50 	.area GSINIT
                                     51 	.area GSFINAL
                                     52 	.area GSINIT
      008007                         53 __sdcc_init_data:
                                     54 ; stm8_genXINIT() start
      008007 AE 00 00         [ 2]   55 	ldw x, #l_DATA
      00800A 27 07            [ 1]   56 	jreq	00002$
      00800C                         57 00001$:
      00800C 72 4F 00 00      [ 1]   58 	clr (s_DATA - 1, x)
      008010 5A               [ 2]   59 	decw x
      008011 26 F9            [ 1]   60 	jrne	00001$
      008013                         61 00002$:
      008013 AE 00 00         [ 2]   62 	ldw	x, #l_INITIALIZER
      008016 27 09            [ 1]   63 	jreq	00004$
      008018                         64 00003$:
      008018 D6 80 23         [ 1]   65 	ld	a, (s_INITIALIZER - 1, x)
      00801B D7 00 00         [ 1]   66 	ld	(s_INITIALIZED - 1, x), a
      00801E 5A               [ 2]   67 	decw	x
      00801F 26 F7            [ 1]   68 	jrne	00003$
      008021                         69 00004$:
                                     70 ; stm8_genXINIT() end
                                     71 	.area GSFINAL
      008021 CC 80 04         [ 2]   72 	jp	__sdcc_program_startup
                                     73 ;--------------------------------------------------------
                                     74 ; Home
                                     75 ;--------------------------------------------------------
                                     76 	.area HOME
                                     77 	.area HOME
      008004                         78 __sdcc_program_startup:
      008004 CC 80 5C         [ 2]   79 	jp	_main
                                     80 ;	return from main will return to caller
                                     81 ;--------------------------------------------------------
                                     82 ; code
                                     83 ;--------------------------------------------------------
                                     84 	.area CODE
                                     85 ;	main.c: 14: static inline void delay_ms(uint16_t ms) {
                                     86 ;	-----------------------------------------
                                     87 ;	 function delay_ms
                                     88 ;	-----------------------------------------
      008024                         89 _delay_ms:
      008024 52 0A            [ 2]   90 	sub	sp, #10
      008026 1F 05            [ 2]   91 	ldw	(0x05, sp), x
                                     92 ;	main.c: 16: for (i = 0; i < ((F_CPU / 18000UL) * ms); i++)
      008028 5F               [ 1]   93 	clrw	x
      008029 1F 09            [ 2]   94 	ldw	(0x09, sp), x
      00802B 1F 07            [ 2]   95 	ldw	(0x07, sp), x
      00802D                         96 00103$:
      00802D 1E 05            [ 2]   97 	ldw	x, (0x05, sp)
      00802F 89               [ 2]   98 	pushw	x
      008030 AE 00 6F         [ 2]   99 	ldw	x, #0x006f
      008033 CD 80 80         [ 4]  100 	call	___muluint2ulong
      008036 5B 02            [ 2]  101 	addw	sp, #2
      008038 1F 03            [ 2]  102 	ldw	(0x03, sp), x
      00803A 17 01            [ 2]  103 	ldw	(0x01, sp), y
      00803C 1E 09            [ 2]  104 	ldw	x, (0x09, sp)
      00803E 13 03            [ 2]  105 	cpw	x, (0x03, sp)
      008040 7B 08            [ 1]  106 	ld	a, (0x08, sp)
      008042 12 02            [ 1]  107 	sbc	a, (0x02, sp)
      008044 7B 07            [ 1]  108 	ld	a, (0x07, sp)
      008046 12 01            [ 1]  109 	sbc	a, (0x01, sp)
      008048 24 0F            [ 1]  110 	jrnc	00105$
                                    111 ;	main.c: 17: __asm__("nop");
      00804A 9D               [ 1]  112 	nop
                                    113 ;	main.c: 16: for (i = 0; i < ((F_CPU / 18000UL) * ms); i++)
      00804B 1E 09            [ 2]  114 	ldw	x, (0x09, sp)
      00804D 5C               [ 1]  115 	incw	x
      00804E 1F 09            [ 2]  116 	ldw	(0x09, sp), x
      008050 26 DB            [ 1]  117 	jrne	00103$
      008052 1E 07            [ 2]  118 	ldw	x, (0x07, sp)
      008054 5C               [ 1]  119 	incw	x
      008055 1F 07            [ 2]  120 	ldw	(0x07, sp), x
      008057 20 D4            [ 2]  121 	jra	00103$
      008059                        122 00105$:
                                    123 ;	main.c: 18: }
      008059 5B 0A            [ 2]  124 	addw	sp, #10
      00805B 81               [ 4]  125 	ret
                                    126 ;	main.c: 20: void main() {
                                    127 ;	-----------------------------------------
                                    128 ;	 function main
                                    129 ;	-----------------------------------------
      00805C                        130 _main:
                                    131 ;	main.c: 21: PB_DDR |= (1 << LED_PIN); // configure PD4 as output
      00805C 72 1A 50 07      [ 1]  132 	bset	0x5007, #5
                                    133 ;	main.c: 22: PB_CR1 |= (1 << LED_PIN); // push-pull mode
      008060 72 1A 50 08      [ 1]  134 	bset	0x5008, #5
                                    135 ;	main.c: 24: while (1) {
      008064                        136 00102$:
                                    137 ;	main.c: 26: PB_ODR ^= (1 << LED_PIN);
      008064 90 1A 50 05      [ 1]  138 	bcpl	0x5005, #5
                                    139 ;	main.c: 16: for (i = 0; i < ((F_CPU / 18000UL) * ms); i++)
      008068 90 5F            [ 1]  140 	clrw	y
      00806A 5F               [ 1]  141 	clrw	x
      00806B                        142 00107$:
      00806B 90 A3 B1 98      [ 2]  143 	cpw	y, #0xb198
      00806F 9F               [ 1]  144 	ld	a, xl
      008070 A2 01            [ 1]  145 	sbc	a, #0x01
      008072 9E               [ 1]  146 	ld	a, xh
      008073 A2 00            [ 1]  147 	sbc	a, #0x00
      008075 24 ED            [ 1]  148 	jrnc	00102$
                                    149 ;	main.c: 17: __asm__("nop");
      008077 9D               [ 1]  150 	nop
                                    151 ;	main.c: 16: for (i = 0; i < ((F_CPU / 18000UL) * ms); i++)
      008078 90 5C            [ 1]  152 	incw	y
      00807A 26 EF            [ 1]  153 	jrne	00107$
      00807C 5C               [ 1]  154 	incw	x
      00807D 20 EC            [ 2]  155 	jra	00107$
                                    156 ;	main.c: 27: delay_ms(1000);
                                    157 ;	main.c: 29: }
      00807F 81               [ 4]  158 	ret
                                    159 	.area CODE
                                    160 	.area CONST
                                    161 	.area INITIALIZER
                                    162 	.area CABS (ABS)
