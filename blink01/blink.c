typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

void PUT32(uint32_t, uint32_t);
unsigned int GET32(uint32_t);

#define GPIOABASE 0x40010800
#define GPIOBBASE 0x40010C00
#define GPIOCBASE 0x40011000

#define RCCBASE   0x40021000

#define GPIO_CNF_IN_ANALOG    0
#define GPIO_CNF_IN_FLOAT     0x01
#define GPIO_CNF_IN_PULL      0x10
#define GPIO_CNF_RESERVED     0x11

#define GPIO_CNF_OUT_PUSH     0
#define GPIO_CNF_OUT_OPEN     0x01
#define GPIO_CNF_OUT_ALT_PUSH 0x10
#define GPIO_CNF_OUT_ALT_OPEN 0x11

#define GPIO_MODE_INPUT       0
#define GPIO_MODE_OUT_10_MHZ  0x01
#define GPIO_MODE_OUT_2_MHZ   0x10
#define GPIO_MODE_OUT_50_MHZ  0x11

#define GPIO_IOPAEN           1 << 2
#define GPIO_IOPBEN           1 << 3
#define GPIO_IOPCEN           1 << 4


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

void delay(uint32_t count){
	while(count--);
}

void gpio_init(gpio_t *gpio, rcc_t *rcc, uint8_t pin, uint8_t mode, uint8_t type){
 uint8_t i = 0;
 
 if(gpio == (struct gpio_t *)GPIOABASE){
  rcc->apb2enr |= GPIO_IOPAEN;
 }else if(gpio == (struct gpio_t *)GPIOBBASE){
  rcc->apb2enr |= GPIO_IOPBEN;
 }else if(gpio == (struct gpio_t *)GPIOCBASE){
  rcc->apb2enr |= GPIO_IOPCEN;
 }

 if(pin > 8) i = 1;
 gpio->cr[i] |= mode << (pin - (i * 8)) * 4;
 gpio->cr[i] |= type << ((pin - (i * 8)) * 4) + 2;
}

void gpio_out(gpio_t *gpio, uint8_t pin, uint8_t value){
 uint8_t reg_val = 0;

 if(value == 0) reg_val = 16;
 gpio->bsrr = 1 << (pin + reg_val);
}

int startup(){
 gpio_t *gpio_c = (struct gpio_t *) GPIOCBASE;
 gpio_t *gpio_a = (struct gpio_t *) GPIOABASE;
 rcc_t  *rcc    = (struct rcc_t  *) RCCBASE;

 /*Enable PORT C, pin 13*/
 gpio_init(gpio_c, rcc, 13, GPIO_MODE_OUT_50_MHZ, GPIO_CNF_OUT_PUSH);
 gpio_init(gpio_a, rcc, 2,  GPIO_MODE_OUT_50_MHZ, GPIO_CNF_OUT_PUSH);

 while(1){
  gpio_out(gpio_c, 13, 1);
  gpio_out(gpio_a, 2, 1);
  delay(100000);
  gpio_out(gpio_c, 13, 0);
  gpio_out(gpio_a, 2,  0);
  delay(100000);
 }

 return 0;
}
