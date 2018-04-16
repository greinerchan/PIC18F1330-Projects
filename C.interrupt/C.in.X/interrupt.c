#include <xc.h>
#include<p18f1330.h>


 
 /** M A I N *****************************************/ 
 void main(void) 
 { 
     TRISBbits.TRISB4 = 0;          // Port A - analog 
     TRISAbits.TRISA0 = 1; // RA0 - input 
     PORTBbits.RB4 = 0;            // Port C - analog 
     ADCON1bits.PCFG0 =1;     
 
     //Interrupt configuration 
     INTCONbits.GIE = 1; //Allow low priority interrups 
     INTCONbits.INT0E = 1;    //enable Interrupt 0 (RA0 as interrupt)
    INTCON2bits.INTEDG0 = 1; //cause interrupt at rising edge
    INTCONbits.INT0F = 0;    //clear interrupt flag
ei(); 
     while (1) ; 
 {   
       if(INTCONbits.INT0F == 1)
        PORTBbits.RB4 = 1;
        INTCONbits.INT0F = 0;
       
    }
 }  