typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

void PUT32(uint32_t, uint32_t);
unsigned int GET32(uint32_t);

#define GPIOBBASE  0x40010C00
#define GPIOCBASE  0x40011000
#define GPIOABASE  0x40010800
#define USART1BASE 0x40013800
#define RCCBASE    0x40021000


#define GPIO_CNF_IN_ANALOG    0x0
#define GPIO_CNF_IN_FLOAT     0x4
#define GPIO_CNF_IN_PULL      0x8
#define GPIO_CNF_RESERVED     0xC

#define GPIO_CNF_OUT_PUSH     0x0
#define GPIO_CNF_OUT_OPEN     0x4
#define GPIO_CNF_OUT_ALT_PUSH 0x8
#define GPIO_CNF_OUT_ALT_OPEN 0xC

#define GPIO_MODE_INPUT       0x0
#define GPIO_MODE_OUT_10_MHZ  0x1
#define GPIO_MODE_OUT_2_MHZ   0x2
#define GPIO_MODE_OUT_50_MHZ  0x3

#define GPIO_IOPAEN           1 << 2
#define GPIO_IOPBEN           1 << 3
#define GPIO_IOPCEN           1 << 4


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

#define PLL_LOCK	0x02000000

#define PLL_HSE		0x10000	/* 1 is HSE, 0 is HSI/2 */
#define PLL_HSI		0x00000
#define PLL_9		(7<<18)	/* multiply by 9 to get 72 Mhz */
#define	SYS_PLL		0x02	/* HSE multiplied by PLL */
#define APB1_DIV2	(4<<8)	/* 36 Mhz max */
#define	SYS_HSI		0x00	/* reset value - 8 Mhz internal RC */
#define	SYS_PLL		0x02

#define APB1_DIV2	(4<<8)

#define PLL_HSE		0x10000

#define HSI_ON		1
#define HSE_ON		0x10000
#define HSE_TRIM	0x80

#define CCR_NORM	(HSI_ON | HSE_ON | HSE_TRIM)

/* Bits in the clock control register CCR */
#define PLL_ENABLE	0x01000000
#define FLASH_ACR      ((volatile uint32_t *) 0x40022000)

typedef struct gpio_t {
    volatile uint32_t cr[2];
    volatile uint32_t idr;
    volatile uint32_t odr;
    volatile uint32_t bsrr;
    volatile uint32_t brr;
    volatile uint32_t lckr;
} gpio_t;

typedef struct rcc_t {
    volatile uint32_t cr;
    volatile uint32_t cfgr;
    volatile uint32_t cir;
    volatile uint32_t apb2rstr;
    volatile uint32_t abp1rstr;
    volatile uint32_t ahbenr;
    volatile uint32_t apb2enr;
    volatile uint32_t apb1enr;
    volatile uint32_t bdcr;
    volatile uint32_t csr;
    volatile uint32_t ahbrstr;
    volatile uint32_t cfgr2;
} rcc_t;


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
	while(count--){
        asm("nop");
	}
}

void gpio_init(gpio_t *gpio, rcc_t *rcc, uint8_t pin, uint32_t mode){
 uint8_t i = 0;

 if(gpio == (struct gpio_t *)GPIOABASE){
  rcc->apb2enr |= GPIO_IOPAEN;
 }else if(gpio == (struct gpio_t *)GPIOBBASE){
  rcc->apb2enr |= GPIO_IOPBEN;
 }else if(gpio == (struct gpio_t *)GPIOCBASE){
  rcc->apb2enr |= GPIO_IOPCEN;
 }

 if(pin > 8) i = 1;
 uint8_t shift = ((pin - (i * 8)) * 4);
 uint32_t prev = gpio->cr[i] & ~(0xf << shift);
 gpio->cr[i] = prev | (mode << shift);
 //gpio->cr[i] |= type << ((pin - (i * 8)) * 4) + 2);
}

void gpio_out(gpio_t *gpio, uint8_t pin, uint8_t value){
 uint8_t reg_val = 0;

 if(value == 0) reg_val = 16;
 gpio->bsrr = 1 << (pin + reg_val);
}

void init(){
 // Init RCC clocks
 /*
  //Initializes the CPU, AHB and APB busses clocks
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  //Initializes the CPU, AHB and APB busses clocks
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

 */



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
