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

//My Variables

char  Data_Received;
char  Dummy_Var;

//My functions

void Delay_Sec(int i2)
{
       int iseconds;

          for (iseconds = 0; iseconds < i2;  iseconds++)
  {

//Clear the Timer 0 Interrupt Flag

    INTCONbits.TMR0IF = 0;

// Example: 1 Second Delay with 1 MHz clock
// Instruction period = 4 * 1usec = 4 usec
// with prescale of 8, 8 * 4 usec = 32 usec
// 1sec divided by 32usec = 31250
// 65,535 - 31250 = 34,285
// Load Initial Count of 34,285 = 85ED

    TMR0H = 0x85;
    TMR0L = 0xED;

// Turn Timer 0 ON

    T0CONbits.TMR0ON = 1;

// Monitor Timer 0 Interrupt Flag

    while(TMR0IF == 0)
       {

       }

// Turn Timer 0 OFF
   T0CONbits.TMR0ON = 0;
          }

}
// END MY Functions
void interrupt switch_pushed(void)
{
    if(PORTAbits.RA5 = 1)
    {  
        Data_Received= RCREG;
}
}
int main(void)
{
    // Initialize internal clock to 1 MHz

    OSCCON = 0x6C; //changed to 4 MHz

// Initialize PORT B Bit 4 as output and Value Low


    TRISBbits.TRISB4 = 0;
    PORTBbits.RB4 = 0;

 // Set up Serial Communication
 
    // SET UP BAUD RATE
    // SPBRG =  ((FOSC/Desired Baud Rate)/16) ? 1
 
   SPBRG = 25;          // Set Up for 9.6 Baud serial comm
   BAUDCONbits.BRG16 = 0;   // Use 8-bit reg for SPBRG  
   TXSTAbits.BRGH = 1; // Select high baud rate options
  
 // Serial Data Selections  
   
   TXSTAbits.SYNC = 0;     // Asynch mode
   TXSTAbits.TX9 = 0;      // 8 bit transmission
   BAUDCONbits.TXCKP = 0;  // High when Idling
   

   TRISAbits.RA3 = 1;   //RX as input
   TRISAbits.RA2 = 1;   // TX as output (contradicts datasheet
   TRISBbits.RB5 = 1;
 

   TXSTAbits.TXEN = 1;  //enable transmit
   RCSTAbits.SPEN = 1;  //enable Serial Port
   RCSTAbits.CREN = 1;  // enable Continuous Receiving

//   PIR1bits.RCIF = 1;   // enalbe Receive Interrupt Flag
//   PIR1bits.TXIF = 1;   // enable Transmit Interupt Flag
   INTCONbits.GIE = 1;  // enable (golobal) interrupts
   INTCONbits.PEIE = 1; // enable peripheral interrupts
   PIE1bits.RCIE = 1;   // enable the Recieve Interrupt
   PIE1bits.TXIE = 0;  // Dis Enable Transmit Interrupt

//  Initialize Timer for 16-Bit Operation

    T0CONbits.T08BIT = 0;

//Initialize Timer function as Timer (not counter)
// Select Timer clock source as the internal clock

    T0CONbits.T0CS = 0;

//Initialize Timer - Select Prescaler

    T0CONbits.PSA = 0;

//;Initialize Timer - Set Prescale to divide by 8
//  See Timer 0 Control Register info for Prescale values

    T0CONbits.T0PS2 = 0;
    T0CONbits.T0PS1 = 1;
    T0CONbits.T0PS0 = 0;
  
 RCREG = 0;
  while (1)
  {
      while (RCIF==0) {}
      Data_Received= RCREG;

      if(Data_Received == 0x0F)
      {
          PORTBbits.RB4 = 1;
          Delay_Sec(3);
          PORTBbits.RB4 = 0;
          PIR1bits.TXIF = 0;
      }


      if(Data_Received == 0x87)
      {
          PORTBbits.RB4 = 1;
          Delay_Sec(1);
          PORTBbits.RB4 = 0;
          Delay_Sec(1);
          PORTBbits.RB4 = 1;  
          Delay_Sec(1);
          PORTBbits.RB4 = 0;
          PIR1bits.TXIF = 0;
      }
      RCREG = 0;
    }
}




   



