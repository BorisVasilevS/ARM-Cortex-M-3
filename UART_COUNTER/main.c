/*
//includes
#include <MDR32Fx.h>
#include <stdint.h>
//init variable&contants
uint32_t flag = 0;
//init function prototype
void set_speed(uint8_t x);

uint16_t sign = 0;
uint16_t counter = 0;
//main function
int main(void)
{
	 uint8_t speed = 0;
	//init rst_clk
	
	MDR_RST_CLK->HS_CONTROL = RST_CLK_HS_CONTROL_HSE_ON;// turn on hse
	while (!(MDR_RST_CLK->CLOCK_STATUS & RST_CLK_CLOCK_STATUS_HSE_RDY));// wait for hse stability
	MDR_RST_CLK->CPU_CLOCK |= 2<<RST_CLK_CPU_CLOCK_CPU_C1_SEL_Pos;//select hse c1
	MDR_RST_CLK->PLL_CONTROL = (9<<RST_CLK_PLL_CONTROL_PLL_CPU_MUL_Pos);//select multiplier 10
	MDR_RST_CLK->PLL_CONTROL |= RST_CLK_PLL_CONTROL_PLL_CPU_ON;//turn on pll
	MDR_RST_CLK->PLL_CONTROL |= RST_CLK_PLL_CONTROL_PLL_CPU_PLD;//
	MDR_RST_CLK->PLL_CONTROL &= ~(RST_CLK_PLL_CONTROL_PLL_CPU_PLD);//reloading pll
	while (!(MDR_RST_CLK->CLOCK_STATUS & RST_CLK_CLOCK_STATUS_PLL_CPU_RDY));//wait for pll stability
	MDR_RST_CLK->CPU_CLOCK |= 1<<RST_CLK_CPU_CLOCK_HCLK_SEL_Pos;//select cpu_c3 hclk	
	MDR_RST_CLK->CPU_CLOCK |= 1<<RST_CLK_CPU_CLOCK_CPU_C2_SEL_Pos;//select pll c2
	
	
	
	

	MDR_RST_CLK->PER_CLOCK = 0xFFFFFFFF;
	MDR_RST_CLK->UART_CLOCK |= RST_CLK_UART_CLOCK_UART2_CLK_EN; 
	
	MDR_RST_CLK->TIM_CLOCK = RST_CLK_TIM_CLOCK_TIM1_CLK_EN;
	
	MDR_TIMER1 -> CNTRL |= (1 << TIMER_CNTRL_ARRB_EN_Pos)| TIMER_CNTRL_CNT_EN;
	MDR_TIMER1 -> CH2_CNTRL1|= (0x2 << TIMER_CH_CNTRL1_SELO_Pos);
	MDR_TIMER1 -> CH2_CNTRL1|= (0x01 << TIMER_CH_CNTRL1_SELOE_Pos);	
	MDR_TIMER1 -> CH2_CNTRL |= (0x6 << TIMER_CH_CNTRL_OCCM_Pos);
	MDR_TIMER1-> ARR = 9999;

	
	MDR_PORTF -> OE |= (1 << 1); 
	MDR_PORTF -> FUNC |= (3 << PORT_FUNC_MODE0_Pos)|(3 << PORT_FUNC_MODE1_Pos);
	MDR_PORTF -> ANALOG |= 3;
	//MDR_PORTF -> PD &= ~ (1 << 10);
	MDR_PORTF -> PWR |= (0x3 << PORT_PWR1_Pos);


	//init perefirial 
		
	
	MDR_UART2->IBRD = 43;
	MDR_UART2->FBRD = 26;
	MDR_UART2->LCR_H |= (3 << UART_LCR_H_WLEN_Pos)|(1 << UART_LCR_H_FEN_Pos);
	MDR_UART2->IFLS |= (1 << UART_IFLS_TXIFLSEL_Pos);
	MDR_UART2->IFLS &= ~(UART_IFLS_RXIFLSEL_Msk);
  MDR_UART2->IMSC |= (1 << UART_IMSC_TXIM_Pos)|(1 << UART_IMSC_RXIM_Pos);	
	MDR_UART2->CR |= (1 << UART_CR_TXE_Pos)|(1 << UART_CR_RXE_Pos)|(1 << UART_CR_UARTEN_Pos);
	
	NVIC->ISER[0] = (1 << 7);
	NVIC_EnableIRQ (7);
	NVIC_EnableIRQ (14);
	__enable_irq();
	
 	while(1)
	{
		
	}
}


//Subprograms

void set_speed(uint8_t x)
{
	x = sign;
	MDR_TIMER1->PSG = x;
}
	
//Interrupt routines
void UART2_IRQHandler(void)
{
	uint16_t temp = 0;
	if(MDR_UART2->MIS & UART_MIS_RXMIS)  
		{					
						sign = MDR_UART2->DR & 0xff;			
						temp = sign*6 - 10;
						MDR_TIMER1->PSG = temp; 
	  }
}

void Timer1_IRQHandler(void)
{
	counter++;
	MDR_UART2->DR = counter;
}

*/



