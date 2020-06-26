#include "globals.h"
#include "rcc.h"
#include "gpio.h"
#include "usart.h"


#define SPI1BASE       0x40013000

#define SPI_CLK_DIV2   0b000
#define SPI_CLK_DIV4   0b001
#define SPI_CLK_DIV8   0b010
#define SPI_CLK_DIV16  0b011
#define SPI_CLK_DIV32  0b100
#define SPI_CLK_DIV64  0b101
#define SPI_CLK_DIV128 0b110
#define SPI_CLK_DIV256 0b111

#define SPI_CPOL_0     0
#define SPI_CPOL_1     1

#define SPI_CPHA_1     0
#define SPI_CPHA_2     1

#define SPI_DFF_8BIT   0
#define SPI_DFF_16BIT  1

#define SPI_ENDIAN_MSB 0
#define SPI_ENDIAN_LSB 1

#define SPI_SLAVE_MGMT_ENABLE  (0 << 1)
#define SPI_SLAVE_MGMT_DISABLE (1 << 1)
#define SPI_INTERNAL_SS        1

#define SPI_SLAVE      0
#define SPI_MASTER     1

#define SPI_SR_TXE     0x02
#define SPI_SR_RXNE    1

#define SPI_MODE_UNI2  (0 << 1)
#define SPI_MODE_BI1   (1 << 1)

#define SPI_OUTPUT_DISABLE 0
#define SPI_OUTPUT_ENABLE  1

void PUT32(uint32_t, uint32_t);
unsigned int GET32(uint32_t);


typedef struct spi_t {
    volatile uint32_t cr1;
    volatile uint32_t cr2;
    volatile uint32_t sr;
    volatile uint32_t dr;
    volatile uint32_t cpcpr;
    volatile uint32_t rxcrcr;
    volatile uint32_t txcrcr;
    volatile uint32_t i2scfgr;
    volatile uint32_t i2spr;
} spi_t;

void spi_init(spi_t *spi, uint8_t mode, uint8_t dir, uint8_t frame_format, 
        uint8_t baud, uint8_t idle_polarity, uint8_t clk_phase, uint8_t slave_mgmt){

    //spi->cr1 = (baud << 3) | (idle_polarity << 1) | clk_phase | (slave_mgmt << 8) | (mode << 2) | (frame_format << 7) | (dir << 14);
    spi->cr1 = (0b010 << 3) | (0 << 1) | 0 | (0b00 << 8) | (1 << 2) | (1 << 7) | (0b01 << 14);
    //Enable the peripheral
    spi->cr1 |= (1 << 6);
}

void spi_write(spi_t *spi, uint16_t data){
    //Clear the overrun flag
    spi->sr &= ~(1 << 6);
    spi->dr = data;
    while(! (spi->sr & SPI_SR_TXE));
}

uint8_t spi_read(spi_t *spi){
    while(! (spi->sr & SPI_SR_RXNE));
    return spi->dr;
}

uint8_t spi_transfer(spi_t *spi, uint16_t data){
    spi->dr = data;
    while(! (spi->sr & SPI_SR_RXNE));
    return spi->dr;
}

void spi_close(spi_t *spi){
    spi->cr1 &= ~(1 << 6);
}

void spi_mcp23s17_write(spi_t *spi, uint8_t addr, uint8_t reg, uint8_t val){
     spi_transfer(spi, addr);
     spi_transfer(spi, reg);
     spi_transfer(spi, val);
}

int startup(){
    unsigned char tc;
    uint16_t mcp_data = 0;
    gpio_t *gpio_c = (struct gpio_t *) GPIOCBASE;
    gpio_t *gpio_a = (struct gpio_t *) GPIOABASE;
    rcc_t  *rcc    = (struct rcc_t  *) RCCBASE;
    systick_t *syt = (struct systick_t *) SYSTICKBASE;
 
    usart_t *usart = (usart_t *) USART1BASE;

    spi_t   *spi1 = (spi_t *)SPI1BASE;

    rcc_init(rcc);
    systick_init(syt);
 
    /*Enable PORT A, PA9 = TX, PA10 = RX*/
    gpio_init(gpio_a, rcc, 9,  GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_ALT_PUSH);
    //gpio_init(gpio_a, rcc, 9,  GPIO_MODE_OUT_50_MHZ, GPIO_CNF_OUT_PUSH);
    gpio_init(gpio_a, rcc, 10,  GPIO_MODE_INPUT | GPIO_CNF_IN_FLOAT);
    gpio_init(gpio_c, rcc, 13, GPIO_MODE_OUT_50_MHZ | GPIO_CNF_OUT_PUSH);
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

    spi_init(spi1, SPI_MASTER, SPI_MODE_UNI2 | SPI_OUTPUT_ENABLE, 
            SPI_DFF_8BIT, SPI_CLK_DIV8, SPI_CPOL_1, SPI_CPHA_1, SPI_SLAVE_MGMT_DISABLE);


    //usart_putchar(usart, (spi1->cr1 & 0xff));
    //usart_putchar(usart, ((spi1->cr1 >> 4 ) & 0xff));
    //usart_putchar(usart, '\n');
    //spi_mcp23s17_write(spi1, 0b01000000, 0x00, 0x00);
    gpio_out(gpio_c, 13, 1);
    while(1){
        //gpio_out(gpio_c, 13, 1);
        gpio_out(gpio_c, 13, 0);
        //spi_mcp23s17_write(spi1, 0b01000000, 0x12, 0xff);
        //spi_write(spi1, 0b01000001);
        spi_write(spi1, 0xff);
        spi_read(spi1);
        //spi_write(spi1, 0x00);
        //uint8_t val = spi_read(spi1);
        gpio_out(gpio_c, 13, 1);
        //if(val != 0b01000001)
        //    continue;
        //usart_putchar(usart, val);
        //usart_putchar(usart, '\r');
        //usart_putchar(usart, '\n');
        delay_ms(syt, 1000);
        gpio_out(gpio_c, 13, 0);
        spi_write(spi1, 0);
        spi_read(spi1);
        //val = spi_read(spi1);
        //gpio_out(gpio_c, 13, 0);
        gpio_out(gpio_c, 13, 1);
        //spi_mcp23s17_write(spi1, 0b01000000, 0x12, 0x00);
        delay_ms(syt, 1000);
    }

 return 0;
}
