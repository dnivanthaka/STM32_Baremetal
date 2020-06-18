#define USART1BASE 0x40013800

#define SR_TXE    (1 << 7)
#define SR_TC     (1 << 6)
#define CR1_UE    (1 << 13)
#define CR1_M     (1 << 12)
#define CR1_PCE   (1 << 10)
#define CR1_PS    (1 << 9)
#define CR1_PEIE  (1 << 8)
#define CR1_TXEIE  (1 << 7)
#define CR1_TCIE   (1 << 6)
#define CR1_TE    (1 << 3)
#define CR1_RE    (1 << 2)
#define CR1_SBK   1

#define CR2_STOP_1  0x00
#define CR2_STOP_2  0x10
#define CR2_CLKEN   (1 << 11)


#include "globals.h"
#include "rcc.h"
#include "gpio.h"

void PUT32(uint32_t, uint32_t);
unsigned int GET32(uint32_t);


typedef struct usart_t {
    volatile uint32_t sr;
    volatile uint32_t dr;
    volatile uint32_t brr;
    volatile uint32_t cr1;
    volatile uint32_t cr2;
    volatile uint32_t cr3;
    volatile uint32_t gtpr;
} usart_t;


usart_t *usart1 = (usart_t *) USART1BASE;


void init(){
 /*Setting up TX*/
 //usart1->cr1 |= CR1_UE; //We only set the ones that we need, all others are set to 0
 //usart1->cr2 = CR2_STOP_1;
 usart1->cr1 = 0x340c; //We only set the ones that we need, all others are set to 0
 usart1->cr2 = 0;
 usart1->cr3 = 0;
 usart1->gtpr = 0;

 usart1->brr = 9000000 / 9600;
 usart1->cr1 |= CR1_TE | CR1_RE;
 //usart1->cr1 = 0x200C;
}

int startup(){
 unsigned char tc = 'X';
 gpio_t *gpio_c = (struct gpio_t *) GPIOCBASE;
 gpio_t *gpio_a = (struct gpio_t *) GPIOABASE;
 rcc_t  *rcc    = (struct rcc_t  *) RCCBASE;

 rcc_init();
 
 //rcc->cr = 1 | (1  << 24) | 0x80;

 //rcc->cfgr |= PLL_HSI | SYS_HSI | APB1_DIV2;

 //rcc->cfgr |= PLL_HSE | PLL_9 | SYS_HSI | APB1_DIV2;

 //rcc->cr = CCR_NORM | PLL_ENABLE;

 //while ( ! (rcc->cr & PLL_LOCK ) )
	   ;
 //rcc->cfgr = PLL_HSE | PLL_9 | SYS_PLL | APB1_DIV2;

 //rcc->cfgr |= (0b100 << 11);

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

 init();

 while(1){
  gpio_out(gpio_c, 13, 1);
  //gpio_out(gpio_a, 9, 1);
  delay(1000000);
  while(! (usart1->sr & SR_TXE));
  usart1->dr = 'X';
  gpio_out(gpio_c, 13, 0);
  //gpio_out(gpio_a, 9, 0);
  delay(1000000);
 }

 return 0;
}