//includes
#include <MDR32Fx.h>
#include <stdint.h>
//init variable&contants
uint8_t  sign = 0;
//init function prototype

uint32_t cnt = 0;
//main function


int main (void)
{
	//timer
	MDR_RST_CLK->PER_CLOCK = 0xFFFFFFFF;
	MDR_RST_CLK -> UART_CLOCK |= (1<< RST_CLK_UART_CLOCK_UART2_CLK_EN_Pos);	
	
//	MDR_RST_CLK -> HS_CONTROL = (0x0 << RST_CLK_HS_CONTROL_HSE_BYP_Pos) | (0x1 << RST_CLK_HS_CONTROL_HSE_ON_Pos);
//	
//	while ( (MDR_RST_CLK -> CLOCK_STATUS & RST_CLK_CLOCK_STATUS_HSE_RDY) != RST_CLK_CLOCK_STATUS_HSE_RDY );
//	
//	MDR_RST_CLK -> CPU_CLOCK = (0x2 << RST_CLK_CPU_CLOCK_CPU_C1_SEL_Pos);
//	MDR_RST_CLK -> PLL_CONTROL = (0x9 << RST_CLK_PLL_CONTROL_PLL_CPU_MUL_Pos) | (0x1 << RST_CLK_PLL_CONTROL_PLL_CPU_ON_Pos);
//	MDR_RST_CLK -> PLL_CONTROL |= (RST_CLK_PLL_CONTROL_PLL_CPU_PLD);
//	MDR_RST_CLK -> PLL_CONTROL &= ~(RST_CLK_PLL_CONTROL_PLL_CPU_PLD);
//	while ( (MDR_RST_CLK -> CLOCK_STATUS & RST_CLK_CLOCK_STATUS_PLL_CPU_RDY) != RST_CLK_CLOCK_STATUS_PLL_CPU_RDY );
//	
//	MDR_RST_CLK -> CPU_CLOCK |= (0x1 << RST_CLK_CPU_CLOCK_CPU_C2_SEL_Pos) | (0x1 << RST_CLK_CPU_CLOCK_HCLK_SEL_Pos);
//	MDR_RST_CLK -> PER_CLOCK = 0xFFFFFFFF;
	
	MDR_PORTD -> OE |= (1 << 10);
	MDR_PORTD -> FUNC |= (0x0 << PORT_FUNC_MODE10_Pos);
	MDR_PORTD -> ANALOG |= (1 << 10);
	MDR_PORTD -> PD &= ~(1 << 10);
	MDR_PORTD -> PWR |= (0x3 << PORT_PWR10_Pos);

	MDR_PORTC -> FUNC |= (0x2 << PORT_FUNC_MODE12_Pos);
	MDR_PORTC -> ANALOG |= (0x1 << 12);
	MDR_PORTC -> PD &= ~(1 << 12);
	MDR_PORTC -> PWR |= (0x3 << PORT_PWR12_Pos);
	MDR_PORTC -> GFEN |= (0x1 << 12);


	
//port
MDR_PORTF -> FUNC |= (0x3 << PORT_FUNC_MODE0_Pos);	
MDR_PORTF -> FUNC |= (0x3 << PORT_FUNC_MODE1_Pos);
MDR_PORTF -> PWR |= (0x3 << PORT_PWR1_Pos);
MDR_PORTF -> ANALOG |= (1 <<0);
MDR_PORTF -> ANALOG |= (1 <<1);
MDR_PORTF -> OE |= (1 << 1);

//UART
//MDR_UART2 ->  FR |= (1<< UART_FR_BUSY_Pos);
MDR_UART2 ->  IBRD = 43; 
MDR_UART2 ->  FBRD = 26; 
MDR_UART2 -> LCR_H |= ((3 << UART_LCR_H_WLEN_Pos)|(1<< UART_LCR_H_FEN_Pos));
MDR_UART2 -> CR |= ((1 << UART_CR_RXE_Pos)| (1 << UART_CR_TXE_Pos)| (1<< UART_CR_UARTEN_Pos ));
MDR_UART2 -> IFLS |= (1 << UART_IFLS_TXIFLSEL_Pos ) ;
MDR_UART2 -> IFLS &= ~( UART_IFLS_RXIFLSEL_Msk );
MDR_UART2 -> IMSC |= (1 << UART_IMSC_RXIM_Pos);
MDR_UART2 -> MIS |=  (1 << UART_MIS_RXMIS_Pos);

//Timer1
	MDR_RST_CLK -> TIM_CLOCK |= (RST_CLK_TIM_CLOCK_TIM1_CLK_EN);	
//	MDR_TIMER1 -> CNTRL |= (1 << TIMER_CNTRL_ARRB_EN_Pos)| TIMER_CNTRL_CNT_EN;	
	MDR_TIMER1 -> CH2_CNTRL |= (6 << TIMER_CH_CNTRL_OCCM_Pos);	
	MDR_TIMER1 -> ARR = 8000;
	MDR_TIMER1 -> PSG = 2000;
	MDR_TIMER1 -> IE |= (1 << 0);


	//	NVIC -> ISER [0] = (0x1 << 29);
	NVIC -> ISER [0] |= (0x1 << 7);
	NVIC -> ISER [0] |= (0x1 << 14);
	__enable_irq();
	
while (1)
		{
			
		}
}
void UART2_IRQHandler (void)
{
	if (MDR_UART2 -> MIS & UART_MIS_RXMIS)
	{
		sign = MDR_UART2 -> DR ; 
		switch (sign){ //F_CPU/(PSG*ARR)
			case 0x30 : MDR_TIMER1 -> ARR = 4000; break;//1 Ghz 
			case 0x31 : MDR_TIMER1 -> ARR = 8000; break;//0.5 Ghz
			case 0x32 : MDR_TIMER1 -> ARR = 12121; break;//0.33 Ghz
			case 0x33 : MDR_TIMER1 -> PSG = 16000; break;//0.25 Ghz
			case 0x34 : MDR_TIMER1 -> PSG = 20000; break;//0.2 Ghz
			case 0x35 : MDR_TIMER1 -> PSG = 23529; break;//0.17 Ghz
			case 0x36 : MDR_TIMER1 -> PSG = 28571; break;//0.14 Ghz
			case 0x37 : MDR_TIMER1 -> PSG = 32000; break;//0.125 Ghz
			case 0x38 : MDR_TIMER1 -> ARR = 36363; break;//0.11 Ghz
			default 	: MDR_TIMER1 -> PSG = 1000; break;
		}
	}
	NVIC->ICPR[0] &= ~(1 << 7);
}
	
	void Timer1_IRQHandler (void)
	{
		MDR_UART2 -> DR = ((cnt/100)+ 0x30);
		MDR_UART2 -> DR = (((cnt/10)%10)+ 0x30);	
		MDR_UART2 -> DR = (cnt%10+ 0x30);
		MDR_UART2 -> DR = 0xA;
		cnt = (cnt == 1000) ? 0 : cnt++;

		NVIC->ICPR[0] &= ~(1 << 14);
		MDR_TIMER1-> STATUS = 0;
	}


