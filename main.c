/*
* Copyright (c) 2012, Mauro Scomparin
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of Mauro Scomparin nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY Mauro Scomparin ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL Mauro Scomparin BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* File:			main.c.
* Author:		Mauro Scomparin <http://scompoprojects.worpress.com>.
* Version:		1.0.0.
* Description:	Main sample file.
*/

#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"

#include <stdio.h>
#include <stdlib.h>

// Basically here I'm checking that everything works fine.
volatile unsigned long count;

// An interrupt function.
void Timer1A_ISR(void);

// main function.
int main(void) {
	SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

  SysCtlPeripheralEnable( SYSCTL_PERIPH_UART0);
  UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
                     (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8));
  UARTEnable(UART0_BASE);


  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  GPIOPinConfigure(GPIO_PA0_U0RX);
  GPIOPinConfigure(GPIO_PA1_U0TX);
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

  unsigned char* test = malloc(1024); // ask for a kb 
  int i;
  for (i=0; i<10; i++)
    test[i] = i+0x30; 
  test[i]=0;
  printf("test string: %s\r\n", test);
  free(test); // didnt want it...   

  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	TimerConfigure(TIMER1_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC);
	TimerControlStall(TIMER1_BASE, TIMER_A, true);
	TimerLoadSet(TIMER1_BASE, TIMER_A, 2111);
	TimerIntRegister(TIMER1_BASE, TIMER_A, Timer1A_ISR);
	TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	count=3;
	TimerEnable(TIMER1_BASE, TIMER_A);
    while(1)
    {	
		
    }
}

// The interrupt function definition.
void Timer1A_ISR(void){
	TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
  printf("Timer interrupt %d\r\n",count);
	count++;
}
