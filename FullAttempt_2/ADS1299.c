/*
 * ADS1299.c
 *
 * Created: 8/30/2019 8:36:26 PM
 *  Author: senthu
 */ 



#include "ADS1299.h"

uint8_t data_return;
uint8_t sample_count;

void ADSspi_init(void)
{
	// port configuratie mmc/sd/sdhc card
	PORTD.REMAP    = PORT_SPI_bm;         // swap SCK and MOSI
	PORTD.DIRCLR   = PIN6_bm;             // MISO        input
	PORTD.DIRSET   = PIN5_bm;             // SCK         output (after remap)
	PORTD.DIRSET   = PIN7_bm;             // MOSI        output (after remap)
	PORTD.DIRSET   = PIN4_bm;             // chip select output
	//PORTD.PIN6CTRL = PORT_OPC_PULLUP_gc;  // MISO        pullup

	PORTD.OUTCLR = PIN5_bm; // Make SCLK of ADS low
	PORTD.OUTCLR = PIN7_bm; // Make MOSI of ADS low
	
	// disable card
	//ADSspi_CShigh();
	
	PORTD.DIRCLR = PIN0_bm;		//DRDY  input
	PORTD.DIRSET = PIN2_bm;		//START output
	PORTD.OUTCLR = PIN2_bm;		// we use command for start so hold start pin low
	
	// fspi = (fcpu/64)*2 =  1MHz
	//SPID.CTRL      = SPI_CLK2X_bm | SPI_ENABLE_bm |(0<<SPI_DORD_bp) |SPI_MASTER_bm |SPI_PRESCALER_DIV64_gc |SPI_MODE_1_gc;
	SPID.CTRL      = SPI_ENABLE_bm |(0<<SPI_DORD_bp) |SPI_MASTER_bm |SPI_PRESCALER_DIV64_gc |SPI_MODE_1_gc;
	SPID.INTCTRL   = SPI_INTLVL_OFF_gc;

	sample_count = 65;
	buffer_full =  false;
	ADSnewData = false;
	return;
}




byte transfer(byte _data){
	//byte out;
	SPID.DATA = _data;
	//_delay_us    (1);
	while ( !(SPID.STATUS & (SPI_IF_bm))) ;
	//out  = SPID.DATA;
	return SPID.DATA;
}



//System Commands
void adsWakeup() {
	ADSspi_CSlow();
	_delay_us    (2);
	transfer(_WAKEUP);
	_delay_us    (5);
	ADSspi_CShigh();
	_delay_us    (5);//must wait least 4 tCLK cycles before sending another command
	return;
}
void adsStandby() {
	ADSspi_CSlow();
	_delay_us    (5);
	transfer(_STANDBY);
	_delay_us    (5);
	ADSspi_CShigh();
	return;
}
void adsReset() {
	ADSspi_CSlow();
	_delay_us    (5);
	transfer(_RESET);
	_delay_ms(20);// delay(18.0*tCLK); //must wait 18 tCLK cycles to execute this command
	ADSspi_CShigh();
	return;
}
void adsStart() {
	ADSspi_CSlow();
	_delay_us    (5);
	transfer(_START);
	_delay_us    (5);
	ADSspi_CShigh();
	return;
}
void adsStop() {
	ADSspi_CSlow();
	_delay_us    (5);
	transfer(_STOP);
	_delay_us    (5);
	ADSspi_CShigh();
	return;
}



//Data Read Commands
void adsRDATAC() {
	ADSspi_CSlow();
	_delay_us    (5);
	transfer(_RDATAC);
	_delay_us    (5);
	ADSspi_CShigh();
	return;
}
void adsSDATAC() {
	ADSspi_CSlow();
	_delay_us    (25);
	transfer(_SDATAC);
	_delay_us    (25);
	ADSspi_CShigh();
	return;
}
void adsRDATA() {
	ADSspi_CSlow();
	_delay_us    (5);
	transfer(_RDATA);
	_delay_us    (5);
	ADSspi_CShigh();
	return;
}



//Register Read/Write Commands
byte adsGetDeviceID() {
	ADSspi_CSlow();
	_delay_us    (5);
	transfer(_SDATAC); //SDATAC
	_delay_us    (2);
	transfer(_RREG); //_RREG
	_delay_us    (2);
	transfer(0x00); //Asking for 1 byte
	_delay_us    (2);
	byte data = transfer(0x00); // byte to read (hopefully 0b???11110)
	_delay_us    (2);
	transfer(_RDATAC); //turn read data continuous back on
	_delay_us    (5);
	ADSspi_CShigh();
	return data;
}


byte adsRREG_single(byte _address) {
	opcode2 = _RREG + (_address & 0x1F); //001rrrrr; _RREG = 00100000 and _address = rrrrr
	ADSspi_CSlow();
	_delay_us    (5);
	transfer(_SDATAC); //SDATAC
	_delay_us    (2);
	transfer(opcode2); //RREG
	_delay_us    (2);
	transfer(0x00); //opcode2
	_delay_us    (2);
	byte data = transfer(0x00); // returned byte should match default of register map unless edited manually
	_delay_us    (2);
	transfer(_RDATAC); //turn read data continuous back on
	_delay_us    (5);
	ADSspi_CShigh();
	return data;
}

void adsRREG(byte _address, byte _numRegisters, byte* data) {
	byte opcode1 = _RREG + (_address & 0x1F); //001rrrrr; _RREG = 00100000 and _address = rrrrr
	ADSspi_CSlow();
	_delay_us    (5);
	transfer(_SDATAC); //SDATAC
	transfer(opcode1); //RREG
	transfer(_numRegisters-1); //opcode2
	for(byte i = 0; i < _numRegisters; i++){
		*data = transfer(0x00); // returned byte should match default of register map unless previously edited manually
		data++;
	}
	transfer(_RDATAC); //turn read data continuous back on
	_delay_us    (5);
	ADSspi_CShigh();
	return;
}

void adsWREG(byte _address, byte data) {
	byte opcode1 = _WREG + (_address & 0x1F); //001rrrrr; _RREG = 00100000 and _address = rrrrr
	ADSspi_CSlow();
	_delay_us    (25);
	transfer(_SDATAC); //SDATAC
	transfer(opcode1);
	transfer(0x00);
	transfer(data);
	transfer(_RDATAC);
	_delay_ms    (10);
	ADSspi_CShigh();
	return;
}


void adsGetData(){
	if(ADSnewData){
		samples_got++;
		ADSspi_CSlow();
		int i;
		for ( i=0; i<SAMPLE_SIZE-1;i++)
		{
			sample[i] = transfer(0x00);
			//sample[i] = 79;
		}
/*		
		sample[i] = sample_count++;
		if (sample_count==91)
		{
			sample_count = 65;
		}
*/		
		
		sample[i] = trigger;

		if (SD_DataBuffPutSample(sample)==0) //buffer full condition
		{
			sd_buff_miss++;
			//buffer_full = true;
			//uart_send_serial_str("b\n\r");
			//uart_send_char_serial('\n');
			//uart_send_char_serial('\r');
		}

		if (BL_DataBuffPutSample(sample)==0) //buffer full condition
		{
			bl_buff_miss++;
			//buffer_full = true;
			//uart_send_serial_str("bf\n\r");
			//uart_send_char_serial('\n');
			//uart_send_char_serial('\r');
		}

		_delay_us    (5);
		ADSspi_CShigh();
		ADSnewData = false;
		
	}
}