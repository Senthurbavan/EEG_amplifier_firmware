/*
 * Application.c
 *
 * Created: 11/17/2019 12:34:53 AM
 *  Author: senthu
 */ 
#include "ADS1299.h"
#include "Application.h"
#include "uart.h"
#include "string.h"
/*
//if DataBUFF is 1-dimentional
bool DataBuffPutByte(uint8_t data){
	
	int head_temp = (databuff_head+1) & DATABUFF_MASK;
	bool buff_free = (head_temp != databuff_tail);
	
	if (buff_free)
	{
		DataBUFF[databuff_head] = (uint8_t)data;
		databuff_head = head_temp;
	}
	
	return buff_free;
}


void uart_send_serial_byte(void) {
	while (databuff_head != databuff_tail)
	{
		uint8_t data = DataBUFF[databuff_tail];
		//uart_send_serial((uint8_t *)&data, 1);
		uart_send_char_serial(data);
		databuff_tail = (databuff_tail + 1) & DATABUFF_MASK;
	}
}
*/


bool BL_DataBuffPutSample(uint8_t* data){
	
	int head_temp = (BL_databuff_head+1) & BL_DATABUFF_MASK;
	bool buff_free = (head_temp != BL_databuff_tail);
	
	if (buff_free)
	{
		for (int i=0; i<SAMPLE_SIZE; i++)
		{
			BL_DataBUFF[BL_databuff_head][i] = (uint8_t)data[i];
		}
		BL_databuff_head = head_temp;
		//bl_samples++;
	}
	
	return buff_free;
}


//fill SD_DataBuff with sample
//followed by \r and \0
bool SD_DataBuffPutSample(uint8_t* data){
	
	int head_temp = (SD_databuff_head+1) & SD_DATABUFF_MASK;
	bool buff_free = (head_temp != SD_databuff_tail);
	//uint32_t val;
	//char buffer[10];
	
	if (buff_free)
	{
		int j=0;
		for (int i=0; i<SAMPLE_SIZE-1; i=i+3)
		{
			val = ((uint32_t)data[i]<<16) | ((uint32_t)data[i+1]<<8) | (uint32_t)data[i+2];
			ltoa(val,buffer,10);
			for (int buf_i=0; (buf_i < 10) && (buffer[buf_i] != '\0');buf_i++,j++){
				SD_DataBUFF[SD_databuff_head][j] = buffer[buf_i];
			}
			SD_DataBUFF[SD_databuff_head][j++] = ',';
		}
		SD_DataBUFF[SD_databuff_head][j++] = data[SAMPLE_SIZE-1];
		SD_DataBUFF[SD_databuff_head][j++] = '\r';
		SD_DataBUFF[SD_databuff_head][j] = '\0';
		SD_databuff_head = head_temp;
		//sd_samples++;
	}
	
	return buff_free;
}


//don't use this with SD write cuz this will alter the tail
//this is just for debugging
void uart_send_serial_sample(void) {
	while (SD_databuff_head != SD_databuff_tail)
	{
		uint8_t* data = &SD_DataBUFF[SD_databuff_tail][0];
		//uart_send_serial(data, SD_DATABUFF_SAMPLE_SIZE);
		uart_send_serial_srt_max(data,SD_DATABUFF_SAMPLE_SIZE);
		uart_send_char_serial('\n');
		uart_send_char_serial('\r');
		SD_databuff_tail = (SD_databuff_tail + 1) & SD_DATABUFF_MASK;
	}
}

void uart_send_BT_sample(void) {
	while (BL_databuff_head != BL_databuff_tail)
	{
		uint8_t* data = &BL_DataBUFF[BL_databuff_tail][0];
		uart_send_BT(data, BL_DATABUFF_SAMPLE_SIZE);
		uart_send_char_BT('\n');
		uart_send_char_BT('\r');
		BL_databuff_tail = (BL_databuff_tail + 1) & BL_DATABUFF_MASK;
	}
}


void SPI_send_SD_sample(void){
	//UINT    i;
	uint8_t ret;

	if ( (ret = f_mount(&FatFs, "", 1)) != FR_OK ) {
		return;			//device not found
	}
	
	if ((ret = f_open(&Fil, "demo.txt",  FA_WRITE|FA_OPEN_APPEND)) != FR_OK) {
		return;			//can't create file
	}

	while (SD_databuff_head != SD_databuff_tail)
	{
		TCHAR* data = &SD_DataBUFF[SD_databuff_tail][0];
		f_printf(&Fil, data);
/*		if (f_printf(&Fil, &SD_DataBUFF[SD_databuff_tail]) == EOF)
		{
			uart_send_serial_str("command ERROR");
			uart_send_char_serial('\n');
			uart_send_char_serial('\r');
		}
		*///f_printf(&Fil, &SD_DataBUFF[SD_databuff_tail]);
		SD_databuff_tail = (SD_databuff_tail + 1) & SD_DATABUFF_MASK;
	}
	
	f_close(&Fil);
	return;
}


