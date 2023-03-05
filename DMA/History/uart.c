#include <MDR32Fx.h>
#include "uart.h"
void InitUART2(int freq, int BAUD) 	
{
	
	int I; //????? ?????
	double Frac; //???????
	
	MDR_PORTF -> OE |= 0x2; //RX ?? ???? TX ?? ?????
	MDR_PORTF -> FUNC |= 0xF;
	MDR_PORTF -> ANALOG |= 0x3;
	MDR_PORTF -> PD &= ~0x3;
	MDR_PORTF -> PWR |= 0x2 << 2;
		
	MDR_RST_CLK -> UART_CLOCK |= RST_CLK_UART_CLOCK_UART2_CLK_EN;
	
	I = freq/(16*BAUD);
	MDR_UART2->IBRD = I;
	Frac = (((double)freq)/((double)16*BAUD) - (double)I)*64 + 0.5;
	MDR_UART2->FBRD = (int)Frac;
	/*MDR_UART2->LCR_H |= (3 << UART_LCR_H_WLEN_Pos)|(1 << UART_LCR_H_FEN_Pos);
	MDR_UART2->IFLS |= (1 << UART_IFLS_TXIFLSEL_Pos);
	MDR_UART2->IFLS &= ~(UART_IFLS_RXIFLSEL_Msk);
  MDR_UART2->IMSC |= (1 << UART_IMSC_TXIM_Pos)|(1 << UART_IMSC_RXIM_Pos);	
	MDR_UART2->CR |= (1 << UART_CR_TXE_Pos)|(1 << UART_CR_RXE_Pos)|(1 << UART_CR_UARTEN_Pos);*/
	
	
	MDR_UART2 -> LCR_H = (0x3 << UART_LCR_H_WLEN_Pos);
	MDR_UART2 -> CR = UART_CR_RXE | UART_CR_TXE;
	MDR_UART2 -> IMSC = 1 << 4;
	MDR_UART2 -> CR |= UART_CR_UARTEN;
		
	NVIC -> ISER [0] |= (0x1 << 7);
}