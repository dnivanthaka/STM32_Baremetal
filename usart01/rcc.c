#include "globals.h"
#include "rcc.h"


static rcc_t  *rcc    = (struct rcc_t  *) RCCBASE;

void rcc_init(){
 rcc_setup_cpu();
}

void delay(uint32_t count){
	while(count--){
        asm("nop");
	}
}

void rcc_setup_cpu(){
 //Enable HSE
 rcc->cr |= (1 << 16);
 while(! (rcc->cr & (1 << 17)));

 rcc->cfgr |= 1;  //Switch to HSE
 rcc->cr &= ~1;   // Disable HSI
 rcc->cfgr |= (0b111 << 18); //PLL = 9
 rcc->cfgr |= (1 << 16); //HSE as PLL_SRC

 *FLASH_ACR = 0b10010;

 rcc->cr |= (1 << 24);
 while(! (rcc->cr & (1 << 25)));

 rcc->cfgr = (rcc->cfgr | 0b10) & ~1 | (0b110 << 11); // ABP = CLK / 8
 while(! (rcc->cfgr & (1 << 3)));
}
