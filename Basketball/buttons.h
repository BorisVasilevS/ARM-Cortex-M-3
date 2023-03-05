void initButtons(void){	
	//pc10
	MDR_PORTC->OE &= ~(1 << 10);
	MDR_PORTC->FUNC |= (0x0 << PORT_FUNC_MODE10_Pos);
	MDR_PORTC->ANALOG |= 1 << 10;
	MDR_PORTC->PD &= ~(1 << 10);
	MDR_PORTC->PWR |= (0x3 << PORT_PWR10_Pos);
	
	//pc11
	MDR_PORTC->OE &= ~(1 << 11);
	MDR_PORTC->FUNC |= (0x0 << PORT_FUNC_MODE11_Pos);
	MDR_PORTC->ANALOG |= 1 << 11;
	MDR_PORTC->PD &= ~(1 << 11);
	MDR_PORTC->PWR |= (0x3 << PORT_PWR11_Pos);
	
	//pc12
	MDR_PORTC->OE &= ~(1 << 12);
	MDR_PORTC->FUNC |= (0x0 << PORT_FUNC_MODE12_Pos);
	MDR_PORTC->ANALOG |= 1 << 12;
	MDR_PORTC->PD &= ~(1 << 12);
	MDR_PORTC->PWR |= (0x3 << PORT_PWR12_Pos);
	
	//pc13
	MDR_PORTC->OE &= ~(1 << 13);
	MDR_PORTC->FUNC |= (0x0 << PORT_FUNC_MODE13_Pos);
	MDR_PORTC->ANALOG |= 1 << 13;
	MDR_PORTC->PD &= ~(1 << 13);
	MDR_PORTC->PWR |= (0x3 << PORT_PWR13_Pos);
	
	//pc13
	MDR_PORTC->OE &= ~(1 << 14);
	MDR_PORTC->FUNC |= (0x0 << PORT_FUNC_MODE14_Pos);
	MDR_PORTC->ANALOG |= 1 << 14;
	MDR_PORTC->PD &= ~(1 << 14);
	MDR_PORTC->PWR |= (0x4 << PORT_PWR13_Pos);
	
	//pd
	MDR_PORTD->OE |= (1<<10);
	MDR_PORTD->FUNC &= ~(3<<PORT_FUNC_MODE10_Pos);
	MDR_PORTD->ANALOG |= (1<<10);
	MDR_PORTD->PD &= ~(1<<10);
	MDR_PORTD->PWR |= (3<<PORT_PWR10_Pos);
	
	MDR_PORTD->OE |=(1<<11);
	MDR_PORTD->FUNC |= (0<<PORT_FUNC_MODE11_Pos);
	MDR_PORTD->ANALOG |= (1<<11);
	MDR_PORTD->PD &= ~(1<<11);
	MDR_PORTD->PWR |= (3<<PORT_PWR11_Pos);
	//MDR_PORTD -> RXTX |= (1 << 11);
}
