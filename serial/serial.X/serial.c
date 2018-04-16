/*
 * File:   serial.c
 * Author: boy52
 *
 * Created on April 17, 2017, 1:45 PM
 */


#include <xc.h>
#include<p18f1330.h>

void main(void) {
    TRISBbits.RB0=0
            RCSTA=0X90;
    SPBRG=15;
    while(1)
    {
        while(PIR1bits.RCIF==0)
            PORTB=RCREG;
    }
}
