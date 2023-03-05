#include <MDR32Fx.h>
#include <stdint.h>
#include "uart.h"

//lcd defines
#define RESET_LCD 10
#define CMD_E0 (int*)0x50100000
#define CMD_E1 (int*)0x50200000
#define Data_E0 (int*)0x58100000
#define Data_E1 (int*)0x58200000
#define LCD_ON 1
#define LCD_OFF 0
#define E0 4
#define E1 5
#define A0 11


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
int PRI_TBL[256] __attribute__ ((aligned(0x400)));
int elem = 0;
int flag = 0;
void LCDON(int Status);
void InitLcd(void);
void SetPage(int Page, int crystal);
void SetAddr(int Addr, int crystal);
void Send_Data(int Data, int crystal);
void clear_lcd(void);
void load_pic(uint8_t* pic);
void WriteCMD(int crystal, int CMD);


int main (void)
{
	MDR_RST_CLK->PER_CLOCK = 0xFFFFFFFF;
//	MDR_PORTF -> OE |= (1 << 1);
	//lcd ports
	//init external bus contoller
	MDR_EBC->CONTROL |= (EBC_CONTROL_WAIT_STATE_Msk)|(EBC_CONTROL_CPOL)|(EBC_CONTROL_RAM);
	//portA - DATA
	MDR_PORTA->FUNC |= (1<<PORT_FUNC_MODE0_Pos)|(1<<PORT_FUNC_MODE1_Pos)|(1<<PORT_FUNC_MODE2_Pos)|(1<<PORT_FUNC_MODE3_Pos)|(1<<PORT_FUNC_MODE3_Pos)|(1<<PORT_FUNC_MODE4_Pos)|(1<<PORT_FUNC_MODE5_Pos)|(1<<PORT_FUNC_MODE6_Pos)|(1<<PORT_FUNC_MODE7_Pos);
	MDR_PORTA->ANALOG |= 0XFF;
	MDR_PORTA->PWR |= (0x3<<PORT_PWR0_Pos)|(0x3<<PORT_PWR1_Pos)|(0x3<<PORT_PWR2_Pos)|(0x3<<PORT_PWR3_Pos)|(0x3<<PORT_PWR4_Pos)|(0x3<<PORT_PWR5_Pos)|(0x3<<PORT_PWR6_Pos)|(0x3<<PORT_PWR7_Pos);
	//pc2-WE,PC7 - E,
	MDR_PORTC->FUNC |= (1<<PORT_FUNC_MODE2_Pos)|(1<<PORT_FUNC_MODE7_Pos);
	MDR_PORTC->ANALOG |= (1<<2)|(1<<7);
	MDR_PORTC->PWR |= (0x3<<PORT_PWR2_Pos)|(0x3<<PORT_PWR7_Pos);
	MDR_PORTC->OE |=(1<<2)|(1<<7);
	//PE4-E0,PE5,PE11, PE10 - RESET
	MDR_PORTE->FUNC |= (0<<PORT_FUNC_MODE4_Pos)|(0<<PORT_FUNC_MODE5_Pos)|(1<<PORT_FUNC_MODE11_Pos)|(0<<PORT_FUNC_MODE10_Pos);
	MDR_PORTE->ANALOG |= (1<<4)|(1<<5)|(1<<11)|(1<<10);
	MDR_PORTE->PWR |= (0x3<<PORT_PWR4_Pos)|(0x3<<PORT_PWR5_Pos)|(0x3<<PORT_PWR11_Pos)|(0x3<<PORT_PWR10_Pos);
	MDR_PORTE->OE |=(1<<4)|(1<<5)|(1<<11)|(1<<10);
	//MDR_PORTD->OE &= ~(1<<10);
	MDR_PORTD->OE |=(1<<10);
	MDR_PORTD->FUNC |= (1<<PORT_FUNC_MODE10_Pos);
	MDR_PORTD->ANALOG |= (1<<10);
	MDR_PORTD->PD &= ~(1<<10);
	MDR_PORTD->PWR |= (0x3<<PORT_PWR10_Pos);

	MDR_PORTC->FUNC |= (0x2<<PORT_FUNC_MODE12_Pos);
	MDR_PORTC->ANALOG |= (1<<12);
	MDR_PORTC->PD &= ~(1<<12);
	MDR_PORTC->GFEN |= (1<<12);
	
	InitUART2(8000000, 115200);
	
//dma
PRI_TBL[4 * DMA_CH_UART2_RX ] = 0x40038000;
PRI_TBL[4 * DMA_CH_UART2_RX + 1] = (int)pic + N - 1;

PRI_TBL[4 * DMA_CH_UART2_RX + 2] = (0x0 << dst_inc) | (0 << dst_size) | (0x3 << src_inc) | (0 << src_size) | (0 << dst_prot) | (0 << src_prot) | (0xF << R_power) | ((N-1) << n_minus_1) | (0 << nextBurst) | (0x1 << cycle_ctrl);
MDR_DMA -> CTRL_BASE_PTR = (int)PRI_TBL;
MDR_DMA -> ALT_CTRL_BASE_PTR = (int)PRI_TBL + 0x200; 
MDR_DMA -> CFG = DMA_CFG_MASTER_ENABLE;
MDR_DMA -> CHNL_REQ_MASK_CLR = 1 << DMA_CH_UART2_RX;
MDR_DMA -> CHNL_ENABLE_SET = 1 << DMA_CH_UART2_RX;
MDR_DMA -> CHNL_PRI_ALT_CLR = 1 << DMA_CH_UART2_RX;
		
		
		InitLcd();
		SetPage(0, 0);
		SetAddr(0, 0);
		clear_lcd();

	__enable_irq();
	
	
		
	while(1)
	{
		while(flag == 0);
		MDR_DMA -> CHNL_SW_REQUEST = 1 << DMA_CH_UART2_RX;
		for(int i = 0; i < 15; i++);
		
	}
	
}
//Subprograms
void load_pic(uint8_t* pic)
{
	int cnt = 0;
	for (int j=0; j<2;j++)
	{
		cnt = (j == 0) ? 0 : 64;
		for(int i =0; i<8;i++)
		{
			SetAddr(0, j);
			SetPage(i,j);
			for(int k=0;k<64;k++)
			{
				Send_Data(pic[cnt],j);
				cnt++;
			}
		cnt = cnt + 64;
		}
	}
	cnt = 0;
}	

