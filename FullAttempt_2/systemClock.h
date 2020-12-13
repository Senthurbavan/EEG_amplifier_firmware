/*
 * systemClock.h
 *
 * Created: 11/2/2019 5:56:39 PM
 *  Author: senthu
 */ 


#ifndef SYSTEMCLOCK_H_
#define SYSTEMCLOCK_H_

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/*! \brief This macro will protect the following code from interrupts. */
#define AVR_ENTER_CRITICAL_REGION( ) uint8_t volatile saved_sreg = SREG; \
                                     cli();

/*! \brief This macro must always be used in conjunction with AVR_ENTER_CRITICAL_REGION
 *        so the interrupts are enabled again.
 */
#define AVR_LEAVE_CRITICAL_REGION( ) SREG = saved_sreg;


void CCPWrite( volatile uint8_t * address, uint8_t value );
uint8_t ClockSrc_Sel( CLK_SCLKSEL_t clockSource );
void set_32MHz_clk(void);


#endif /* SYSTEMCLOCK_H_ */