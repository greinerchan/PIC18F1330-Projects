#include <xc.h>

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

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
#pragma config MCLRE = OFF       // Master Clear Enable bit (MCLR pin enabled, RA5 input pin disabled)

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

//My Variables

char  ForXmit;

//My functions

void Delay_Sec(int i2)
{
       int iseconds;

          for (iseconds = 0; iseconds < i2;  iseconds++)
  {

    INTCONbits.TMR0IF = 0;    //Clear Timer 0 Interrupt Flag

// Example: 1 Second Delay with 1 MHz clock
// Instruction period = 4 * 1usec = 4 usec
// with prescale of 8, 8 * 4 usec = 32 usec
// 1sec divided by 32usec = 31250
// 65,535 - 31250 = 34,285
// Load Initial Count of 34,285 = 85ED

    TMR0H = 0x85;
    TMR0L = 0xED;

    T0CONbits.TMR0ON = 1;     // Turn Timer 0 ON

    while(TMR0IF == 0)      // Monitor Timer 0 Interrupt Flag
       {

       }

   T0CONbits.TMR0ON = 0;   // Turn Timer 0 OFF

  }
}

// END MY Functions

int main(void)
{
    OSCCON = 0x6C; //Initialize Clock to 4 MHz

    TRISBbits.TRISB0 = 1;   //PORT B Bit 0,1 as input
    TRISBbits.TRISB1 = 1;
    
 // Set up Serial Communication
 
 // SET UP BAUD RATE  SPBRG = ((FOSC/Desired Baud Rate)/16)
 
   SPBRG = 25;          // Set Up for 9.6 Baud serial comm
   BAUDCONbits.BRG16 = 0;   // Use 8-bit reg for SPBRG  
   TXSTAbits.BRGH = 1; // Select high baud rate options
  
 // Serial Data Selections  
   
   TXSTAbits.SYNC = 0;     // Asynch mode
   TXSTAbits.TX9 = 0;      // 8 bit transmission
   BAUDCONbits.TXCKP = 0;  // High when Idling
   
//Enabling Serial Communication   
   
   RCSTAbits.SPEN = 1;  //enable Serial Port
   TXSTAbits.TXEN = 1;  //enable transmit
 //  TRISAbits.RA2 = 1;   // TX as output (contradicts datasheet?
 
   
//Initialize Timer 
   
    T0CONbits.T08BIT = 0;  //  Init Timer for 16-Bit Operation
    T0CONbits.T0CS = 0;  //Timer function as Timer (not counter)
    T0CONbits.PSA = 0;   //Initialize Timer - Select Prescaler

// Timer - Set Prescale to divide by 8

    T0CONbits.T0PS2 = 0;
    T0CONbits.T0PS1 = 1;
    T0CONbits.T0PS0 = 0;

 // end Timer Initialization


   //TEMP for DEBUG - keep sending 55

       while(0)
   {

     TXREG = 0x23;
     while(TRMT==0)
     {}
     TXREG = 0x45;
     while(TRMT==0)
     {}
     Delay_Sec(1);
   }
    
//   SPBRG = 124;          // Set Up for 9.6 Baud serial comm
//   BAUDCONbits.BRG16 = 0;   // Use 8-bit reg for SPBRG  
//   TXSTAbits.BRGH = 0; // Select high baud rate options
 //  Delay_Sec(1);
   
     while(1)
     {
              
         if(PORTBbits.RB1 ==0)
         {ForXmit = 0x0F;}
         if(PORTBbits.RB1 == 1)
         {ForXmit = 0x87;}    
           
         if(PORTBbits.RB0 == 1 ) 
            {
             TXREG = ForXmit;
             Delay_Sec(1);
            }     
         
     }   
   
    


   }



