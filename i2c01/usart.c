#include "globals.h"
#include "rcc.h"
#include "gpio.h"
#include "usart.h"

void PUT32(uint32_t, uint32_t);
unsigned int GET32(uint32_t);


void usart_init(usart_t *usart){
 /*Setting up TX*/
 usart->cr1 |= CR1_UE; //We only set the ones that we need, all others are set to 0
 //usart1->cr2 = CR2_STOP_1;
 //usart->cr1 = 0x340c; //We only set the ones that we need, all others are set to 0
 usart->cr2 = 0;
 usart->cr3 = 0;
 usart->gtpr = 0;

 usart->brr = 9000000 / 9600;
 usart->cr1 |= CR1_TE | CR1_RE;
 //usart1->cr1 = 0x200C;
}

void usart_putchar(usart_t *usart, uint8_t ch){
 while(! (usart->sr & 1 << 6));
 usart->dr = ch;
}

uint8_t usart_getchar(usart_t *usart){
 while(! (usart->sr & SR_RXNE));
  return usart->dr;
}

void usart_puts(usart_t *usart, uint8_t *str){
 while(*str){
  usart_putchar(usart, *str);
  str++;
 }
}

void usart_hex2ascii32(usart_t *usart, uint32_t val){
    usart_hex2ascii8(usart, (val >> 24) & 0xff);
    usart_hex2ascii8(usart, (val >> 16) & 0xff);
    usart_hex2ascii8(usart, (val >> 8) & 0xff);
    usart_hex2ascii8(usart, val & 0xff);
}

static void usart_hex2ascii(usart_t *usart, uint8_t val){
    if((val & 0x0f) < 10){
        val += 0x30;
    }else if((val & 0x0f) > 9 && (val & 0x0f) < 16){
        val += 55;
    }
    usart_putchar(usart, val);
}

void usart_hex2ascii8(usart_t *usart, uint8_t val){
    usart_hex2ascii(usart, (val >> 4) & 0x0f);
    usart_hex2ascii(usart, val & 0x0f);
}