void SPI_send_SD_str(char*str){
	//UINT    i;
	uint8_t ret;
	
	if ( (ret = f_mount(&FatFs, "", 1)) != FR_OK ) {
		return;			//device not found
	}
	
	if ((ret = f_open(&Fil, "demo.txt", FA_WRITE|FA_OPEN_APPEND)) != FR_OK) {
		return;			//can't create file
	}
	
	f_printf(&Fil,str);
	
	f_close(&Fil);
	return;
}


void SD_clear(void){
	//UINT    i;
	uint8_t ret;
		
	if ( (ret = f_mount(&FatFs, "", 1)) != FR_OK ) {
		return ;			//device not found
	}
		
	if ((ret = f_open(&Fil, "demo.txt", FA_WRITE|FA_CREATE_ALWAYS)) != FR_OK) {
		return ;			//can't create file
	}
	
	f_printf(&Fil, "SD card Initialized\n");
	
	f_close(&Fil);
	return;
}


//process the command in the new_cmd buffer and clear the buffer
void process_command(void)
{
	if (strstr(new_cmd,"AT+IDLE:") == new_cmd)
	{
		//uart_send_serial_str("command is abc");
		//uart_send_char_serial('\n');
		//uart_send_char_serial('\r');
		app_state = IDLE;
	}else if (strstr(new_cmd,"AT+CRM:") == new_cmd)
	{
		app_state = CRM_SETUP;
	}else
	{
		uart_send_serial_str("command ERROR");
		uart_send_char_serial('\n');
		uart_send_char_serial('\r');
	}
	
	memset(new_cmd,'\0',COMMAND_WIDTH_MAX);
	new_cmd_index = 0;
	return;
}



//process the command in the new_cmd buffer and clear the buffer
void process_bl_command(void)
{
	if (strstr(new_bl_cmd,"A") == new_bl_cmd)
	{
		//uart_send_serial_str("command is idle");
		//uart_send_char_serial('\n');
		//uart_send_char_serial('\r');
		app_state = IDLE;
	}else if (strstr(new_bl_cmd,"B") == new_bl_cmd)
	{
		//uart_send_serial_str("command is crm");
		//uart_send_char_serial('\n');
		//uart_send_char_serial('\r');
		app_state = CRM_SETUP;
	}else if (strstr(new_bl_cmd,"C") == new_bl_cmd)
	{
		//uart_send_serial_str("leadoff setup\n\r");
		//uart_send_char_serial('\n');
		//uart_send_char_serial('\r');
		leadoff_enable = true;
	}else if (strstr(new_bl_cmd,"D") == new_bl_cmd)
	{
		//uart_send_serial_str("leadoff disable\n\r");
		//uart_send_char_serial('\n');
		//uart_send_char_serial('\r');
		leadoff_disable = true;
	}else if (strstr(new_bl_cmd,"E") == new_bl_cmd)
	{
		//uart_send_serial_str("leadoff setup\n\r");
		//uart_send_char_serial('\n');
		//uart_send_char_serial('\r');
		imp_enable = true;
	}else if (strstr(new_bl_cmd,"F") == new_bl_cmd)
	{
		//uart_send_serial_str("leadoff disable\n\r");
		//uart_send_char_serial('\n');
		//uart_send_char_serial('\r');
		imp_disable = true;
	}else
	{

		uart_send_serial_str("command ERROR");
		uart_send_char_serial('\n');
		uart_send_char_serial('\r');
		
		uart_send_serial_str(new_bl_cmd);
		uart_send_char_serial('\n');
		uart_send_char_serial('\r');
	}
	
	memset(new_bl_cmd,'\0',COMMAND_WIDTH_MAX); //clear the command buffer  
	new_bl_cmd_index = 0;
	return;
}




void setup_ADS_dry_interrupt()
{
	//setup interrupt
	int_flag = false;
	PORTD_DIRCLR = PIN0_bm;										//make as input pullup
	PORTD_INTCTRL = PMIC_LOLVLEX_bm << PORT_INT0LVL_gp;
	PORTD_INT0MASK = PIN0_bm;
	PORTD_INTFLAGS &=  PORT_INT0IF_bm;							//clear int flag
	PORTD_PIN0CTRL = PORT_OPC_PULLUP_gc|PORT_ISC_FALLING_gc;
	drdy_count = 0;
	//setup interrupt end
}

void disable_ADS_dry_inerrupt()
{
	PORTD_INTCTRL = 0x00 << PORT_INT0LVL_gp;					//disable
	PORTD_INTFLAGS &=  PORT_INT0IF_bm;							//clear int flag
}


ISR(PORTD_INT0_vect){
	ADSnewData = true;
	if (PORTC.IN & 0x01) // trigger signal 
	{
		trigger = '1';
	} 
	else
	{
		trigger = '0';
	}
	drdy_samples++;
	adsGetData    ();
}