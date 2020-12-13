/*
 * uart.c
 *
 * Created: 11/2/2019 12:48:45 PM
 *  Author: senthu
 */ 

#include "uart.h"

	
unsigned char UART_buff[] = {'a','b','b','b','c',
							'b','b','b','b','d',
							'b','b','b','b','e',
							'b','b','b','b','f',
							'b','b','b','b','g',
							'b','b','b','b','a'};


void uart_serial_init(){
	
	tx_ptr = 0;
	complete_flg = true;	//for the first run
	
	//set RX, TX pin directions
	PORTC_DIRCLR = PIN2_bm;				//RX
	PORTC_OUTSET = PIN3_bm;				//set TX pin high
	PORTC_DIRSET = PIN3_bm;				//TX
	
	//set frame format
	UART_SERIAL.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | (0 << USART_SBMODE_bp) | USART_CHSIZE_8BIT_gc;
	
	//set baud rate @2MHz
	//9600		: bscale=0,  bsel=12
	//115200	: bscale=-7, bsel=11
	
	//set baud rate @32MHz
	//115200	: bscale =-5,  bsel =1079, CLK2X = 1
	//460800	: bscale =-7,  bsel =983,  CLK2X = 1
	unsigned int bsel = 983;
	signed char bscale = -7;
	UART_SERIAL.BAUDCTRLA = (uint8_t)bsel;
	UART_SERIAL.BAUDCTRLB = (bscale << USART_BSCALE0_bp) | (bsel >> 8);
	UART_SERIAL.CTRLB |= USART_CLK2X_bm ;
	
	//setup RX interrupt level
	UART_SERIAL.CTRLA = USART_RXCINTLVL0_bm;
	
	//Enable TX and RX
	UART_SERIAL.CTRLB |= USART_TXEN_bm | USART_RXEN_bm;
	
	
	//usually we will be sending SD card data through UART for checking
	SD_databuff_head = 0;
	SD_databuff_tail = 0;
	
	//initialize new command bufer
	new_cmd_index = 0;
	new_cmd_flg = false;
}




void uart_bluetooth_init(){
	
	tx_ptr = 0;
	complete_flg = true;	//for the first run
	
	// set pins as input floating
	PORTF_DIRCLR = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm;
	PORTF_PIN0CTRL &= 0b11000111;
	PORTF_PIN1CTRL &= 0b11000111;
	PORTF_PIN2CTRL &= 0b11000111;
	PORTF_PIN3CTRL &= 0b11000111;
		
	//set RX, TX pin directions
	PORTE_DIRCLR = PIN2_bm;				//RX
	PORTE_OUTSET = PIN3_bm;				//set TX pin high
	PORTE_DIRSET = PIN3_bm;				//TX
		
	//set frame format
	UART_BT.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | (0 << USART_SBMODE_bp) | USART_CHSIZE_8BIT_gc;
		
	//set baud rate @2MHz
	//9600		: bscale=0,  bsel=12
	//115200	: bscale=-7, bsel=11
		
	//set baud rate @32MHz
	//115200	: bscale =-5,  bsel =1079, CLK2X = 1
	//460800	: bscale =-7,  bsel =983,  CLK2X = 1
	unsigned int bsel = 1079;
	signed char bscale = -5;
	UART_BT.BAUDCTRLA = (uint8_t)bsel;
	UART_BT.BAUDCTRLB = (bscale << USART_BSCALE0_bp) | (bsel >> 8);
	UART_BT.CTRLB |= USART_CLK2X_bm ;
	
	/*	
	//Enable TX
	UART_BT.CTRLB |= USART_TXEN_bm;
	*/
	
	//setup RX interrupt level
	UART_BT.CTRLA = USART_RXCINTLVL0_bm;
		
	//Enable TX and RX
	UART_BT.CTRLB |= USART_TXEN_bm | USART_RXEN_bm;
	
		
	//PMIC.CTRL |= PMIC_LOLVLEX_bm;
	//sei();	//Enable interrupt
	
	BL_databuff_head = 0;
	BL_databuff_tail = 0;
	
	//initialize new command bufer
	new_bl_cmd_index = 0;
	new_bl_cmd_flg = false;
}




