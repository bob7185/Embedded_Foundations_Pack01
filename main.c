/*include section*/
#include "config.h"
#include "app.h"


void __interrupt() timer_int(void){
    if(TMR1IE && TMR1IF){
            TMR1H = 0x0B;
            TMR1L = 0xDC;
            TMR1IF = 0;
            ledToggle();
    }
}

void main()
{
    // clock configuration 
    OSCCON = 0x38;  //internal oscillator clock speed at 500 khz

    // setting the pin for the led fucntions
    TRISBbits.TRISB0 = 0; //configure portB0 as an output port where the led is connected
    TRISBbits.TRISB1 = 0; // diagnostic pin
    
    //setting the pin for the push button to change the state
    TRISAbits.TRISA4 = 1; // configure as input digital pi 
    ANSELAbits.ANSA4 = 0; // disable analog function of the pin
  
   
    //TTimer 1 configuration for a nominal 500 ms overflow 
    T1CONbits.TMR1CS = 0 ; //instruction  clock
    T1CONbits.T1CKPS = 0; // 1:1 prescaler
    T1CONbits.nT1SYNC = 1; // no sync
    TMR1H = 0x0B;
    TMR1L = 0xDC;


    // Timer 0 configuration for about 1.02 ms per tick
    OPTION_REGbits.PSA = 0; 
    OPTION_REGbits.PS = 0b110;
    OPTION_REGbits.TMR0CS = 0 ; // instruction clock
    
    ButtonState_t buttonState = BUTTON_WAIT_PRESS;
    SystemState_t systemState = RUNNING;
    LATBbits.LATB1 = 0; 

    TMR1IF = 0; // must be cleared before enabling interrupts
    PIE1bits.TMR1IE = 1; // enable timer1 interrupt
    INTCONbits.PEIE = 1; //enable all active  peripheral interrupts
    INTCONbits.GIE = 1; //enable all active interrupts
    T1CONbits.TMR1ON = 1 ; // start timer 1
    
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
                // allow at least 10 ms for mechanical bouncing to settle then check
                if( TMR0 >= 10)
                {
                    if (SWTCH_PIN == 0)
                    {
                    buttonState = BUTTON_WAIT_RELEASE;
                    }
                     else
                    {
                    buttonState = BUTTON_WAIT_PRESS;
                     }
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
                if(TMR0 >= 10)
                {
                    if(SWTCH_PIN == 1)
                    {
                      buttonState = BUTTON_WAIT_PRESS;
                    if (systemState == RUNNING)
                    {
                       PIE1bits.TMR1IE = 0; // disable timer1 interrupt
                       T1CONbits.TMR1ON = 0; // stop the timer
                       TMR1IF = 0; // clear the flag
                       LATBbits.LATB1 = 1;
                       LED_PIN = 0;   // turn off the led
                       systemState = PAUSED; 
                      
                    }
                    else 
                    {
                        TMR1H = 0x0B;
                        TMR1L = 0xDC;
                        TMR1IF = 0;
                        PIE1bits.TMR1IE = 1; // enable timer1 interrupt
                        T1CONbits.TMR1ON = 1 ;
                        LATBbits.LATB1 = 0;
                        systemState = RUNNING;
                    
                    }
                    
                    }
                else
                {
                     buttonState = BUTTON_WAIT_RELEASE;
                    
                }
                   
                }
                
            }
                break;
        }

    }
}

