typedef unsigned int uint32_t;

void PUT32(uint32_t, uint32_t);
unsigned int GET32(uint32_t);

#define GPIOCBASE 0x40011000
#define RCCBASE   0x40021000

//Systick timer access
#define STK_CTRL  0xE000E010
#define STK_LOAD  STK_CTRL + 0x04
#define STK_VAL   STK_CTRL + 0x08
#define STK_CALIB STL_CTRL + 0x0C

static int delay(uint32_t count){
    while(count--){
         while(1){
          uint32_t i = GET32(STK_CTRL);
          if(i && (1 << 16)){
           PUT32(STK_CTRL, 1);
           break;
          }
         }
    }

    return 0;
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

 //Systick timer init
 PUT32(STK_CTRL, 4);
 PUT32(STK_LOAD,100000000-1);
 PUT32(STK_VAL, 0);
 //Enable timer
 //PUT32(STK_CTRL, 5);
 PUT32(STK_CTRL, 1);
}

int startup(){
 init();

 while(1){
  PUT32(GPIOCBASE + 0x0f, 1 << (13 + 16));
  delay(1000);
  PUT32(GPIOCBASE + 0x10, 1 << 13);
  delay(1000);
 }

 return 0;
}
