/********************************************************************
 FileName:     	ADC_Lab1.c
 Dependencies:	See INCLUDES section
 Processor:		PIC16F690 Microcontrollers

 Compiler:		Microchip XC8 (FREE)
 Company:		Microchip Technology, Inc.
 Engineer:		<Your Name Here>

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the "Company") for its PIC® Microcontroller is intended and
 supplied to you, the Company's customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

********************************************************************
 File Description:

 Change History:
  Rev   Date     By      Description
  XC8_1 02/01/15 Ian.M   Ported code to XC8

********************************************************************/
#ifndef __PICCPRO__  // Fix for borked error hiliting on delays in MPLAB X
#define __PICCPRO__
#endif

#include	<xc.h>

//----------------------CONFIGURE DEVICE--------------------
 
#pragma config FOSC=INTRCIO 
#pragma config WDTE=OFF 
#pragma config PWRTE=OFF
#pragma config MCLRE=OFF			
#pragma config CP=OFF
#pragma config CPD=OFF
#pragma config BOREN=OFF
#pragma config IESO=OFF
#pragma config FCMEN=OFF

//----------------------MACROS & DEFINES--------------------
			
#define _XTAL_FREQ 4000000 // For XC8 delays: Fosc=4MHz

//-----------------------DATA MEMORY------------------------

unsigned char LED_Output = 0;	//assigned to LED PORT to light
								//connected LEDs


//----------------------PROGRAM MEMORY----------------------


/** PRIVATE PROTOTYPES *********************************************/
void Initialize(void);
void Get_Inputs(void);
void Decide(void);
void Do_Outputs(void);
void Timing(void);


/**SUPPORT ROUTINES*************************************************/


/**INTERRUPT CODE***************************************************/


/**SOFTWARE CONTROL LOOP CODE***************************************/
/*----------------------------------------------------------
	Subroutine: main
	Parameters: none
	Returns:	nothing
	Synopsys:	Main program function
-----------------------------------------------------------*/
void main(void)
{

	Initialize(); //Initialize the relevant registers
	while(1)
	{
		Get_Inputs();
		Decide();
		Do_Outputs();
	}
	
}

/*----------------------------------------------------------
	Subroutine: INITIALIZE
	Parameters: none
	Returns:	nothing
	Synopsys:	Initializes all registers 
				associated with the application by calling
				initialization functions for each peripheral
----------------------------------------------------------*/
void Initialize(void)
{
	//Configure Port:
	//Disable pin output driver (See TRIS register)
	TRISBbits.TRISB4 = 1;
	// Configure pin as analog
	ANSELHbits.ANS10 = 1;
	//Configure RC0, RC1, RC2 and RC3 as digital output
	PORTC = 0;
	TRISCbits.TRISC0 = 0;
	TRISCbits.TRISC1 = 0;
	TRISCbits.TRISC2 = 0;
	TRISCbits.TRISC3 = 0;
	ANSELbits.ANS4 = 0;
	ANSELbits.ANS5 = 0;
	ANSELbits.ANS6 = 0;
	ANSELbits.ANS7 = 0;
	//Configure the ADC module:
	//Select ADC conversion clock Frc
	ADCON1bits.ADCS = 0b111;

	//Configure voltage reference using VDD
	ADCON0bits.VCFG = 0;
	//Select ADC input channel Pin 13 (RB4/AN10)
	ADCON0bits.CHS = 10;
	//Select result format left justified
	ADCON0bits.ADFM = 0;
	//Turn on ADC module
	ADCON0bits.ADON = 1;
}
	
/*----------------------------------------------------------
	Subroutine: Get_Inputs
	Parameters: none
	Returns:	nothing
	Synopsys:	Obtains all off-chip inputs
----------------------------------------------------------*/
void Get_Inputs(void)
{
	//Perform an ADC of potentiometer connected to pin 13
	//Wait the required acquisition time
	__delay_ms(1);
	//Start conversion by setting the GO/DONE bit.
	ADCON0bits.GO_nDONE = 1;
	//Wait for ADC conversion to complete
	//Polling the GO/DONE bit
	// 0 = ADC completed
	// 1 = ADC in progress
	while(ADCON0bits.GO_nDONE == 1);
}

/*----------------------------------------------------------
	Subroutine: Decide
	Parameters: none
	Returns:	nothing
	Synopsys:	Function that calls all sub-routines for 
				processing the input data
----------------------------------------------------------*/
void Decide(void)
{
	//Assign the upper 4 bits of ADRESH to the lower 4 bits
	//of LED_Output
	LED_Output = ADRESH >> 4; 	//Shifts the bits in ADRESL 4 bits
								//to the right
}

/*----------------------------------------------------------
	Subroutine: Do_Outputs
	Parameters: none
	Returns:	nothing
	Synopsys:	Ouputs to off-chip circuitry based off of
				Decide function
----------------------------------------------------------*/
void Do_Outputs(void)
{
	//Assign contents of LED_Output to PORTC to light the connected
	//LEDs
	PORTC = LED_Output;
}

/*----------------------------------------------------------
	Subroutine: Timing
	Parameters: none
	Returns:	nothing
	Synopsys:	Controls the how fast code in the main() is executed 
----------------------------------------------------------*/
void Timing(void)

{
	//ADD TIMING CODE HERE
}

