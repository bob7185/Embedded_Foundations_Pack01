#ifndef APP_H__
#define APP_H__
/*******************************************************************************
 * ledToggle()
 * reads the current led status and toggles it
 * 
 * 
 *******************************************************************************/

#include <xc.h>
#define LED_ON  1   // based on hardware circuit a logic  high will lit up the led 
#define LED_OFF 0  // based on board wiring a logic low will turn off the  Led
#define LED_PIN  LATBbits.LATB0   // change accordingly to where you have the led connected 

void ledToggle();

#endif /* __APP_H__*/