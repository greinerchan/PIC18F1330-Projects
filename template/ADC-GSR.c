// Example: 1 Second Delay with 1 MHz clock
// Instruction period = 4 * 1usec = 4 usec
// with prescale of 8, 8 * 4 usec = 32 usec
// 1sec divided by 32usec = 31250
// 65,535 - 31250 = 34,285
// Load Initial Count of 34,285 = 85ED


#include <xc.h>

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
char result;
// CONFIG1H
#pragma config OSC = INTIO1       // Oscillator (External RC oscillator, port function on RA6)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = BOHW       // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3L
#pragma config PWMPIN = OFF     // PWM Output Pins Reset State Control bit (PWM outputs disabled upon Reset)
#pragma config LPOL = HIGH      // Low-Side Transistors Polarity bit (Even PWM Output Polarity Control bit) (PWM0, PWM2 and PWM4 are active-high (default))
#pragma config HPOL = HIGH      // High Side Transistors Polarity bit (Odd PWM Output Polarity Control bit) (PWM1, PWM3 and PWM5 are active-high (default))

// CONFIG3H
#pragma config FLTAMX = RA5     // FLTA Mux bit (FLTA input is muxed onto RA5)
#pragma config T1OSCMX = LOW    // T1OSO/T1CKI MUX bit (T1OSO/T1CKI pin resides on RB2)
#pragma config MCLRE = ON       // Master Clear Enable bit (MCLR pin enabled, RA5 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Reset on stack overflow/underflow enabled)
#pragma config BBSIZ = BB256    // Boot Block Size Select bits (256 Words (512 Bytes) Boot Block size)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled)

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit Block 0 (000800-000FFF) (Block 0 is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit Block 1 (001000-001FFF) (Block 1 is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Code Protection bit (Boot Block Memory Area) (Boot Block is not code-protected)
#pragma config CPD = OFF        // Code Protection bit (Data EEPROM) (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit Block 0 (000800-000FFF) (Block 0 is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit Block 1 (001000-001FFF) (Block 1 is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Write Protection bit (Configuration Registers) (Configuration registers are not write-protected)
#pragma config WRTB = OFF       // Write Protection bit (Boot Block Memory Area) (Boot Block is not write-protected)
#pragma config WRTD = OFF       // Write Protection bit (Data EEPROM) (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit Block 0 (000800-000FFF) (Block 0 is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit Block 1 (001000-001FFF) (Block 1 is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Table Read Protection bit (Boot Block Memory Area) (Boot Block is not protected from table reads executed in other blocks)

int  sgvar;
char result;
//  MY FUNCTIONS

void Delay_Sec(int i2)
{
       int iseconds;
       sgvar = sgvar + 1;
          for (iseconds = 0; iseconds < i2;  iseconds++)
  {

    INTCONbits.TMR0IF = 0;          //Clear the Timer 0 Interrupt Flag

    TMR0H = 0x00;
    TMR0L = 0x00;



     

   T0CONbits.TMR0ON = 0;            // Turn Timer 0 OFF

  }
}

   

// END MY Functions

int main(void)
{

// SET UP INTERNAL CLOCK TO 1 MHz

    OSCCON = 0x6C; //changed to 4 MHz

//PORT PIN ASSIGNMENTS    
    
    // Initialize PORT B Bit for digital output
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB1 = 0;
    TRISBbits.RB5 = 1;  
    PORTBbits.RB1 = 0;
 result=TMR0L;
    TRISAbits.RA3 = 0;         // Make RA0 pin output (GREEN LED)
    TRISBbits.RB1 = 0;          // Make RB0 pin output  (RED LED)
    // Initialize PORTA Bit 1 as an analog input (not digital)

    ADCON1bits.PCFG1 = 0;
    TRISAbits.RA1 = 1;
            
// SET UP THE ANALOG-DIGITAL CONVERTER

    ADCON2bits.ADFM = 0;        // Left Justify
    ADCON1bits.VCFG0 = 0;        //  Use 5V Vdd as the reference

    ADCON0bits.CHS1 = 0;     //Select Channel 1
    ADCON0bits.CHS0 = 1;

    ADCON2bits.ACQT2 = 0;    // Select Aquisition Time
    ADCON2bits.ACQT1 = 0;
    ADCON2bits.ACQT0 = 0;

    ADCON2bits.ADCS2 = 1;    // Select Conversion Clock
    ADCON2bits.ADCS1 = 0;
    ADCON2bits.ADCS0 = 0;

    ADCON0bits.ADON = 1;     // Turn on ADC Module

       
//SET UP TIMER 0

    T0CONbits.T0CS = 0;          // Select Timer not Event counter
 
    T0CONbits.PSA = 1;           //Select Prescaler

    //Initialize Timer - Set Prescale to divide by 8

    T0CONbits.T0PS2 = 0;
    T0CONbits.T0PS1 = 1;
    T0CONbits.T0PS0 = 0;
    result=TMR0L;
    INTCONbits.GIE = 1; //Allow low priority interrups 
     INTCONbits.INT0E = 1;    //enable Interrupt 0 (RA0 as interrupt)
    INTCON2bits.INTEDG0 = 1; //cause interrupt at rising edge
    INTCONbits.INT0F = 0;    //clear interrupt flag
 
   INTCONbits.PEIE = 1; // enable peripheral interrupts
   

    // end Timer Initialization
       
     
           sgvar = 0;
           
//Start_Test
           
        TMR0L=0;  
        ADCON0bits.GO = 1;              // Start A/D Conversion
        while (ADCON0bits.GODONE == 1)
     //   {}
T0CONbits.TMR0ON = 0;
       if(ADRESH >= 0x19 )
       {PORTBbits.RB2 = 1;}
        

     
       
    if (result >= 0x01 && result <= 0x55)
{
    
             
      PORTAbits.RA3 = 1;       // Make RA0 high (GREEN LED ON)
      PORTBbits.RB1 = 0;       // Make RA0 high (RED LED OFF)
}
if (result >= 0x55)
{
          
       PORTAbits.RA3 = 0;       // Make RA0 high (GREEN LED OFF)
       PORTBbits.RB1 = 1;       // Make RA0 high (RED LED ON)


           } 
    
     
          if (PORTBbits.RB5 ==1)
          {
              TMR0L=0;
              result=0;
                PORTAbits.RA3 =0;       
             PORTBbits.RB1 = 0;  
              
   
    }  


}




