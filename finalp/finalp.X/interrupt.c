// CONFIG


#include <xc.h>
#include <pic18f1330.h>
////////////////////////////////////
#pragma config FOSC=INTRCIO 
#pragma config WDTE=OFF 
#pragma config PWRTE=OFF
#pragma config MCLRE=OFF			
#pragma config CP=OFF
#pragma config CPD=OFF
#pragma config BOREN=OFF
#pragma config IESO=OFF
#pragma config FCMEN=OFF
////////////////////////////////////////
#define _XTAL_FREQ 4000000
//////////////////////////////////////////
void Initialize(void);
void Get_Inputs(void);
void Decide(void);
void Do_Outputs(void);
void Timing(void);
///////////////////////////////////////////
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
	TRISBbits.TRISB4 = 0;          // Port A - analog 
    TRISAbits.TRISA0 = 1;
    ADCON0= 0X00;
    ADCON1 = 0b00001110;
    ADCON2 =
}
	
/*----------------------------------------------------------
	Subroutine: Get_Inputs
	Parameters: none
	Returns:	nothing
	Synopsys:	Obtains all off-chip inputs
----------------------------------------------------------*/
void Get_Inputs(void)
{

	//Start conversion by setting the GO/DONE bit.
	ADCON0bits.GO_nDONE = 1;
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
    if(TRISA0>24)
{
  
 
  }
	
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

}

/*----------------------------------------------------------
	Subroutine: Timing
	Parameters: none
	Returns:	nothing
	Synopsys:	Controls the how fast code in the main() is executed 
----------------------------------------------------------*/
void Timing(void)

{
	
}

