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

typedef i2c_t {
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

int startup(){
    uint16_t mcp_data = 0;
    gpio_t *gpio_c = (struct gpio_t *) GPIOCBASE;
    gpio_t *gpio_a = (struct gpio_t *) GPIOABASE;
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

    gpio_init(gpio_a, rcc, 5,  GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_ALT_PUSH);
    //gpio_init(gpio_a, rcc, 5,  GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_PUSH);
    gpio_init(gpio_a, rcc, 6,  GPIO_MODE_INPUT | GPIO_CNF_IN_PULL);
    gpio_init(gpio_a, rcc, 7,  GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_ALT_PUSH);

    //rcc->apb1enr |= 1;

    //Enable USART1 clock
    rcc->apb2enr |= (1 << 14) | 1;
    //Enable SPI1 clock
    rcc->apb2enr |= (1 << 12) | 1;
    //rcc->apb2enr |= 0x4000;
    // Disable Reset
    rcc->apb2rstr &= ~(1 << 14);
    rcc->apb2rstr &= ~(1 << 12);

    usart_init(usart);

    gpio_out(gpio_c, 13, 1);
    gpio_out(gpio_c, 14, 1);

    while(1){
        for(uint8_t i = 0; i < 16; i++){
            gpio_out(gpio_c, 13, 0);
            gpio_out(gpio_c, 14, 0);

            gpio_out(gpio_c, 13, 1);

            gpio_out(gpio_c, 14, 1);
            
            delay_ms(syt, 1000);
        }
    }

 return 0;
}
