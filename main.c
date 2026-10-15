/*include section*/
#include "config.h"
#include "app.h"

void main()
{
    // clock configuration 
    OSCCON = 0x38;  //internal oscilator clock speed at 500 khz
    // setting the pin for the led fucntions
    TRISBbits.TRISB0 = 0; //configure portB0 as an output port where the led is connected
    
    //setting the pin for the push button to change the state
    TRISAbits.TRISA4 = 1; // configure as input digital pi 
    ANSELAbits.ANSA4 = 0; // disable analog function of the pin
    
    //configurtion of the timing source 
    T1CON = 0x05; // instruction clock, 1:1 prescaler , no sync , timer on 
    // preload the timer for an accurate overflow at 500 ms 
    TMR1H = 0x0B;
    TMR1L = 0xDC;
    
    
    
    while(1){
        if (TMR1IF)
        {
            ledToggle();
            TMR1H = 0x0B;
            TMR1L = 0xDC;
            TMR1IF = 0;
            
        }
    
    }
}

