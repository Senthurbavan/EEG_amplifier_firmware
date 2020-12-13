/*
 * Application.h
 *
 * Created: 11/17/2019 12:35:11 AM
 *  Author: senthu
 */ 


#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "ff/ff.h"
#include <avr/io.h>
//#include "string.h"

#ifndef F_CPU
#define F_CPU 32000000
#endif

#include <util/delay.h>


#define BL_DATABUFF_SIZE			64	//must be power of 2
#define BL_DATABUFF_MASK			(BL_DATABUFF_SIZE - 1)
#define BL_DATABUFF_SAMPLE_SIZE		28
#define SAMPLE_SIZE					28
#define COMMAND_WIDTH_MAX			20

uint32_t val;
char buffer[10];

volatile int BL_databuff_head;
volatile int BL_databuff_tail;

//for Serial command from BlueTooth
volatile uint8_t rx_byte;

volatile bool new_cmd_flg;
volatile uint8_t new_cmd[COMMAND_WIDTH_MAX];
volatile uint8_t new_cmd_index;

volatile bool new_bl_cmd_flg;
volatile uint8_t new_bl_cmd[COMMAND_WIDTH_MAX];
volatile uint8_t new_bl_cmd_index;

#define SD_DATABUFF_SIZE			64	//must be power of 2
#define SD_DATABUFF_MASK			(SD_DATABUFF_SIZE - 1)
#define SD_DATABUFF_SAMPLE_SIZE		100
//#define SAMPLE_SIZE_CHAR			100

FATFS FatFs;           //!< FatFs work area needed for each volume
FIL   Fil;             //!< File object needed for each open file

volatile uint8_t BL_DataBUFF[BL_DATABUFF_SIZE][BL_DATABUFF_SAMPLE_SIZE];
volatile uint8_t SD_DataBUFF[SD_DATABUFF_SIZE][SD_DATABUFF_SAMPLE_SIZE];
volatile int SD_databuff_head;
volatile int SD_databuff_tail;


volatile int_flag; 
volatile double drdy_count;
volatile double drdy_samples;
volatile double samples_got;
volatile bool buffer_full;
volatile uint8_t trigger;

volatile double sd_samples;
volatile double bl_samples;
volatile double sd_buff_miss;
volatile double bl_buff_miss;


#define IDLE				0
#define CRM_SETUP			1
#define CRM_READ			2
#define CRM_SD_MOUNT		3
#define CRM_SD_FILE_OPEN	4
#define CRM_START_READ		5
#define CRM_READ_new		6
#define LEAD_OFF_SETUP		7
#define LEAD_OFF_DISABLE	8
#define SD_SYNQ				9
#define LEAD_OFF_CHECK		10
#define IMP_SETUP			11
#define IMP_DISABLE			12

int app_state;
bool leadoff_enable ;
bool leadoff_disable; 
bool imp_enable ;
bool imp_disable;

//bool DataBuffPutByte(uint8_t data);
//void uart_send_serial_byte(void);
bool BL_DataBuffPutSample(uint8_t* data);
bool SD_DataBuffPutSample(uint8_t* data);
void uart_send_serial_sample(void);
void uart_send_BT_sample(void);
void SPI_send_SD_sample(void);
void SPI_send_SD_str(char*str);
void SD_clear(void);

void process_command(void);
void process_bl_command(void);
void setup_ADS_dry_interrupt(void);
void disable_ADS_dry_inerrupt(void);

#endif /* APPLICATION_H_ */