typedef unsigned int uint32_t;

void PUT32(uint32_t, uint32_t);
unsigned int GET32(uint32_t);

#define GPIOCBASE 0x40011000
#define RCCBASE   0x40021000

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
}

int startup(){
 init();

 while(1){
  PUT32(GPIOCBASE + 0x10, 1 << 13);
  delay(100000);
  PUT32(GPIOCBASE + 0x10, 1 << (13 + 16));
  delay(100000);
 }

 return 0;
}
