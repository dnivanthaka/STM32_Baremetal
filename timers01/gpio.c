#include "globals.h"
#include "rcc.h"
#include "gpio.h"

void gpio_init(gpio_t *gpio, rcc_t *rcc, uint8_t pin, uint8_t mode){
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
 //first unset the previous mode
 uint32_t prev = gpio->cr[i] & ~(0xf << shift);
 gpio->cr[i] = prev | (mode << shift);
}



void gpio_out(gpio_t *gpio, uint8_t pin, uint8_t value){
 uint8_t reg_val = 0;

 if(value > 0) reg_val = 16;
 gpio->bsrr = 1 << (pin + reg_val);
}
