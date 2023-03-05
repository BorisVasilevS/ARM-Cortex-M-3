void initTimer_1(int arr, int psg){
	MDR_RST_CLK -> TIM_CLOCK |= (RST_CLK_TIM_CLOCK_TIM1_CLK_EN);	
	MDR_TIMER1 -> CNTRL |= (1 << TIMER_CNTRL_ARRB_EN_Pos)| TIMER_CNTRL_CNT_EN;	
	//MDR_TIMER1 -> CH2_CNTRL1|= (2 << TIMER_CH_CNTRL1_SELO_Pos);
	//MDR_TIMER1 -> CH2_CNTRL1|= (1 << TIMER_CH_CNTRL1_SELOE_Pos);	
	MDR_TIMER1 -> CH2_CNTRL |= (6 << TIMER_CH_CNTRL_OCCM_Pos);	
	MDR_TIMER1 -> ARR = arr;
	MDR_TIMER1 -> PSG = psg;
	MDR_TIMER1 -> IE |= (1 << 1);
	NVIC->ISER[0] |= 1<<14; //Timer1_IRQHandler
}

void initTimer_2(int arr, int psg){
	MDR_RST_CLK -> TIM_CLOCK |= (RST_CLK_TIM_CLOCK_TIM2_CLK_EN);	
	MDR_TIMER2 -> CNTRL |= (1 << TIMER_CNTRL_ARRB_EN_Pos)| TIMER_CNTRL_CNT_EN;	
	//MDR_TIMER1 -> CH2_CNTRL1|= (2 << TIMER_CH_CNTRL1_SELO_Pos);
	//MDR_TIMER1 -> CH2_CNTRL1|= (1 << TIMER_CH_CNTRL1_SELOE_Pos);	
	MDR_TIMER2 -> CH2_CNTRL |= (6 << TIMER_CH_CNTRL_OCCM_Pos);	
	MDR_TIMER2 -> ARR = arr;
	MDR_TIMER2 -> PSG = psg;
	//MDR_TIMER2 -> IE |= (1 << 1);
	NVIC->ISER[0] |= 1<<15; //Timer2_IRQHandler
}

void initTimer_3(int arr, int psg){
	MDR_RST_CLK -> TIM_CLOCK |= (RST_CLK_TIM_CLOCK_TIM3_CLK_EN);	
	MDR_TIMER3 -> CNTRL |= (1 << TIMER_CNTRL_ARRB_EN_Pos)| TIMER_CNTRL_CNT_EN;	
	//MDR_TIMER1 -> CH2_CNTRL1|= (2 << TIMER_CH_CNTRL1_SELO_Pos);
	//MDR_TIMER1 -> CH2_CNTRL1|= (1 << TIMER_CH_CNTRL1_SELOE_Pos);	
	MDR_TIMER3 -> CH2_CNTRL |= (6 << TIMER_CH_CNTRL_OCCM_Pos);	
	MDR_TIMER3 -> ARR = arr;
	MDR_TIMER3 -> PSG = psg;
	MDR_TIMER3 -> IE |= (1 << 1);
	NVIC->ISER[0] |= 1<<16; //Timer3_IRQHandler
}

void timer3Off(void){
	MDR_TIMER3 -> CNTRL &= ~(1 << TIMER_CNTRL_ARRB_EN_Pos)| TIMER_CNTRL_CNT_EN;	
	MDR_TIMER3 -> IE &= ~(1 << 1);
}
void timer2Off(void){
	MDR_TIMER2 -> CNTRL &= ~(1 << TIMER_CNTRL_ARRB_EN_Pos)| TIMER_CNTRL_CNT_EN;	
	MDR_TIMER2 -> IE &= ~(1 << 1);
}
void timer1Off(void){
	MDR_TIMER1 -> CNTRL &= ~(1 << TIMER_CNTRL_ARRB_EN_Pos)| TIMER_CNTRL_CNT_EN;	
	MDR_TIMER1 -> IE &= ~(1 << 1);
}
