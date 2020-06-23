#include "globals.h"
#include "rcc.h"
#include "gpio.h"
#include "usart.h"

void PUT32(uint32_t, uint32_t);
unsigned int GET32(uint32_t);


int startup(){
 unsigned char tc;
 gpio_t *gpio_c = (struct gpio_t *) GPIOCBASE;
 gpio_t *gpio_a = (struct gpio_t *) GPIOABASE;
 rcc_t  *rcc    = (struct rcc_t  *) RCCBASE;
 systick_t *syt = (struct systick_t *) SYSTICKBASE;
 
 usart_t *usart = (usart_t *) USART1BASE;

 rcc_init(rcc);
 systick_init(syt);
 
 /*Enable PORT A, PA9 = TX, PA10 = RX*/
 gpio_init(gpio_a, rcc, 9,  GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_ALT_PUSH);
 //gpio_init(gpio_a, rcc, 9,  GPIO_MODE_OUT_50_MHZ, GPIO_CNF_OUT_PUSH);
 gpio_init(gpio_a, rcc, 10,  GPIO_MODE_INPUT | GPIO_CNF_IN_FLOAT);
 gpio_init(gpio_c, rcc, 13, GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_PUSH);
 //gpio_init(gpio_a, rcc, 2,  GPIO_MODE_OUT_50_MHZ, GPIO_CNF_OUT_ALT_PUSH);

  //rcc->apb1enr |= 1;

 //Enable USART1 clock
 rcc->apb2enr |= (1 << 14) | 1;
 //rcc->apb2enr |= 0x4000;
 rcc->apb2rstr &= ~(1 << 14);

 usart_init(usart);

 while(1){
  gpio_out(gpio_c, 13, 1);
  //gpio_out(gpio_a, 9, 1);
  delay_ms(syt, 1000);
  //while(! (usart->sr & SR_TXE));
  //usart->dr = 'X';
  //usart_putchar(usart, 'X');
  tc = usart_getchar(usart);
  usart_puts(usart, &tc);
  gpio_out(gpio_c, 13, 0);
  //gpio_out(gpio_a, 9, 0);
  delay_ms(syt, 1000);
 }

 return 0;
}
