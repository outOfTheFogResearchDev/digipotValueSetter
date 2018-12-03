// filtertest1.cpp : Defines the entry point for the console application.
//

//	Created by:  David G. Brown, BR Technical Consulting, Inc.
//
//	Version:	1.0
//
//		This is a simple Win-32 console application for writing data
//	to an ACESS I/O, model USB-DIO-32, USB based digital I/O cable. 
//	The code performs setup/ initialization of the USB-DIO-32 device, 
//  takes in a user input frequency (between 225 and 400 MHz, in MHz), 
//	and then WRITEs an 8-bit frequency control word to PORT B of the device
//  using an SPI data scheme.  
//
//  Each 8-bit code written is a frequency code word required by the 
//  pole-zero filter to generate the correct tune frequency for the 
//  filter.  The filter will then tune to the correct user-input tune frequency.
//
//	Code has also been included to calculate the 8-bit tuning control
//  word required by the pole-zero filter 
//
//	Required Headers/ Files:
//		The project requires the "stdafx.h" and "AIOUSB.H" header files
//	as well as the "VCAIOUSB.lib" file.
//
// Port assignments:  B0=SDATA, B1=SCLK, and B2=/STROBE
//

#include "stdafx.h"
#include "AIOUSB.H"
#include <math.h>
#include <ctype.h>
#include <time.h>

void sleep(unsigned int mseconds)
{
	clock_t goal = mseconds + clock();
	while (goal > clock());
}