void start_line(int startline)
{
	int	CMD;
	CMD = ((startline&0x3F) + 0xC0);
	WriteCMD(0, CMD);
}

void InitLcd(void)
{
	int Status = 0;
	MDR_PORTE-> RXTX &= ~(1<<RESET_LCD);
	 for(int i=0;i<50;i++);
	MDR_PORTE-> RXTX |= (1<<RESET_LCD);
		for(int i=0;i<500;i++);
	Status = *((int*)0x50200000);
	
	while((ReadCMD(0))&((1<<7)|(1<<4)));
	while((ReadCMD(1))&((1<<7)|(1<<4)));
	LCDON(LCD_ON);
	
	
}

int ReadCMD(int crystal)
{

	if(crystal)
	{
		MDR_PORTE->RXTX |= 1<<E1;
		MDR_PORTE->RXTX &= ~(1<<E0);
	}
	else
	{
		MDR_PORTE->RXTX |= 1<<E0;
		MDR_PORTE->RXTX &= ~(1<<E1);
	}

	return *CMD_E0;
}

void WriteCMD(int crystal, int CMD)
{

	if(crystal)
	{
		MDR_PORTE->RXTX |= 1<<E1;
		MDR_PORTE->RXTX &= ~(1<<E0);
	}
	else
	{
		MDR_PORTE->RXTX |= 1<<E0;
		MDR_PORTE->RXTX &= ~(1<<E1);
	}

  *CMD_E0 = CMD;
}

void WriteData(int crystal, int Data)
{
	if(crystal)
	{
		MDR_PORTE->RXTX |= 1<<E1;
		MDR_PORTE->RXTX &= ~(1<<E0);
	}
	else
	{
		MDR_PORTE->RXTX |= 1<<E0;
		MDR_PORTE->RXTX &= ~(1<<E1);
	}

  *Data_E0 = Data;
}

void clear_lcd(void)
{	for (int j=0; j<2;j++){
		for(int i =0; i<8;i++){
			SetPage(i,j);
			for(int k=0;k<64;k++){
				Send_Data(0x00,j);
			}
		}
	}
}

void LCDON(int Status)
{
	if(Status)
	{	
		WriteCMD(0, 0x3F);
		WriteCMD(1, 0x3F);
	}
	else
	{
		WriteCMD(0, 0x3E);
		WriteCMD(1, 0x3E);
	}
		
}

void SetPage(int Page, int crystal)
{
	int CMD;
	CMD =((Page&0x7) + 0xB8);
	
	if(crystal)
			WriteCMD(1, CMD);
	else
			WriteCMD(0, CMD);
}

void SetAddr(int Addr, int crystal)
{
	if(crystal)
		WriteCMD(1, 0x40 + (Addr&0x3F));
	else
		WriteCMD(0, 0x40 + (Addr&0x3F));
}

void Send_Data(int Data, int crystal)
{
		MDR_PORTE->RXTX |= (1<<A0);

	if(crystal)
	{
		MDR_PORTE->RXTX |= 1<<E1;
		MDR_PORTE->RXTX &= ~(1<<E0);
	}
	else
	{
		MDR_PORTE->RXTX |= 1<<E0;
		MDR_PORTE->RXTX &= ~(1<<E1);
	}

  *Data_E0 = Data;
}
// interrapts 
void UART2_IRQHandler (void)
{
	flag = 1;
	NVIC->ICPR[0] |= (0x1 << 7);
	MDR_UART2 -> FR = 0;
//		if (MDR_UART2 -> MIS & UART_MIS_RXMIS)
//		{
//			sign = MDR_UART2 -> DR ; 
//			pic[elem] = sign;
//			elem++;
//		}
//		if(elem == 1023)
//			load_pic(pic);
}
