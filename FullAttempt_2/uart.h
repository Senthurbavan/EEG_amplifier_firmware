/*
 * uart.h
 *
 * Created: 11/2/2019 12:49:04 PM
 *  Author: senthu
 */ 


#ifndef UART_H_
#define UART_H_

#include <avr/interrupt.h>
#include <stdbool.h>
#include "Application.h"

#define UART_BT			USARTE0
#define UART_SERIAL		USARTC0

#define BUFFSIZE		30
extern unsigned char UART_buff[BUFFSIZE];

volatile bool complete_flg;
int tx_ptr;
volatile bool rx_new_byte;

void uart_serial_init(void);
void uart_bluetooth_init(void);

//serial send data
void uart_send_serial(uint8_t* buff, int send_len);
void uart_send_serial_srt_max(uint8_t* buff, int send_len);
void uart_send_char_serial(uint8_t data);
void uart_send_serial_str(uint8_t* data);

//bluetooth send data
void uart_send_BT(uint8_t* buff, int send_len);
void uart_send_char_BT(uint8_t data);

#endif /* UART_H_ */