int main(int argc, char* argv[])
{

	unsigned long deviceIndex, PID;
	unsigned long outMask = 0x07, DIOdata = 0xFFFFFFFF;
	unsigned char *data = NULL;
	unsigned char buffer[16];
	unsigned long status;
	unsigned long com_code=0x00000000, addr=0x00000000, offset=0x00000000;
	unsigned long com_word=0x00000000, offset1=0x00000000, offset2=0x00000000;

	int op_num, res_num, reg_num, stop, resp1, resp2;
	int temp, j,res_code, proceed;
	double x, volt, res_code_fl, Vref = 2.5;



// Assume that we are only using one ACCES I/O device in the USB chain.
	deviceIndex = diOnly;

// Query ACCES I/O device for status.
	status = QueryDeviceInfo(deviceIndex, &PID, NULL, NULL, NULL, NULL);

// If status is not 0, send error message.
	if (status != 0) {
		printf("QueryDeviceInfo() returned with error code: %lu\n", status);
		goto ErrorExit;
	}

// Display device ID: 
	printf("Product ID: 0x%x\n", PID);

// Configure device for writing to first 3 DIO ports (Ports A-C write, Port D read).
	DIO_Configure(deviceIndex, false, &outMask, &DIOdata);

	proceed = 1;	//Initialize proceed variable

	while(proceed != 0)
	{

//	Get user info:

		// Get operation to perform:
		printf("Choose an operation to perform:\n");
		printf("0 -- Write Data to Volatile Register;\n");
		//printf("1 -- Increment Resistance;\n");
		//printf("2 -- Decrement Resistance;\n");
		printf("3 -- Read Data from Volatile Register;\n");
		printf("4 -- Write Data to NON-Volatile Register;\n");
		printf("5 -- Write default data to all 12 NON-Volatile registers;\n");
		printf("6 -- Adjust potentiometer wiper setting;\n");
		scanf("%d", &op_num);

		//printf("point 1\n");

//-------------------------------------------------------------------------------------
//  Write data to potentiometer voltile wiper memory
//-------------------------------------------------------------------------------------
		if (op_num == 0) {  //Write data option
			com_code = 0x00;

			// Get resistor number:
			printf("Which resistor? (1-12)\n");
			scanf("%d", &res_num);

			// Set resistor register addresses and IO bank offsets.
			if (res_num == 3) {
				addr = 0x06;
				offset = 0;  //Bank A

			} else if (res_num == 2) {
				addr = 0x07;
				offset = 0;  //Bank A

			} else if (res_num == 1) {
				addr = 0x01;
				offset = 0;  //Bank A

			} else if (res_num == 4) {
				addr = 0x00;
				offset = 0;  //Bank A

			} else if (res_num == 7) {
				addr = 0x06;
				offset = 8;  //Bank B

			} else if (res_num == 6) {
				addr = 0x07;
				offset = 8;  //Bank B

			} else if (res_num == 5) {
				addr = 0x01;
				offset = 8;  //Bank B

			} else if (res_num == 8) {
				addr = 0x00;
				offset = 8;  //Bank B

			} else if (res_num == 11) {
				addr = 0x06;
				offset = 16;  //Bank C

			} else if (res_num == 10) {
				addr = 0x07;
				offset = 16;  //Bank C

			} else if (res_num == 9) {
				addr = 0x01;
				offset = 16;  //Bank C

			} else if (res_num == 12) {
				addr = 0x00;
				offset = 16;  //Bank C

			} else {
				printf("Invalid resistor number.  Quitting Program.\n");
				goto ErrorExit;

			}

			// Get the desired output voltage:
			//printf("Output voltage value? (0V<= Vo <=+2.5V)\n");
			//scanf("%f", &volt);

			printf("Code value? (0<= code <=255)\n");
			scanf("%f", &res_code);

			//Calculate resistor tuning value from voltage:
			//res_code_fl = ceil(((volt/Vref)*256.)-.5);
			
			//res_code = (int)res_code_fl;

			printf("Resistance Code is: %d \n",res_code);

			// Set up the 16-bit command word
			com_word = 0x00000000;

			com_word = com_word | (com_code << 10);
			//printf("com_word is %d\n", com_word);

			com_word = com_word | (addr << 12);
			//printf("com_word is %d\n", com_word);

			com_word = com_word | (res_code << 0);
			//printf("com_word is %d\n", com_word);

			// Write serial data to the DIO ports:
			// Port assignments:  x0=HV_EN, x1=/CS, x2=SCLK, and x3=SDI
			// Here 'x' is {A,B, or C}.

			DIO_Write1(deviceIndex, (3+offset), false);	//Set SDI to '0'
			DIO_Write1(deviceIndex, (2+offset), true);	//Set SCLK to '1'	
			DIO_Write1(deviceIndex, (1+offset), true);	//Set /CS to '1'
			DIO_Write1(deviceIndex, (0+offset), true);	//Set HV_EN to '1'

			//Set /CS to '0' to begin write command cycle:
			DIO_Write1(deviceIndex, (1+offset), false);	
			

			for (j = 0; j < 16; j++)  { 

				//Set SCLK to '0'
				DIO_Write1(deviceIndex, (2+offset), false);	

				if (com_word & (1 << (15-j))) {
					//Set SDI to '1'
					DIO_Write1(deviceIndex, (3+offset), true);	

				} else {
					//Set SDI to '0'
					DIO_Write1(deviceIndex, (3+offset), false);	//Set SDI to '0'

				}

				//Set SCLK to '1'
				DIO_Write1(deviceIndex, (2+offset), true);	//Set SCLK to '1'
				//Read SDO data back
				//
				
			}

			//Reset SCLK and /CS
			DIO_Write1(deviceIndex, (2+offset), true);	//Set SCLK to '1'	
			DIO_Write1(deviceIndex, (1+offset), true);	//Set /CS to '1'



				
		} else if (op_num == 1) {  //Increment resistance option
			com_code = 0x01;

		} else if (op_num == 2) {  //Decrement resistance option
			com_code = 0x02;
//-------------------------------------------------------------------------------------
//  Read Current Potentiometer Wiper Settings (Volatile Memory)
//-------------------------------------------------------------------------------------
		} else if (op_num == 3) {  //Read data option
			com_code = 0x03;

			// Get register number:
			printf("Which resistor register to read? (1-12)\n");
			scanf("%d", &reg_num);

			// Set register addresses and IO bank offsets.
			offset1 = 3;  //Bank D

			// Set resistor register addresses and IO bank offsets.
			if (reg_num == 3) {
				addr = 0x06;
				offset = 0;  //Bank A
				offset2 = 0;

			} else if (reg_num == 2) {
				addr = 0x07;
				offset = 0;  //Bank A
				offset2 = 0;

			} else if (reg_num == 1) {
				addr = 0x01;
				offset = 0;  //Bank A
				offset2 = 0;

			} else if (reg_num == 4) {
				addr = 0x00;
				offset = 0;  //Bank A
				offset2 = 0;

			} else if (reg_num == 7) {
				addr = 0x06;
				offset = 8;  //Bank B
				offset2 = 1;

			} else if (reg_num == 6) {
				addr = 0x07;
				offset = 8;  //Bank B
				offset2 = 1;

			} else if (reg_num == 5) {
				addr = 0x01;
				offset = 8;  //Bank B
				offset2 = 1;

			} else if (reg_num == 8) {
				addr = 0x00;
				offset = 8;  //Bank B
				offset2 = 1;

			} else if (reg_num == 11) {
				addr = 0x06;
				offset = 16;  //Bank C
				offset2 = 2;

			} else if (reg_num == 10) {
				addr = 0x07;
				offset = 16;  //Bank C
				offset2 = 2;

			} else if (reg_num == 9) {
				addr = 0x01;
				offset = 16;  //Bank C
				offset2 = 2;

			} else if (reg_num == 12) {
				addr = 0x00;
				offset = 16;  //Bank C
				offset2 = 2;

			} else {
				printf("Invalid resistor number.  Quitting Program.\n");
				goto ErrorExit;

			}

			// Set up the 16-bit command word
			com_word = 0x00000000;

			com_word = com_word | (com_code << 10);
			//printf("com_word is %d\n", com_word);

			com_word = com_word | (addr << 12);
			//printf("com_word is %d\n", com_word);

			// Write serial data to the DIO ports:
			// Port assignments:  x0=HV_EN, x1=/CS, x2=SCLK, and x3=SDI
			// Here 'x' is {A,B, or C}.

			DIO_Write1(deviceIndex, (3+offset), false);	//Set SDI to '0'
			DIO_Write1(deviceIndex, (2+offset), true);	//Set SCLK to '1'	
			DIO_Write1(deviceIndex, (1+offset), true);	//Set /CS to '1'
			DIO_Write1(deviceIndex, (0+offset), true);	//Set HV_EN to '1'

			//Set /CS to '0' to begin write command cycle:
			DIO_Write1(deviceIndex, (1+offset), false);	
			

			for (j = 0; j < 16; j++)  { 

				//Set SCLK to '0'
				DIO_Write1(deviceIndex, (2+offset), false);	

				if (com_word & (1 << (15-j))) {
					//Set SDI to '1'
					DIO_Write1(deviceIndex, (3+offset), true);	

				} else {
					//Set SDI to '0'
					DIO_Write1(deviceIndex, (3+offset), false);	//Set SDI to '0'

				}

				//Set SCLK to '1'
				DIO_Write1(deviceIndex, (2+offset), true);	//Set SCLK to '1'

				//Read SDO data back
				DIO_Read8(deviceIndex, offset1, &(buffer[15-j]));

				
			}

			//Reset SCLK and /CS
			DIO_Write1(deviceIndex, (2+offset), true);	//Set SCLK to '1'	
			DIO_Write1(deviceIndex, (1+offset), true);	//Set /CS to '1'

			//Process Data Read back:

			if (buffer[9] & (1 << offset2)) {
				//if CMDERR bit=1, then process data

				temp = 0;
				x = 0.5;	// Initialize variable to count powers of 2.
			
				for (j = 0; j < 9; j++)  {
					x *= 2;  // Calculate jth power of 2 (i.e., x = 2^j)
					
					printf("Buffer value is: 0x%02lX for index: %d\n\r",buffer[j],j);

					if (buffer[j] & (1 << offset2)) {

						temp += (int)x;

					}
				}

				printf("Data value is: %d\n",temp);
			} else {
				//if CMDERR bit=0, print error message
				printf("CMDERR detected.\n");

			}

//-------------------------------------------------------------------------------------
//  Store data to NV memory
//-------------------------------------------------------------------------------------
		} else if (op_num == 4) {  //Store data option
			com_code = 0x00;

			// Get resistor number:
			printf("Which resistor? (1-12)\n");
			scanf("%d", &res_num);

			// Set resistor register addresses and IO bank offsets.
			if (res_num == 3) {
				//addr = 0x06;
				addr = 0x08;
				offset = 0;  //Bank A

			} else if (res_num == 2) {
				//addr = 0x07;
				addr = 0x09;
				offset = 0;  //Bank A

			} else if (res_num == 1) {
				//addr = 0x01;
				addr = 0x03;
				offset = 0;  //Bank A

			} else if (res_num == 4) {
				//addr = 0x00;
				addr = 0x02;
				offset = 0;  //Bank A

			} else if (res_num == 7) {
				//addr = 0x06;
				addr = 0x08;
				offset = 8;  //Bank B

			} else if (res_num == 6) {
				//addr = 0x07;
				addr = 0x09;
				offset = 8;  //Bank B

			} else if (res_num == 5) {
				//addr = 0x01;
				addr = 0x03;
				offset = 8;  //Bank B

			} else if (res_num == 8) {
				//addr = 0x00;
				addr = 0x02;
				offset = 8;  //Bank B

			} else if (res_num == 11) {
				//addr = 0x06;
				addr = 0x08;
				offset = 16;  //Bank C

			} else if (res_num == 10) {
				//addr = 0x07;
				addr = 0x09;
				offset = 16;  //Bank C

			} else if (res_num == 9) {
				//addr = 0x01;
				addr = 0x03;
				offset = 16;  //Bank C

			} else if (res_num == 12) {
				//addr = 0x00;
				addr = 0x02;
				offset = 16;  //Bank C

			} else {
				printf("Invalid resistor number.  Quitting Program.\n");
				goto ErrorExit;

			}

			// Get the desired output voltage:
			//printf("Output voltage value? (0V<= Vo <=+2.5V)\n");
			//scanf("%f", &volt);

			printf("Code value? (0<= code <=255)\n");
			scanf("%f", &res_code);

			//Calculate resistor tuning value from voltage:
			//res_code_fl = ceil(((volt/Vref)*256.)-.5);
			
			//res_code = (int)res_code_fl;

			printf("Resistance Code is: %d \n",res_code);

			// Set up the 16-bit command word
			com_word = 0x00000000;

			com_word = com_word | (com_code << 10);
			//printf("com_word is %d\n", com_word);

			com_word = com_word | (addr << 12);
			//printf("com_word is %d\n", com_word);

			com_word = com_word | (res_code << 0);
			//printf("com_word is %d\n", com_word);

			// Write serial data to the DIO ports:
			// Port assignments:  x0=HV_EN, x1=/CS, x2=SCLK, and x3=SDI
			// Here 'x' is {A,B, or C}.

			DIO_Write1(deviceIndex, (3+offset), false);	//Set SDI to '0'
			DIO_Write1(deviceIndex, (2+offset), true);	//Set SCLK to '1'	
			DIO_Write1(deviceIndex, (1+offset), true);	//Set /CS to '1'
			DIO_Write1(deviceIndex, (0+offset), true);	//Set HV_EN to '1'

			//Set /CS to '0' to begin write command cycle:
			DIO_Write1(deviceIndex, (1+offset), false);	
			

			for (j = 0; j < 16; j++)  { 

				//Set SCLK to '0'
				DIO_Write1(deviceIndex, (2+offset), false);	

				if (com_word & (1 << (15-j))) {
					//Set SDI to '1'
					DIO_Write1(deviceIndex, (3+offset), true);	

				} else {
					//Set SDI to '0'
					DIO_Write1(deviceIndex, (3+offset), false);	//Set SDI to '0'

				}

				//Set SCLK to '1'
				DIO_Write1(deviceIndex, (2+offset), true);	//Set SCLK to '1'
				//Read SDO data back
				//
				
			}

			//Reset SCLK and /CS
			DIO_Write1(deviceIndex, (2+offset), true);	//Set SCLK to '1'	
			DIO_Write1(deviceIndex, (1+offset), true);	//Set /CS to '1'

//-------------------------------------------------------------------------------------
//  Store default data to NV memory
//-------------------------------------------------------------------------------------
		} else if (op_num == 5) {  //Store data option
			com_code = 0x00;

			// Get resistor number:
			//printf("Which resistor? (1-12)\n");
			//scanf("%d", &res_num);

			for ( res_num = 1; res_num < 13; ++res_num) {

				sleep(1000);

				// Set resistor register addresses and IO bank offsets.
				if (res_num == 3) {
					//addr = 0x06;
					addr = 0x08;
					offset = 0;  //Bank A
					//volt = 1.637;
					//volt = 1.9;
					res_code = 180;

				} else if (res_num == 2) {
					//addr = 0x07;
					addr = 0x09;
					offset = 0;  //Bank A
					//volt = 1.748;
					//volt = 1.9;
					res_code = 190;

				} else if (res_num == 1) {
					//addr = 0x01;
					addr = 0x03;
					offset = 0;  //Bank A
					//volt = 1.865;
					//volt = 1.9;
					res_code = 200;

				} else if (res_num == 4) {
					//addr = 0x00;
					addr = 0x02;
					offset = 0;  //Bank A
					//volt = 1.514;
					//volt = 1.9;
					res_code = 170;

				} else if (res_num == 7) {
					//addr = 0x06;
					addr = 0x08;
					offset = 8;  //Bank B
					//volt = 1.123;
					//volt = 1.5;
					res_code = 140;

				} else if (res_num == 6) {
					//addr = 0x07;
					addr = 0x09;
					offset = 8;  //Bank B
					//volt = 1.270;
					//volt = 1.5;
					res_code = 150;

				} else if (res_num == 5) {
					//addr = 0x01;
					addr = 0x03;
					offset = 8;  //Bank B
					//volt = 1.391;
					//volt = 1.5;
					res_code = 160;

				} else if (res_num == 8) {
					//addr = 0x00;
					addr = 0x02;
					offset = 8;  //Bank B
					//volt = 1.023;
					//volt = 1.5;
					res_code = 130;

				} else if (res_num == 11) {
					//addr = 0x06;
					addr = 0x08;
					offset = 16;  //Bank C
					//volt = .667;
					//volt = 1.2;
					res_code = 100;

				} else if (res_num == 10) {
					//addr = 0x07;
					addr = 0x09;
					offset = 16;  //Bank C
					//volt = .782;
					//volt = 1.3;
					res_code = 110;

				} else if (res_num == 9) {
					//addr = 0x01;
					addr = 0x03;
					offset = 16;  //Bank C
					//volt = .896;
					//volt = 1.4;
					res_code = 120;

				} else if (res_num == 12) {
					//addr = 0x00;
					addr = 0x02;
					offset = 16;  //Bank C
					//volt = .595;
					//volt = .9;
					res_code = 90;

				} else {
					printf("Invalid resistor number.  Quitting Program.\n");
					goto ErrorExit;

				}

				// Get the desired output voltage:
				//printf("Output voltage value? (0V<= Vo <=+2.5V)\n");
				//scanf("%f", &volt);

				//Calculate resistor tuning value from voltage:
				//res_code_fl = ceil(((volt/Vref)*256.)-.5);
			
				//res_code = (int)res_code_fl;

				printf("Resistance Code is: %d \n",res_code);

				// Set up the 16-bit command word
				com_word = 0x00000000;

				com_word = com_word | (com_code << 10);
				//printf("com_word is %d\n", com_word);

				com_word = com_word | (addr << 12);
				//printf("com_word is %d\n", com_word);

				com_word = com_word | (res_code << 0);
				//printf("com_word is %d\n", com_word);

				// Write serial data to the DIO ports:
				// Port assignments:  x0=HV_EN, x1=/CS, x2=SCLK, and x3=SDI
				// Here 'x' is {A,B, or C}.

				DIO_Write1(deviceIndex, (3+offset), false);	//Set SDI to '0'
				DIO_Write1(deviceIndex, (2+offset), true);	//Set SCLK to '1'	
				DIO_Write1(deviceIndex, (1+offset), true);	//Set /CS to '1'
				DIO_Write1(deviceIndex, (0+offset), true);	//Set HV_EN to '1'

				//Set /CS to '0' to begin write command cycle:
				DIO_Write1(deviceIndex, (1+offset), false);	
			

				for (j = 0; j < 16; j++)  { 

					//Set SCLK to '0'
					DIO_Write1(deviceIndex, (2+offset), false);	

					if (com_word & (1 << (15-j))) {
						//Set SDI to '1'
						DIO_Write1(deviceIndex, (3+offset), true);	

					} else {
						//Set SDI to '0'
						DIO_Write1(deviceIndex, (3+offset), false);	//Set SDI to '0'

					}

					//Set SCLK to '1'
					DIO_Write1(deviceIndex, (2+offset), true);	//Set SCLK to '1'
					//Read SDO data back
					//
				
				}

				//Reset SCLK and /CS
				DIO_Write1(deviceIndex, (2+offset), true);	//Set SCLK to '1'	
				DIO_Write1(deviceIndex, (1+offset), true);	//Set /CS to '1'

			}


//-------------------------------------------------------------------------------------
//  Adjust Potentiometer
//-------------------------------------------------------------------------------------
		} else if (op_num == 6) {  //adjust potentiometer setting option

			//Get resistor number
			printf("Which resistor? (1-12)\n");
			scanf("%d", &res_num);

			// Set resistor register addresses and IO bank offsets.
			if (res_num == 3) {
				addr = 0x06;
				offset = 0;  //Bank A

			} else if (res_num == 2) {
				addr = 0x07;
				offset = 0;  //Bank A

			} else if (res_num == 1) {
				addr = 0x01;
				offset = 0;  //Bank A

			} else if (res_num == 4) {
				addr = 0x00;
				offset = 0;  //Bank A

			} else if (res_num == 7) {
				addr = 0x06;
				offset = 8;  //Bank B

			} else if (res_num == 6) {
				addr = 0x07;
				offset = 8;  //Bank B

			} else if (res_num == 5) {
				addr = 0x01;
				offset = 8;  //Bank B

			} else if (res_num == 8) {
				addr = 0x00;
				offset = 8;  //Bank B

			} else if (res_num == 11) {
				addr = 0x06;
				offset = 16;  //Bank C

			} else if (res_num == 10) {
				addr = 0x07;
				offset = 16;  //Bank C

			} else if (res_num == 9) {
				addr = 0x01;
				offset = 16;  //Bank C

			} else if (res_num == 12) {
				addr = 0x00;
				offset = 16;  //Bank C

			} else {
				printf("Invalid resistor number.  Quitting Program.\n");
				goto ErrorExit;

			}

			//Potentiometer adjust task loop
			stop = 0;

			while (stop != 1) {

				//Get increment, decrement, or stop option
				printf("Increment wiper, decrement wiper, or stop adjusting? (1/2/0)\n");
				scanf("%d", &resp2);


				//Parse response
				
				if (resp2 == 1) {  //Increment the wiper setting

					com_code = 0x01;  //Code for increment operation

					// Set up the 8-bit command word
					com_word = 0x00000000;

					com_word = com_word | (com_code << 2);
					//printf("com_word is %d\n", com_word);

					com_word = com_word | (addr << 4);
					//printf("com_word is %d\n", com_word);

					// Write serial data to the DIO ports:
					// Port assignments:  x0=HV_EN, x1=/CS, x2=SCLK, and x3=SDI
					// Here 'x' is {A,B, or C}.

					DIO_Write1(deviceIndex, (3+offset), false);	//Set SDI to '0'
					DIO_Write1(deviceIndex, (2+offset), true);	//Set SCLK to '1'	
					DIO_Write1(deviceIndex, (1+offset), true);	//Set /CS to '1'
					DIO_Write1(deviceIndex, (0+offset), true);	//Set HV_EN to '1'

					//Set /CS to '0' to begin write command cycle:
					DIO_Write1(deviceIndex, (1+offset), false);	
			

					for (j = 0; j < 8; j++)  { 

						//Set SCLK to '0'
						DIO_Write1(deviceIndex, (2+offset), false);	

						if (com_word & (1 << (7-j))) {
							//Set SDI to '1'
							DIO_Write1(deviceIndex, (3+offset), true);	

						} else {
							//Set SDI to '0'
							DIO_Write1(deviceIndex, (3+offset), false);	//Set SDI to '0'

						}

						//Set SCLK to '1'
						DIO_Write1(deviceIndex, (2+offset), true);	//Set SCLK to '1'
						//Read SDO data back
						//
				
					}

					//Reset SCLK and /CS
					DIO_Write1(deviceIndex, (2+offset), true);	//Set SCLK to '1'	
					DIO_Write1(deviceIndex, (1+offset), true);	//Set /CS to '1'



				} else if (resp2 == 2) {  //Decrement the wiper setting
					
					com_code = 0x02;  //Code for decrement operation

					// Set up the 8-bit command word
					com_word = 0x00000000;

					com_word = com_word | (com_code << 2);
					//printf("com_word is %d\n", com_word);

					com_word = com_word | (addr << 4);
					//printf("com_word is %d\n", com_word);

					// Write serial data to the DIO ports:
					// Port assignments:  x0=HV_EN, x1=/CS, x2=SCLK, and x3=SDI
					// Here 'x' is {A,B, or C}.

					DIO_Write1(deviceIndex, (3+offset), false);	//Set SDI to '0'
					DIO_Write1(deviceIndex, (2+offset), true);	//Set SCLK to '1'	
					DIO_Write1(deviceIndex, (1+offset), true);	//Set /CS to '1'
					DIO_Write1(deviceIndex, (0+offset), true);	//Set HV_EN to '1'

					//Set /CS to '0' to begin write command cycle:
					DIO_Write1(deviceIndex, (1+offset), false);	
			

					for (j = 0; j < 8; j++)  { 

						//Set SCLK to '0'
						DIO_Write1(deviceIndex, (2+offset), false);	

						if (com_word & (1 << (7-j))) {
							//Set SDI to '1'
							DIO_Write1(deviceIndex, (3+offset), true);	

						} else {
							//Set SDI to '0'
							DIO_Write1(deviceIndex, (3+offset), false);	//Set SDI to '0'

						}

						//Set SCLK to '1'
						DIO_Write1(deviceIndex, (2+offset), true);	//Set SCLK to '1'
						//Read SDO data back
						//
				
					}

					//Reset SCLK and /CS
					DIO_Write1(deviceIndex, (2+offset), true);	//Set SCLK to '1'	
					DIO_Write1(deviceIndex, (1+offset), true);	//Set /CS to '1'


				} else {  //Stop the task loop

					printf("Stopping potentiometer adjustment...\n");
					stop = 1;

				}

			}
			
			

			//Save the wiper setting to NV memory?
			printf("Save wiper setting to non-volatile memory? (1/0)\n");
			scanf("%d", &resp1);

			//Respond to user input:
			if (resp1 == 1) {  //Save wiper setting

				printf("Saving setting to memory...\n");

				//-----------------------------------------------------
				//Reading current wiper setting
				//-----------------------------------------------------
				com_code = 0x03;

				// Set register addresses and IO bank offsets.
				offset1 = 3;  //Bank D

				//Set reg_num for reading wiper value
				reg_num = res_num;  

				// Set resistor register addresses and IO bank offsets.
				if (reg_num == 3) {
					addr = 0x06;
					offset = 0;  //Bank A
					offset2 = 0;

				} else if (reg_num == 2) {
					addr = 0x07;
					offset = 0;  //Bank A
					offset2 = 0;

				} else if (reg_num == 1) {
					addr = 0x01;
					offset = 0;  //Bank A
					offset2 = 0;

				} else if (reg_num == 4) {
					addr = 0x00;
					offset = 0;  //Bank A
					offset2 = 0;

				} else if (reg_num == 7) {
					addr = 0x06;
					offset = 8;  //Bank B
					offset2 = 1;

				} else if (reg_num == 6) {
					addr = 0x07;
					offset = 8;  //Bank B
					offset2 = 1;

				} else if (reg_num == 5) {
					addr = 0x01;
					offset = 8;  //Bank B
					offset2 = 1;

				} else if (reg_num == 8) {
					addr = 0x00;
					offset = 8;  //Bank B
					offset2 = 1;

				} else if (reg_num == 11) {
					addr = 0x06;
					offset = 16;  //Bank C
					offset2 = 2;

				} else if (reg_num == 10) {
					addr = 0x07;
					offset = 16;  //Bank C
					offset2 = 2;

				} else if (reg_num == 9) {
					addr = 0x01;
					offset = 16;  //Bank C
					offset2 = 2;

				} else if (reg_num == 12) {
					addr = 0x00;
					offset = 16;  //Bank C
					offset2 = 2;

				}

				// Set up the 16-bit command word
				com_word = 0x00000000;

				com_word = com_word | (com_code << 10);
				//printf("com_word is %d\n", com_word);

				com_word = com_word | (addr << 12);
				//printf("com_word is %d\n", com_word);

				// Write serial data to the DIO ports:
				// Port assignments:  x0=HV_EN, x1=/CS, x2=SCLK, and x3=SDI
				// Here 'x' is {A,B, or C}.

				DIO_Write1(deviceIndex, (3+offset), false);	//Set SDI to '0'
				DIO_Write1(deviceIndex, (2+offset), true);	//Set SCLK to '1'	
				DIO_Write1(deviceIndex, (1+offset), true);	//Set /CS to '1'
				DIO_Write1(deviceIndex, (0+offset), true);	//Set HV_EN to '1'

				//Set /CS to '0' to begin write command cycle:
				DIO_Write1(deviceIndex, (1+offset), false);	
			

				for (j = 0; j < 16; j++)  { 

					//Set SCLK to '0'
					DIO_Write1(deviceIndex, (2+offset), false);	

					if (com_word & (1 << (15-j))) {
						//Set SDI to '1'
						DIO_Write1(deviceIndex, (3+offset), true);	

					} else {
						//Set SDI to '0'
						DIO_Write1(deviceIndex, (3+offset), false);	//Set SDI to '0'

					}

					//Set SCLK to '1'
					DIO_Write1(deviceIndex, (2+offset), true);	//Set SCLK to '1'

					//Read SDO data back
					DIO_Read8(deviceIndex, offset1, &(buffer[15-j]));

				
				}

				//Reset SCLK and /CS
				DIO_Write1(deviceIndex, (2+offset), true);	//Set SCLK to '1'	
				DIO_Write1(deviceIndex, (1+offset), true);	//Set /CS to '1'

				//Process Data Read back:

				if (buffer[9] & (1 << offset2)) {
					//if CMDERR bit=1, then process data

					temp = 0;
					x = 0.5;	// Initialize variable to count powers of 2.
			
					for (j = 0; j < 9; j++)  {
						x *= 2;  // Calculate jth power of 2 (i.e., x = 2^j)
					
						//printf("Buffer value is: 0x%02lX for index: %d\n\r",buffer[j],j);

						if (buffer[j] & (1 << offset2)) {

							temp += (int)x;

						}
					}

					printf("Data value is: %d\n",temp);

				} else {
					//if CMDERR bit=0, print error message
					printf("CMDERR detected.\n");

				}

				//-----------------------------------------------------
				//Saving wiper setting to NV memory
				//-----------------------------------------------------
				com_code = 0x00;

				// Set resistor register addresses and IO bank offsets.
				if (res_num == 3) {
					//addr = 0x06;
					addr = 0x08;
					offset = 0;  //Bank A

				} else if (res_num == 2) {
					//addr = 0x07;
					addr = 0x09;
					offset = 0;  //Bank A

				} else if (res_num == 1) {
					//addr = 0x01;
					addr = 0x03;
					offset = 0;  //Bank A

				} else if (res_num == 4) {
					//addr = 0x00;
					addr = 0x02;
					offset = 0;  //Bank A

				} else if (res_num == 7) {
					//addr = 0x06;
					addr = 0x08;
					offset = 8;  //Bank B

				} else if (res_num == 6) {
					//addr = 0x07;
					addr = 0x09;
					offset = 8;  //Bank B

				} else if (res_num == 5) {
					//addr = 0x01;
					addr = 0x03;
					offset = 8;  //Bank B

				} else if (res_num == 8) {
					//addr = 0x00;
					addr = 0x02;
					offset = 8;  //Bank B

				} else if (res_num == 11) {
					//addr = 0x06;
					addr = 0x08;
					offset = 16;  //Bank C

				} else if (res_num == 10) {
					//addr = 0x07;
					addr = 0x09;
					offset = 16;  //Bank C

				} else if (res_num == 9) {
					//addr = 0x01;
					addr = 0x03;
					offset = 16;  //Bank C

				} else if (res_num == 12) {
					//addr = 0x00;
					addr = 0x02;
					offset = 16;  //Bank C

				}

				// Set up the 16-bit command word
				com_word = 0x00000000;

				com_word = com_word | (com_code << 10);
				//printf("com_word is %d\n", com_word);

				com_word = com_word | (addr << 12);
				//printf("com_word is %d\n", com_word);

				com_word = com_word | (temp << 0);
				//printf("com_word is %d\n", com_word);

				// Write serial data to the DIO ports:
				// Port assignments:  x0=HV_EN, x1=/CS, x2=SCLK, and x3=SDI
				// Here 'x' is {A,B, or C}.

				DIO_Write1(deviceIndex, (3+offset), false);	//Set SDI to '0'
				DIO_Write1(deviceIndex, (2+offset), true);	//Set SCLK to '1'	
				DIO_Write1(deviceIndex, (1+offset), true);	//Set /CS to '1'
				DIO_Write1(deviceIndex, (0+offset), true);	//Set HV_EN to '1'

				//Set /CS to '0' to begin write command cycle:
				DIO_Write1(deviceIndex, (1+offset), false);	
			

				for (j = 0; j < 16; j++)  { 

					//Set SCLK to '0'
					DIO_Write1(deviceIndex, (2+offset), false);	

					if (com_word & (1 << (15-j))) {
						//Set SDI to '1'
						DIO_Write1(deviceIndex, (3+offset), true);	

					} else {
						//Set SDI to '0'
						DIO_Write1(deviceIndex, (3+offset), false);	//Set SDI to '0'

					}

					//Set SCLK to '1'
					DIO_Write1(deviceIndex, (2+offset), true);	//Set SCLK to '1'
					//Read SDO data back
					//
				
				}

				//Reset SCLK and /CS
				DIO_Write1(deviceIndex, (2+offset), true);	//Set SCLK to '1'	
				DIO_Write1(deviceIndex, (1+offset), true);	//Set /CS to '1'

			} else {  //Do not save setting

				printf("Not saving...\n");

			}


		}

		//Prompt the user to see if they wish to continue:
		printf("\nPerform another task or quit? (1/0)\n");

		scanf("%d", &proceed);


	}


ErrorExit:


	return 0;
}

