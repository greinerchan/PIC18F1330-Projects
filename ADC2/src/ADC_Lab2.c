
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

unsigned char LED_Output = 0; 	//assigned to PORT to light
								//connected LEDs
bit toggle = 0;//Used to generate waveform on RC0
unsigned char TMR0_preload = 0; //Varied by ADRESH to change
								//frequency of
								//waveform on RC0

//----------------------PROGRAM MEMORY----------------------


/** PRIVATE PROTOTYPES *********************************************/
void Initialize(void);
void Get_Inputs(void);
void Decide(void);
void Do_Outputs(void);
void Timing(void);

/**SUPPORT ROUTINES*************************************************/


/**INTERRUPT CODE*************************************************/


/**SOFTWARE CONTROL LOOP CODE*************************************/
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
		Timing();
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
	PORTB = 0;
	//Disable pin output driver (See TRIS register)
	TRISBbits.TRISB4 = 1;
	// Configure pin as analog
	ANSELHbits.ANS10 = 1;
	//Configure RC0 as digital output
	TRISCbits.TRISC0 = 0;
	ANSELbits.ANS4 = 0;

	//Configure Timer0 as follows:
	//Select the FOSC/4 internal instruction clock
	//as the clock source for TMR0
	OPTION_REGbits.T0CS = 0;
	//Increment TMR0 value on low-to-high transition
	//of the FOSC/4
	OPTION_REGbits.T0SE = 0;
	//Assign the prescaler to TMR0
	OPTION_REGbits.PSA = 1;
	//Configure the prescaler to increment TMR0
	//at a rate of 1:16
	OPTION_REGbits.PS = 0b011;
	
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
	//Give ADC hold capacitor time to charge
        __delay_us(8);
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
	//Shift the ADRESH result to the left
	//by three bit positions and assign to
	//TMR0_preload
	TMR0_preload = ADRESH<<3;
	//XOR the toggle value with 1
	toggle ^= 1;
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
	//Assign the toggle value to RC0 pin
	PORTCbits.RC0 = toggle;
}

/*----------------------------------------------------------
	Subroutine: Timing
	Parameters: none
	Returns:	nothing
	Synopsys:	Controls the how fast code in the main() is executed 
----------------------------------------------------------*/
void Timing(void)

{
	//Clear the T0IF
	INTCONbits.T0IF = 0;
	
	//Subtract the TMR0_preload value from 255 and
	//then use to preload TMR0
	TMR0 = 255 - TMR0_preload;
	
	//Sit here and wait for TMR0 to overflow
	while(INTCONbits.T0IF == 0);
}

