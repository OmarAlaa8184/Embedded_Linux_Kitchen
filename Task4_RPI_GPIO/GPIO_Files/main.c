
/*#include <stdio.h>*/

#define GPIO_BASE  0x3F200000                                     // Base address for GPIO registers
#define GPFSEL0    ((volatile unsigned int *)(GPIO_BASE + 0x00))  // GPIO function select
#define GPSET0     ((volatile unsigned int *)(GPIO_BASE + 0x1C))  // GPIO set register
#define GPCLR0     ((volatile unsigned int *)(GPIO_BASE + 0x28))  // GPIO clear register


void main(void)
{
    // Set GPIO4 as output (bits 12-14 in GPFSEL0)
    *GPFSEL0 &= ~(7 << 12);  // Clear bits (reset to 000)
    *GPFSEL0 |= (1 << 12);   // Set as output (binary 001)

    while (1)
    {
        *GPSET0 = (1 << 4);  // Turn GPIO4 ON (set high)
    
   	   /* for (int i = 0; i < 10000000; i++);       
   
     	*GPCLR0 = (1 << 4);  // Turn GPIO4 OFF (set low)
   
    	for (int i = 0; i < 10000000; i++);*/

    }
}