void uart_send_serial(uint8_t* buff, int send_len){
	while(send_len > 0){
		while((UART_SERIAL.STATUS & USART_DREIF_bm)==0); //complete previous transmission
		UART_SERIAL.DATA = *(buff++);
		send_len--;
	}
	
}


void uart_send_serial_srt_max(uint8_t* buff, int send_len){
	int i = -1;
	do
	{
		i++;
		while((UART_SERIAL.STATUS & USART_DREIF_bm)==0); //complete previous transmission
		UART_SERIAL.DATA = buff[i];
	} while ((buff[i] != '\0') && (i<(send_len-1)));
	
	if (i == (send_len-1))
	{
		while((UART_SERIAL.STATUS & USART_DREIF_bm)==0);
		UART_SERIAL.DATA = '\0';
	}
	
	while((UART_SERIAL.STATUS & USART_DREIF_bm)==0); //wait before return
	return;
}



void uart_send_serial_str(uint8_t* buff)
{
	int i = 0;
	do 
	{
		while((UART_SERIAL.STATUS & USART_DREIF_bm)==0); //complete previous transmission
		UART_SERIAL.DATA = buff[i];
	} while (buff[i++] != '\0');
	
	while((UART_SERIAL.STATUS & USART_DREIF_bm)==0); //wait before return
	return;
}



void uart_send_char_serial(uint8_t data){
	while((UART_SERIAL.STATUS & USART_DREIF_bm)==0); //complete previous transmission
	UART_SERIAL.DATA = data;
	
}




void uart_send_BT(uint8_t* buff, int send_len){
	while(send_len > 0){
		while((UART_BT.STATUS & USART_DREIF_bm)==0); //complete previous transmission
		UART_BT.DATA = *(buff++);
		send_len--;
	}
	
}




void uart_send_char_BT(uint8_t data){
	while((UART_BT.STATUS & USART_DREIF_bm)==0); //complete previous transmission
	UART_BT.DATA = data;
	
}




//UART_BL ISR --- Not using it -- Tx interrupt is turned off
ISR(USARTE0_DRE_vect){
	uint8_t CTRLA_temp1;
	if (tx_ptr == BUFFSIZE)
	{
		complete_flg = true;
		CTRLA_temp1 = UART_BT.CTRLA;
		CTRLA_temp1 = (CTRLA_temp1 & ~USART_DREINTLVL_gm) | USART_DREINTLVL_OFF_gc;
		UART_BT.CTRLA = CTRLA_temp1;
	}else{
		UART_BT.DATA = UART_buff[tx_ptr++];
	}
}

//UART_SERIAL ISR --- Not using it-- Tx interrupt is turned off
ISR(USARTC0_DRE_vect){
	uint8_t CTRLA_temp1;
	if (tx_ptr == BUFFSIZE)
	{
		complete_flg = true;
		CTRLA_temp1 = UART_SERIAL.CTRLA;
		CTRLA_temp1 = (CTRLA_temp1 & ~USART_DREINTLVL_gm) | USART_DREINTLVL_OFF_gc;
		UART_SERIAL.CTRLA = CTRLA_temp1;
		
	}else{
		UART_SERIAL.DATA = UART_buff[tx_ptr++];
	}
}

//UART_SERIAL ISR for rx interrupt 
ISR(USARTC0_RXC_vect){
	uint8_t c = UART_SERIAL.DATA;
	if (c=='\r')
	{
		new_cmd_flg = true;
	} 
	else
	{
		new_cmd[new_cmd_index++] = c;
		new_cmd[new_cmd_index] = '\0';
	}
	
	if (new_cmd_index == (COMMAND_WIDTH_MAX -1))
	{
		//it is an error but this line is to make sure always last character in buffer is '\0'
		new_cmd_index = 0; 
	}
}

//UART_BL ISR for rx interrupt
ISR(USARTE0_RXC_vect){
	uint8_t c = UART_BT.DATA;
	if (c=='\r')
	{
		new_bl_cmd_flg = true;
	}
	else
	{
		new_bl_cmd[new_bl_cmd_index++] = c;
		new_bl_cmd[new_bl_cmd_index] = '\0';
	}
	
	if (new_bl_cmd_index == (COMMAND_WIDTH_MAX -1))
	{
		//it is an error but this line is to make sure always last character in buffer is '\0'
		new_bl_cmd_index = 0;
	}
}