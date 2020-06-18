#ifndef RCC_H
#define RCC_H

#define RCCBASE    0x40021000

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

#include "globals.h"

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

void rcc_init();
void delay(uint32_t count);
void rcc_setup_cpu();

#endif
