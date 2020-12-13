/*
 * FullAttempt_1.c
 *
 * Created: 11/27/2019 9:15:05 PM
 * Author : senthu
 */ 

#include <avr/io.h>
#include "ff/ff.h"
#include <avr/interrupt.h>
#include <stdbool.h>
#include "ADS1299.h"
#include "uart.h"
#include "systemClock.h"
#include "Application.h"

//redefine F_CPU
#ifdef F_CPU
#undef F_CPU
#endif
#define F_CPU 32000000UL

#include <util/delay.h>


byte deviceid = 0;
uint8_t reg_vals[24];
bool drdy_high = false;
bool sd_error;
uint8_t rx_received_byte;

/*
int main(void)
{
	set_32MHz_clk();
	setup_ADS_dry_interrupt();
	
	int sample_count_dummy;
			
	//init and reset ADS1299
	ADSspi_init();
	_delay_ms(2500);
	adsReset();
	_delay_ms(500);
	
	adsSDATAC();
		
	uart_serial_init();
	uart_bluetooth_init();
	SD_clear    ();
	
	PORTC_DIRCLR = PIN0_bm;
	//uint8_t portctemp = PORTC.PIN0CTRL;
	PORTC.PIN0CTRL = 0b00011000; //set to pullup;
	 

	PMIC.CTRL |= PMIC_LOLVLEX_bm;	//enable low interrupt and global interrupt
	sei();							//Enable interrupt
		
	deviceid = adsGetDeviceID();
	//adsRREG(0x00, 24, reg_vals);
	_delay_ms    (200);
	
/*	
	//configurations for ECG Signals
	adsWREG(ADS1299_REGADDR_CONFIG3, 0b11101100);
	_delay_ms    (1000);
	adsWREG(ADS1299_REGADDR_CONFIG1, 0b11010110);
	adsWREG(ADS1299_REGADDR_CONFIG2, 0b11010000);
	_delay_ms    (200);
	adsWREG(ADS1299_REGADDR_CH1SET, 0b10100101);
	adsWREG(ADS1299_REGADDR_CH2SET, 0b10000011);
	adsWREG(ADS1299_REGADDR_CH3SET, 0b10100101);
	adsWREG(ADS1299_REGADDR_CH4SET, 0b10000011);
	adsWREG(ADS1299_REGADDR_CH5SET, 0b10100101);
	adsWREG(ADS1299_REGADDR_CH6SET, 0b01000000);
	adsWREG(ADS1299_REGADDR_CH7SET, 0b10000000);
	adsWREG(ADS1299_REGADDR_CH8SET, 0b10000011);
	_delay_ms    (200);
	
	adsWREG(ADS1299_REGADDR_MISC1,		0b00100000);
	adsWREG(ADS1299_REGADDR_BIAS_SENSP, 0b00100000);
	adsWREG(ADS1299_REGADDR_BIAS_SENSN, 0b00100000);
	_delay_ms    (200);
	

	
	//configurations for Test Signals
	adsWREG(ADS1299_REGADDR_CONFIG3, 0b11100001);//0b11101000
	_delay_ms    (1000);
	adsWREG(ADS1299_REGADDR_CONFIG1, 0b11010110);//0xD6
	adsWREG(ADS1299_REGADDR_CONFIG2, 0b11010000);//0xC4 0b11010000
	_delay_ms    (200);
	adsWREG(ADS1299_REGADDR_CH1SET, 0b00100101);
	adsWREG(ADS1299_REGADDR_CH2SET, 0b10000011);
	adsWREG(ADS1299_REGADDR_CH3SET, 0b00100101);
	adsWREG(ADS1299_REGADDR_CH4SET, 0b10000011);
	adsWREG(ADS1299_REGADDR_CH5SET, 0b00100101);
	adsWREG(ADS1299_REGADDR_CH6SET, 0b10000011);
	adsWREG(ADS1299_REGADDR_CH7SET, 0b00100101);
	adsWREG(ADS1299_REGADDR_CH8SET, 0b10000011);
	_delay_ms    (200);
	
/*	
	//configurations for Test
 	adsWREG(ADS1299_REGADDR_CONFIG3, 0b11100001);//0b11101000
	_delay_ms    (1000);
	adsWREG(ADS1299_REGADDR_CONFIG1, 0b11010110);//0xD6
	adsWREG(ADS1299_REGADDR_CONFIG2, 0b11000010);//0xC4 0b11010000
	_delay_ms    (200);
	adsWREG(ADS1299_REGADDR_CH1SET, 0b10000011);
	adsWREG(ADS1299_REGADDR_CH2SET, 0b10000011);
	adsWREG(ADS1299_REGADDR_CH3SET, 0b10000011);
	adsWREG(ADS1299_REGADDR_CH4SET, 0b10000011);
	adsWREG(ADS1299_REGADDR_CH5SET, 0b10000011);
	adsWREG(ADS1299_REGADDR_CH6SET, 0b10000011);
	adsWREG(ADS1299_REGADDR_CH7SET, 0b10000011);
	adsWREG(ADS1299_REGADDR_CH8SET, 0b10000011);
	_delay_ms    (200);
	
	

	//configurations for Input Signal generator
	adsWREG(ADS1299_REGADDR_CONFIG1, 0b11010110);//0b11010110/
	adsWREG(ADS1299_REGADDR_CONFIG2, 0b11010100);//0b11010001
	adsWREG(ADS1299_REGADDR_CONFIG3, 0b11101101);//0b11101000
	adsWREG(ADS1299_REGADDR_CH1SET, 0b10110000);
	adsWREG(ADS1299_REGADDR_CH2SET, 0b01100000);
	adsWREG(ADS1299_REGADDR_CH3SET, 0b10100101);
	adsWREG(ADS1299_REGADDR_CH4SET, 0b10100101);
	adsWREG(ADS1299_REGADDR_CH5SET, 0b10100101);
	adsWREG(ADS1299_REGADDR_CH6SET, 0b10100101);
	adsWREG(ADS1299_REGADDR_CH7SET, 0b10100101);
	adsWREG(ADS1299_REGADDR_CH8SET, 0b10100101);
	adsWREG(ADS1299_REGADDR_MISC1,  0b00100000);

	adsRDATAC();//turn read data continuous back on in case if not already turned on
	//adsRREG(0x00, 24, reg_vals);
    adsStart();
	//adsRDATAC();
	
	double sd_test_count = 0;

	while(1)
	{
		
		//process uart command
		if (new_bl_cmd_flg)
		{
			new_bl_cmd_flg = false;
			process_bl_command    ();
		}
		
		
		//uart_send_serial_sample();
		SPI_send_SD_sample();
		//uart_send_serial_str("a\n\r");
		//uart_send_BT_sample();
		
		//SPI_send_SD_str("hello\n\r");
		//sd_test_count++;
		
	}


	return 0;
}
*/



