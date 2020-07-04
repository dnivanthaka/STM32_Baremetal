/**
 * 
 */
#include "globals.h"
#include "rcc.h"
#include "gpio.h"
#include "usart.h"


void PUT32(uint32_t, uint32_t);
unsigned int GET32(uint32_t);

#define I2C1BASE 0x40005400

#define I2C_MODE_MASTER_SM    (0 << 15)
#define I2C_MODE_MASTER_FM    (1 << 15)
#define I2C_FM_MODE_DUTY_2    (0 << 14)
#define I2C_FM_MODE_DUTY_16_9 (1 << 14)


typedef struct i2c_t {
    volatile uint32_t cr1;
    volatile uint32_t cr2;
    volatile uint32_t oar1;
    volatile uint32_t oar2;
    volatile uint32_t dr;
    volatile uint32_t sr1;
    volatile uint32_t sr2;
    volatile uint32_t ccr;
    volatile uint32_t trise;
} i2c_t;


void i2c_init(i2c_t *i2c){
    //i2c->ccr |= I2C_MODE_MASTER_FM | I2C_FM_MODE_DUTY_2 | 0x28;
    //i2c->ccr |= I2C_MODE_MASTER_FM | I2C_FM_MODE_DUTY_2 | 0x168;
    i2c->ccr |= (1 << 15) | (0 << 14) | 0x28;
    //i2c->trise |= 0x51;
    //i2c->trise |= 0x09;

    //Set clock speed APB
    i2c->cr2 |= 0b01001;

    i2c->oar1 = 0 << 15;
    i2c->oar1 |= 1 << 14;
    i2c->oar2 |= 0b10101100;

    i2c->cr1 |= (1 << 10) | 1;
}

void i2c_write(i2c_t *i2c, uint8_t addr, uint8_t data){
    //wait for not-busy state
    while((i2c->sr2 & (1 << 1)));
    //Generate start condition
    i2c->cr1 |= (1 << 8);
    //Wait for master mode select flag and send address
    //while(! (i2c->sr1 & 1));
    //while(! (i2c->sr2 & 1));
    //i2c->dr = addr;
    //while(! (i2c->sr1 & (1 << 1)));
    //(void)i2c->sr2; // read sr2
    //Send data
    //i2c->dr = data;

    //while(! (i2c->sr1 & (1 << 7)));
    //asm("nop");
    //asm("nop");
    //Generate stop condition
    //i2c->cr1 |= (1 << 9);
    //while(i2c->sr2 & 1);
}

void i2c_close(i2c_t *i2c){
    i2c->cr1 &= ~(1);
}

void usart_dumpreg(usart_t *usart, uint8_t *str, uint32_t reg){
    usart_putchar(usart, '|');
    usart_puts(usart, str);
    usart_putchar(usart, '#');
    usart_hex2ascii32(usart, reg);
    usart_putchar(usart, 'H');
    usart_putchar(usart, '|');
}

int startup(){
    uint16_t mcp_data = 0;
    gpio_t *gpio_c = (struct gpio_t *) GPIOCBASE;
    gpio_t *gpio_a = (struct gpio_t *) GPIOABASE;
    gpio_t *gpio_b = (struct gpio_t *) GPIOBBASE;
    rcc_t  *rcc    = (struct rcc_t  *) RCCBASE;
    systick_t *syt = (struct systick_t *) SYSTICKBASE;
 
    usart_t *usart = (usart_t *) USART1BASE;

    i2c_t *i2c1 = (i2c_t *) I2C1BASE; 

    rcc_init(rcc);
    systick_init(syt);
 
    /*Enable PORT A, PA9 = TX, PA10 = RX*/
    gpio_init(gpio_a, rcc, 9,  GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_ALT_PUSH);
    //gpio_init(gpio_a, rcc, 9,  GPIO_MODE_OUT_50_MHZ, GPIO_CNF_OUT_PUSH);
    gpio_init(gpio_a, rcc, 10,  GPIO_MODE_INPUT | GPIO_CNF_IN_FLOAT);
    gpio_init(gpio_c, rcc, 13, GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_PUSH);
    gpio_init(gpio_c, rcc, 14, GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_PUSH);
    //gpio_init(gpio_a, rcc, 2,  GPIO_MODE_OUT_50_MHZ, GPIO_CNF_OUT_ALT_PUSH);

    gpio_init(gpio_b, rcc, 6,  GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_ALT_OPEN);
    gpio_init(gpio_b, rcc, 7,  GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_ALT_OPEN);
    //gpio_init(gpio_b, rcc, 6,  GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_PUSH);
    //gpio_init(gpio_b, rcc, 7,  GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_PUSH);

    //rcc->apb1enr |= 1;

    //Enable USART1 clock
    rcc->apb2enr |= (1 << 14) | 1;
    //Enable SPI1 clock
    rcc->apb1enr |= (1 << 21);
    //rcc->apb2enr |= 0x4000;


    // Reset Peripherals
    rcc->apb2rstr |= (1 << 14);
    rcc->apb2rstr &= ~(1 << 14);
    rcc->apb1rstr |= (1 << 21);
    rcc->apb1rstr &= ~(1 << 21);

    usart_init(usart);
    i2c_init(i2c1);

    gpio_out(gpio_c, 13, 1);
    //gpio_out(gpio_c, 14, 1);

    //i2c_write(i2c1, 0x20, 0x00);
    while(1){
        gpio_out(gpio_c, 13, 0);
        //gpio_out(gpio_b, 6, 0);
        //gpio_out(gpio_b, 7, 0);
        //uint8_t v = 0x5;
        //v + 0x30;
        /*usart_hex2ascii32(usart, usart->sr);
        usart_putchar(usart, '\r');
        usart_putchar(usart, '\n');
        usart_hex2ascii32(usart, i2c1->cr2);
        usart_putchar(usart, '\r');
        usart_putchar(usart, '\n');
        usart_hex2ascii32(usart, (i2c1 + 4));
        usart_putchar(usart, '\r');
        usart_putchar(usart, '\n');
        usart_hex2ascii32(usart, i2c1->sr2);
        usart_putchar(usart, '\r');
        usart_putchar(usart, '\n');
        */
        //volatile uint32_t *memory_count = (uint32_t *)0x1ffff7e0;
        usart_dumpreg(usart, "GPIO_CRL", gpio_b->cr[0]);
        usart_dumpreg(usart, "GPIO_CRH", gpio_b->cr[1]);
        usart_dumpreg(usart, "CR1", i2c1->cr1);
        usart_dumpreg(usart, "CR2", i2c1->cr2);
        i2c_write(i2c1, 0x20, 0x00);
        usart_dumpreg(usart, "SR1", i2c1->sr1);
        usart_dumpreg(usart, "SR2", i2c1->sr2);
        //usart_putchar(usart, 0x36);
        //usart_putchar(usart, 'B');
        usart_putchar(usart, '*');
        usart_putchar(usart, '\r');
        usart_putchar(usart, '\n');
        //i2c_write(i2c1, 0x20, 0x00);

        delay_ms(syt, 500);
        gpio_out(gpio_c, 13, 1);
        //gpio_out(gpio_b, 6, 1);
        //gpio_out(gpio_b, 7, 1);
        //i2c_write(i2c1, 0x20, 0xff);
            
        delay_ms(syt, 500);
    }

 return 0;
}

