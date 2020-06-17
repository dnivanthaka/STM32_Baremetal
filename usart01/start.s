.cpu cortex-m3
.thumb

/*---------------- Setup vector table entries -----------------*/

.word 0x20003000 /*SP*/
.word _reset     /*Reset*/
.word hang       /*Hard fault*/
.word hang       /*MM fault*/
.word hang       /*Bus fault*/
.word hang       /*Usage fault*/
.word hang       /*Reserved*/
.word hang       /*Reserved*/
.word hang       /*Reserved*/
.word hang       /*Reserved*/
.word hang       /*SVCall*/
.word hang       /*Debug - reserved*/
.word hang       /*Reserved*/
.word hang       /*Pend SV*/
.word hang       /*Systick*/

.thumb_func
hang: b hang

.thumb_func
_reset:
 bl startup
 b .             /*Should not reach here*/

.align

.thumb_func
.globl PUT16
PUT16:
 strh r1, [r0]
 bx lr

.thumb_func
.globl PUT32
PUT32:
 str r1, [r0]
 bx lr

.thumb_func
.globl GET32
GET32:
 ldr r0, [r0]
 bx lr

.end
