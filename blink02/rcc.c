#include "globals.h"
#include "rcc.h"

void rcc_init(rcc_t *rcc){
 rcc_setup_cpu(rcc, PLL_9, PPRE2_8);
}

void delay(uint32_t count){
 while(count--){
  asm("nop");
 }
}

void rcc_setup_cpu(rcc_t *rcc, uint32_t pll_clk, uint32_t apb_clk){
 //Enable HSE
 rcc->cr |= (1 << 16);
 while(! (rcc->cr & (1 << 17)));

 rcc->cfgr |= 1;  //Switch to HSE
 rcc->cr &= ~1;   // Disable HSI
 rcc->cfgr |= pll_clk; //PLL = 9
 rcc->cfgr |= (1 << 16); //HSE as PLL_SRC

 *FLASH_ACR = 0b10010;

 rcc->cr |= (1 << 24);
 while(! (rcc->cr & (1 << 25)));

 rcc->cfgr = (rcc->cfgr | 0b10) & ~1 | apb_clk; // ABP = CLK / 8
 while(! (rcc->cfgr & (1 << 3)));
}
