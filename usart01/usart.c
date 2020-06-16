typedef unsigned int uint32_t;

void PUT32(uint32_t, uint32_t);
unsigned int GET32(uint32_t);

#define GPIOCBASE  0x40011000
#define GPIOABASE  0x40010800
#define USART1BASE 0x40013800
#define RCCBASE    0x40021000

#define RCC_APB2ENR_IOPAEN (1 << 2)

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
#define CR1_SBK   1

#define CR2_STOP_1  0x00
#define CR2_STOP_2  0x10
#define CR2_CLKEN   (1 << 11)


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

void delay(uint32_t count){
	while(count--);
}

void init(){
 /*Enable PORT C*/
 uint32_t abp2_reg = GET32(RCCBASE + 0x18);
 abp2_reg |= 1 << 4;
 PUT32(RCCBASE + 0x18, abp2_reg);

 uint32_t crl_reg = GET32(GPIOCBASE + 0x04);
 crl_reg |= 3 << 20;       /*PORTC 13*/
 crl_reg |= 3 << 20;
 PUT32(GPIOCBASE + 0x04, crl_reg);

 /*Enable PORT A, PA9 = TX, PA10 = RX*/
 abp2_reg = GET32(RCCBASE + 0x18);
 abp2_reg |= RCC_APB2ENR_IOPAEN;
 PUT32(RCCBASE + 0x18, abp2_reg);

 crl_reg = GET32(GPIOABASE + 0x04);
 crl_reg |= 3 << 4;       /*PORTA 9*/
 crl_reg |= 2 << 6;
 PUT32(GPIOABASE + 0x04, crl_reg);

 /*Setting up TX*/
 usart1->cr1 = CR1_UE; //We only set the ones that we need, all others are set to 0 
 usart1->cr2 = CR2_STOP_1;
 usart1->cr3 = 0;

 usart1->brr = 0x0045;
 usart1->cr1 |= CR1_TE;
}

int startup(){
 unsigned char tc = 'X';
	
 init();

 while(1){
  PUT32(GPIOCBASE + 0x10, 1 << 13);
  delay(100000);
  usart1->dr = tc;
  while(! usart1->sr && SR_TXE);
  PUT32(GPIOCBASE + 0x10, 1 << (13 + 16));
  delay(100000);
 }

 return 0;
}
