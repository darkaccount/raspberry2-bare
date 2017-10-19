
;@ ------------------------------------------------------------------
;@ ------------------------------------------------------------------
.section .init
.globl _start
_start:
    ldr pc,reset_handler
    ldr pc,undefined_handler
    ldr pc,swi_handler
    ldr pc,prefetch_handler
    ldr pc,data_handler
    ldr pc,hyp_handler
    ldr pc,irq_handler
    ldr pc,fiq_handler
reset_handler:      .word reset
undefined_handler:  .word hang
swi_handler:        .word hang
prefetch_handler:   .word hang
data_handler:       .word hang
hyp_handler:        .word hang
irq_handler:        .word hang
fiq_handler:        .word hang

.section .text
reset:
    mrc p15,0,r0,c0,c0,5 ;@ MPIDR
    mov r1,#0xFF
    ands r1,r1,r0
    bne not_zero

    mov sp,#0x8000
    bl notmain
hang: b hang

not_zero:
    cmp r1,#1
    beq core_one
    cmp r1,#2
    beq core_two
    cmp r1,#3
    beq core_three
    b .

core_one:
ldr r1,=0x1000
str r0,[r1]
    mov sp,#0x6000
    mov r1,#0
    str r1,[sp]
core_one_loop:
    ldr r0,[sp]
    cmp r0,#0
    beq core_one_loop
    bl hopper
    b hang

core_two:
ldr r1,=0x1004
str r0,[r1]
    mov sp,#0x4000
    mov r1,#0
    str r1,[sp]
core_two_loop:
    ldr r0,[sp]
    cmp r0,#0
    beq core_two_loop
    bl hopper
    b hang

core_three:
ldr r1,=0x1008
str r0,[r1]
    mov sp,#0x2000
    mov r1,#0
    str r1,[sp]
core_three_loop:
    ldr r0,[sp]
    cmp r0,#0
    beq core_three_loop
    bl hopper
    b hang

hopper:
    bx r0

.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

.globl dummy
dummy:
    bx lr



;@ ------------------------------------------------------------------
;@ ------------------------------------------------------------------


;@-------------------------------------------------------------------------
;@
;@ Copyright (c) 2012 David Welch dwelch@dwelch.com
;@
;@ Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
;@
;@ The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
;@
;@ THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
;@
;@-------------------------------------------------------------------------