int main(void)
{
	set_32MHz_clk();
	setup_ADS_dry_interrupt();
		
	ADSspi_init();
	_delay_ms(2500);
	adsReset();
	_delay_ms(500);
	
	uart_serial_init();
	uart_bluetooth_init();
	SD_clear    ();

	
	PMIC.CTRL |= PMIC_LOLVLEX_bm;		//enable low interrupt and global interrupt
	sei();								//Enable interrupt
	
	app_state = IDLE;
	bool in_idle_state = true;
	sd_error = false;
	uint8_t ret;
	
	drdy_count=0;
	sd_samples = 0;
	bl_samples = 0;
	sd_buff_miss = 0;
	bl_buff_miss = 0;
	
	leadoff_enable = false;
	leadoff_disable = false;
	imp_enable = false;
	imp_disable = false;
	
	//drdy_count = sizeof(int);
	
	drdy_samples = 0;
	samples_got = 0;
	double th = 8192;

	while(1)
	{
		//adsGetData    ();
		//process uart command
		if (new_bl_cmd_flg)
		{
			new_bl_cmd_flg = false;
			process_bl_command    ();
		}
	
		switch (app_state)
		{
			case CRM_SETUP:
			
				setup_ADS_dry_interrupt();
				
				adsWakeup();
				_delay_ms    (100);
			
				deviceid = adsGetDeviceID();
				//adsRREG(0x00, 24, reg_vals);	
				
						
				//change configurations
				adsWREG(ADS1299_REGADDR_CONFIG1, 0b11010110);//0b11010110/
				adsWREG(ADS1299_REGADDR_CONFIG2, 0b11010001);//0b11010001
				adsWREG(ADS1299_REGADDR_CONFIG3, 0b11100000);//0b11101000
					
				adsWREG(ADS1299_REGADDR_CH1SET, 0b10010101);
				adsWREG(ADS1299_REGADDR_CH2SET, 0b10000101);
				adsWREG(ADS1299_REGADDR_CH3SET, 0b10000001);
				adsWREG(ADS1299_REGADDR_CH4SET, 0b10010101);
				adsWREG(ADS1299_REGADDR_CH5SET, 0b10000001);
				adsWREG(ADS1299_REGADDR_CH6SET, 0b00010101);
				adsWREG(ADS1299_REGADDR_CH7SET, 0b10010101);
				adsWREG(ADS1299_REGADDR_CH8SET, 0b10010101);
			
/*		
				adsWREG(ADS1299_REGADDR_CONFIG3, 0b11101100);
				_delay_ms    (1000);
				adsWREG(ADS1299_REGADDR_CONFIG1, 0b11010110);
				adsWREG(ADS1299_REGADDR_CONFIG2, 0b11010000);
				_delay_ms    (200);
				adsWREG(ADS1299_REGADDR_CH1SET, 0b10100101);
				adsWREG(ADS1299_REGADDR_CH2SET, 0b10000011);
				adsWREG(ADS1299_REGADDR_CH3SET, 0b10100101);
				adsWREG(ADS1299_REGADDR_CH4SET, 0b10000011);
				adsWREG(ADS1299_REGADDR_CH5SET, 0b10100101);
				adsWREG(ADS1299_REGADDR_CH6SET, 0b01000000);
				adsWREG(ADS1299_REGADDR_CH7SET, 0b11000000);
				adsWREG(ADS1299_REGADDR_CH8SET, 0b10000011);
				_delay_ms    (200);
	
				adsWREG(ADS1299_REGADDR_MISC1,		0b00100000);
				adsWREG(ADS1299_REGADDR_BIAS_SENSP, 0b00000000);
				adsWREG(ADS1299_REGADDR_BIAS_SENSN, 0b00000000);
				_delay_ms    (200);
*/				
/*				
				adsWREG(ADS1299_REGADDR_LOFF,		0b00000000);
				adsWREG(ADS1299_REGADDR_LOFF_SENSP, 0b01100000);
				adsWREG(ADS1299_REGADDR_CONFIG4,	0b00000010);
*/							
				SPI_send_SD_str    ("\n  \n\0");  // add a space before every write segments
				app_state = CRM_SD_MOUNT;
				break;
			
			case LEAD_OFF_SETUP :
				uart_send_serial_str("ls\n\r");
				adsWREG(ADS1299_REGADDR_LOFF,		0b10000000);
				adsWREG(ADS1299_REGADDR_LOFF_SENSP, 0b11111111);
				adsWREG(ADS1299_REGADDR_CONFIG4,	0b00000010);
				app_state = CRM_READ;
				break;
				
			case LEAD_OFF_DISABLE :
				uart_send_serial_str("ld\n\r");
				adsWREG(ADS1299_REGADDR_LOFF,		0b00000000);
				adsWREG(ADS1299_REGADDR_LOFF_SENSP, 0b00000000);
				adsWREG(ADS1299_REGADDR_CONFIG4,	0b00000000);
				app_state = CRM_READ;
				break;
				
			case IMP_SETUP :
				uart_send_serial_str("is\n\r");
				adsWREG(ADS1299_REGADDR_LOFF,		0b00000001);
				adsWREG(ADS1299_REGADDR_LOFF_SENSP, 0b00100000);
				adsWREG(ADS1299_REGADDR_CONFIG4,	0b00000010);
				app_state = CRM_READ;
				break;
							
			case IMP_DISABLE :
				uart_send_serial_str("id\n\r");
				adsWREG(ADS1299_REGADDR_LOFF,		0b00000000);
				adsWREG(ADS1299_REGADDR_LOFF_SENSP, 0b00000000);
				adsWREG(ADS1299_REGADDR_CONFIG4,	0b00000000);
				app_state = CRM_READ;
				break;
				
				
			case CRM_SD_MOUNT :
				if ( (ret = f_mount(&FatFs, "", 1)) != FR_OK ) {
					uart_send_serial_str("device not found - ");
					uart_send_serial_str('9' + ret);
					uart_send_serial_str("\n\r");
					return;			//device not found
					//_delay_ms    (500);
					//app_state = CRM_SD_MOUNT;
					break;
				}
				app_state = CRM_SD_FILE_OPEN;
				break; 
				
			case CRM_SD_FILE_OPEN :
				if ((ret = f_open(&Fil, "demo.txt",  FA_WRITE|FA_OPEN_APPEND)) != FR_OK) {
					uart_send_serial_str("can't create file\n\r");
					return;			//can't create file
					//_delay_ms    (500);
					//app_state = CRM_SD_FILE_OPEN;
					break;
				}
				app_state = CRM_START_READ;
				if (sd_error)
				{
					sd_error = false;
					app_state = CRM_READ;
				}
				//SPI_send_SD_str    ("\n  \n\0");//takes too much time - buffer overflow
				break;
				
			case CRM_START_READ :
				uart_send_serial_str("CR mode\n\r");
				adsRDATAC();//turn read data continuous back on in case if not already turned on
				//adsRREG(0x00, 24, reg_vals);
				adsStart();
				in_idle_state = false;
				app_state = CRM_READ;
				break;
				
			case CRM_READ:
				app_state = CRM_READ_new;
				//uart_send_serial_sample();
				//SPI_send_SD_sample();//wont work
				uart_send_BT_sample();
/*			
				//comment this to skip SD card writing
				while (SD_databuff_head != SD_databuff_tail)
				{
					//TCHAR* data = &SD_DataBUFF[SD_databuff_tail][0];
					//f_printf(&Fil, data);
					//f_printf(&Fil,  &SD_DataBUFF[SD_databuff_tail]);
					
					if (f_printf(&Fil, &SD_DataBUFF[SD_databuff_tail]) == EOF)
					{
						uart_send_serial_str("SD\n\r");
						sd_error = true;
						f_close(&Fil);
						app_state = CRM_SD_MOUNT;
						break;
					}
										
					SD_databuff_tail = (SD_databuff_tail + 1) & SD_DATABUFF_MASK;
				}
*/			
				//uart_send_char_BT('b');
				//uart_send_char_BT('\n');
				//uart_send_char_BT('\r');
				
				break;
				
			case CRM_READ_new:
				app_state = SD_SYNQ;
				//uart_send_BT_sample();
				
				if (SD_databuff_head != SD_databuff_tail)
				{
					//TCHAR* data = &SD_DataBUFF[SD_databuff_tail][0];
					//f_printf(&Fil, data);
					//f_printf(&Fil,  &SD_DataBUFF[SD_databuff_tail]);
					
					if (f_printf(&Fil, &SD_DataBUFF[SD_databuff_tail]) == EOF)
					{
						//uart_send_serial_str("SD\n\r");
						sd_error = true;
						f_close(&Fil);
						app_state = CRM_SD_MOUNT;
						break;
					}
					
					SD_databuff_tail = (SD_databuff_tail + 1) & SD_DATABUFF_MASK;
				}
				
				//app_state = CRM_READ;
				if (samples_got >= th)
				{
					th += 8192;
					f_sync(&Fil);
				}
				
				break;
				
			case SD_SYNQ:
				app_state = CRM_READ;
				
				if (leadoff_enable)
				{
					leadoff_enable  = false;
					app_state = LEAD_OFF_SETUP;
				}
				
				if (leadoff_disable)
				{
					leadoff_disable = false;
					app_state = LEAD_OFF_DISABLE;
				}
				
				if (imp_enable)
				{
					imp_enable  = false;
					app_state = IMP_SETUP;
				}
				
				if (imp_disable)
				{
					imp_disable = false;
					app_state = IMP_DISABLE;
				}
			
				break;
				
			case IDLE:
				adsStop();
				disable_ADS_dry_inerrupt();
				if (!in_idle_state)
				{
					in_idle_state = true;
					adsStandby();
					f_close(&Fil);
					//SPI_send_SD_str    ("\nstopping the data stream!!!\n\0");
					uart_send_serial_str("IDLE mode\n\r");
				}
				buffer_full =  false;
				ADSnewData = false;
				app_state = IDLE;
				break;
				
			default:
				app_state = IDLE;
				uart_send_serial_str("default mode\n\r");
		}
		
	}


	return 0;
}
