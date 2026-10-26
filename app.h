#ifndef APP_H__
#define APP_H__
/*******************************************************************************
 * ledToggle()
 * reads the current led status and toggles it
 * 
 * 
 *******************************************************************************/

#include <xc.h>
#define LED_PIN  LATBbits.LATB0   // change accordingly to where you have the led connected 
#define SWTCH_PIN PORTAbits.RA4


// defini g the enum type for the swtch state variable 

typedef enum {
    
        BUTTON_WAIT_PRESS,
        BUTTON_DEBOUNCE_PRESS,
        BUTTON_WAIT_RELEASE,
        BUTTON_DEBOUNCE_RELEASE            
 }ButtonState_t;
 
 typedef enum {
     RUNNING,
     PAUSED
 }SystemState_t;
 
void ledToggle();

#endif /* __APP_H__*/