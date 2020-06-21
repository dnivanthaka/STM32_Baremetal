/*
 Testing the systick timer, this is not the proper way since there is
 only one systick we could read the values directly
*/

#include "globals.h"
#include "rcc.h"
#include "gpio.h"
#include "usart.h"

#define SYSTICKBASE 0xE000E010

#define SYT_COUNTF   (1 << 16)
#define SYT_SRC      (1 << 2)
#define SYT_ENABLE   (1)

void PUT32(uint32_t, uint32_t);
unsigned int GET32(uint32_t);

/*
 This function would initialize the registers but not activate the counter
 Counter would be started by the delay function
*/
void systick_init(systick_t *syt){
 //Here the calibration value should be set according the cpu clock, for now its set to 9Mhz
 syt->ctrl &= ~SYT_ENABLE;
 syt->ctrl &= ~SYT_COUNTF;
 syt->ctrl &= ~SYT_SRC;

 syt->calib = 0x0002328;        //9Mhz

 syt->load  = 0;
}

void delay_ms(systick_t *syt, uint32_t val){
 //syt->load = 0xffff - 1;
 syt->load = 0xfff - 1;
 syt->val = 0;
 syt->ctrl &= ~SYT_COUNTF;
 syt->ctrl |= SYT_ENABLE;

 while(val--){
  while(! (syt->ctrl & SYT_COUNTF));
 }

 //Stop the counter
 syt->ctrl &= ~SYT_ENABLE;
}

int startup(){
 unsigned char tc;
 gpio_t *gpio_c = (struct gpio_t *) GPIOCBASE;
 rcc_t  *rcc    = (struct rcc_t  *) RCCBASE;

 systick_t *syt = (struct systick_t *) SYSTICKBASE;

 rcc_init(rcc);
 
 gpio_init(gpio_c, rcc, 13, GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_PUSH);


 while(1){
  gpio_out(gpio_c, 13, 1);
  delay_ms(syt, 1000);
  gpio_out(gpio_c, 13, 0);
  delay_ms(syt, 1000);
 }

 return 0;
}
