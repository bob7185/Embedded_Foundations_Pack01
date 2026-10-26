/*include section*/
#include "config.h"
#include "app.h"

void main()
{
    // clock configuration 
    OSCCON = 0x38;  //internal oscillator clock speed at 500 khz
    // setting the pin for the led fucntions
    TRISBbits.TRISB0 = 0; //configure portB0 as an output port where the led is connected
    
    //setting the pin for the push button to change the state
    TRISAbits.TRISA4 = 1; // configure as input digital pi 
    ANSELAbits.ANSA4 = 0; // disable analog function of the pin
  
   
    //TTimer 1 configuration for an exact 500 ms overflow 
    TMR1H = 0x0B;
    TMR1L = 0xDC;
    //T1CON = 0x05; // instruction clock, 1:1 prescaler , no sync , timer on 
    T1CONbits.TMR1CS = 0 ; //instruction  clock
    T1CONbits.T1CKPS = 0; // 1:1 prescaler
    T1CONbits.nT1SYNC = 1; // no sync
    T1CONbits.TMR1ON = 1 ; // start timer 1
    
    
    // Timer 0 configuration for about 1.02 ms per tick
    OPTION_REGbits.PSA = 0; 
    OPTION_REGbits.PS = 0b110;
    OPTION_REGbits.TMR0CS = 0 ; // instruction clock
    
    
    
    
    
    ButtonState_t buttonState = BUTTON_WAIT_PRESS;
    SystemState_t systemState = RUNNING;
    
    while(1){
        
        switch(buttonState)
        {
            case BUTTON_WAIT_PRESS:
            {
                if (SWTCH_PIN == 0)
                {
                    buttonState = BUTTON_DEBOUNCE_PRESS;
                    TMR0 = 0;
                }
            }
            break;
            case BUTTON_DEBOUNCE_PRESS:
            {
                // if button has been low for more than 10 s  valid press swithc to wait for release
                if( TMR0 > 2 && SWTCH_PIN == 0)
                {
                    buttonState = BUTTON_WAIT_RELEASE;
                }
                
            }
                break;
            case BUTTON_WAIT_RELEASE:
            {
                if (SWTCH_PIN == 1){
                    buttonState = BUTTON_DEBOUNCE_RELEASE;
                    TMR0 = 0;
                }
            
            }
                break;
            case BUTTON_DEBOUNCE_RELEASE:
            {
                if(TMR0 > 2 && SWTCH_PIN == 1)
                {
                     buttonState = BUTTON_WAIT_PRESS;
                    if (systemState == RUNNING)
                    {
                       systemState = PAUSED;
                      
                    }
                    else 
                    {
                        systemState = RUNNING;
                                   TMR1H = 0x0B;
                                  TMR1L = 0xDC;

                            T1CONbits.TMR1ON = 1 ;
                    }
                }
            
            }
                break;
        }
        if (systemState == RUNNING)
        {
     
        if (TMR1IF)
        {
            ledToggle();
            TMR1H = 0x0B;
            TMR1L = 0xDC;
            TMR1IF = 0;
            
        }
        }
        else if (systemState == PAUSED) {
         T1CONbits.TMR1ON = 0 ;// stop the timer 
         LED_PIN = 0;   // turn off the led
        }

    
    }
}

