/* p6_4.c UART0 Receive using interrupt */

/* This program modifies p4_2.c to use interrupt to handle the UART0 receive.
   Receiving any key from terminal emulator (TeraTerm) of the host PC to the UART0 on Freescale FRDM-KL25Z board.
   UART0 is connected to openSDA debug agent and has a virtual connection to the host PC COM port.
   Launch TeraTerm on a PC and hit any key.
   The LED program from P2_7 of Chapter 2 is used to turn on the tri-color LEDs according to the key received.
   By default in SystemInit(), FLL clock output is 41.94 MHz.
   Setting BDH=0, BDL=0x17, and OSR=0x0F yields 115200 Baud. */

#include <MKL25Z4.H>

void UART0_init(void);
void delayMs(int n);
void LED_init(void);
void LED_set(char value);
int i=0;
int main (void) {
    __disable_irq();        /* global disable IRQs */
    UART0_init();
    LED_init();
    __enable_irq();         /* global enable IRQs */

    while (1) { 
        /* UART0 receive is moved to interrupt handler*/
    }
}

/* UART0 interrupt handler */
void UART0_IRQHandler(void) 
{
    char c;

    c = UART0->D;           /* read the char received */
    LED_set(c);             /* and use it to set LEDs */
}

/* initialize UART0 to receive at 115200 Baud */
void UART0_init(void) 
	{
    SIM->SCGC4 |= 0x0400;    /* enable clock for UART0 */
    SIM->SOPT2 |= 0x04000000;   /* use FLL output for UART Baud rate generator */
    UART0->C2 = 0;          /* turn off UART0 while changing configurations */
    UART0->BDH = 0x00;
    UART0->BDL = 0x17;      /* 115200 Baud */
    UART0->C4 = 0x0F;       /* Over Sampling Ratio 16 */
    UART0->C1 = 0x00;       /* 8-bit data */
    UART0->C2 = 0x24;       /* enable receive and receive interrupt*/
    NVIC->ISER[0] |= 0x00001000;    /* enable INT12 (bit 12 of ISER[0]) */

    SIM->SCGC5 |= 0x0200;    /* enable clock for PORTA */
    PORTA->PCR[1] = 0x0200; /* make PTA1 UART0_Rx pin */
}

/* initialize all three LEDs on the FRDM board */
void LED_init(void)
{
    SIM->SCGC5 |= 0x400;        /* enable clock to Port B */
    SIM->SCGC5 |= 0x1000;       /* enable clock to Port D */
    PORTB->PCR[18] = 0x100;     /* make PTB18 pin as GPIO */
    PTB->PDDR |= 0x40000;       /* make PTB18 as output pin */
    PTB->PSOR |= 0x40000;       /* turn off red LED */
    PORTB->PCR[19] = 0x100;     /* make PTB19 pin as GPIO */
    PTB->PDDR |= 0x80000;       /* make PTB19 as output pin */
    PTB->PSOR |= 0x80000;       /* turn off green LED */
    PORTD->PCR[1] = 0x100;      /* make PTD1 pin as GPIO */
    PTD->PDDR |= 0x02;          /* make PTD1 as output pin */
    PTD->PSOR |= 0x02;          /* turn off blue LED */
}

/* turn on or off the LEDs according to bit 2-0 of the value */
void LED_set(char value)
{
  
    if (value=='c') 
		{			/* use bit 1 of value to control green LED */
        PTB->PCOR = 0x80000;    /* turn on green LED */
s		}
		if (~(value=='c') ) 
		{			/* use bit 1 of value to control green LED */
      PTB->PSOR = 0x80000;    /* turn off green LED */
	
		}

  
}