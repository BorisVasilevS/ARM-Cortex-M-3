#include <MDR32Fx.h>
#include <stdint.h>
#include "uart.h"
#include "lcd.h"

//dma defines
#define DMA_CH_UART2_RX 3
#define dst_inc 30
#define dst_size 28
#define src_inc 26
#define src_size 24
#define dst_prot 21
#define src_prot 18
#define R_power 14
#define n_minus_1 4
#define nextBurst 3
#define cycle_ctrl 0

//uint8_t sign = 0;
uint32_t N = 1024;
uint8_t pic[1024] = {0};
int PRI_TBL[400] __attribute__ ((aligned(0x400)));
int elem = 0;
int flag = 0;

int main (void)
{
	MDR_RST_CLK->PER_CLOCK = 0xFFFFFFFF;
//MDR_PORTF -> OE |= (1 << 1);

	
//dma
	PRI_TBL[4 * (DMA_CH_UART2_RX - 1)] = (int)pic;
	PRI_TBL[4 * (DMA_CH_UART2_RX - 1) + 1] = (int)pic + N - 1;
	PRI_TBL[4 * (DMA_CH_UART2_RX - 1) + 2] = (0x0 << dst_inc) | (0 << dst_size) | (0x3 << src_inc) | (0 << src_size) | (0 << dst_prot) | (0 << src_prot) | (0xF << R_power) | ((N-1) << n_minus_1) | (0 << nextBurst) | (0x1 << cycle_ctrl);
	MDR_DMA -> CTRL_BASE_PTR = (int)PRI_TBL;
	MDR_DMA -> ALT_CTRL_BASE_PTR = (int)PRI_TBL + 0x200; 
	MDR_DMA -> CFG = DMA_CFG_MASTER_ENABLE;
	MDR_DMA -> CHNL_REQ_MASK_CLR = 1 << DMA_CH_UART2_RX;
	MDR_DMA -> CHNL_ENABLE_SET = 1 << DMA_CH_UART2_RX;
	MDR_DMA -> CHNL_PRI_ALT_CLR = 1 << DMA_CH_UART2_RX;
		
	
	
		initPerefirial();
		InitUART2(8000000, 115200);
		InitLcd();
		SetPage(0, 0);
		SetAddr(0, 0);
		clear_lcd();

	__enable_irq();
	
		
	while(1)
	{
//		while(flag == 0);
//		MDR_DMA -> CHNL_SW_REQUEST = 1 << DMA_CH_UART2_RX;
//		for(int i = 0; i < 15; i++);
		
		load_pic(pic);
		for(int i = 0; i < 10000; i++);
//		
	}
	
}

// interrapts 
void UART2_IRQHandler (void)
{
	MDR_DMA -> CHNL_SW_REQUEST = 1 << DMA_CH_UART2_RX;
	__disable_irq();
	flag = 1;
	NVIC->ICPR[0] |= (0x1 << 7);
	MDR_UART2 -> FR = 0;
	MDR_UART2 -> DR = 0x38;

}
