#include <lpc17xx.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <cmsis_os2.h>
//Code by Stephanie Skarica and Elizabeth Drew for MTE 241

#define runMain 6

#if runMain == 1
int main (void) 
{
	//Set LED pins to output
	LPC_GPIO1 -> FIODIR |= (0b1011u << 28); 
	LPC_GPIO2 -> FIODIR |= (0b11111 << 2);
	
	//Turn off all LED's (default are all on)
	LPC_GPIO1 -> FIOCLR |= (0b1011u << 28); 
	LPC_GPIO2 -> FIOCLR |= (0b11111 << 2);
	 
	//Polling loop
	while(true)
	{
		if(!(LPC_GPIO2 -> FIOPIN & (1 << 10)))
		{
			//Set LED to be on if button pressed
			LPC_GPIO2 -> FIOSET |= (0b1u << 6);		
			printf("button pressed\n"); 
		}
		else
		{
			//Turn LED off again if button not pressed
			LPC_GPIO2 -> FIOCLR |= (0b1u << 6); 
		}
	}
}

#elif runMain == 2
int main()
{
	//Polling loop
	while(true)
	{
		//Initialize directions 
		bool north, east, south, west, pushed; 
		
		//Read the corresponding GPIO pin for status
		north = !(LPC_GPIO1->FIOPIN &= (0b1 << 23));
		east = !(LPC_GPIO1->FIOPIN &= (0b1 << 24));
		south = !(LPC_GPIO1->FIOPIN &= (0b1 << 25));
		west = !(LPC_GPIO1->FIOPIN &= (0b1 << 26));

		//Read the GPIO Pushed bit  
		pushed = !(LPC_GPIO1->FIOPIN &= (0b1 << 20)); 

		//Print the corresponding direction
		if(north)
			printf("North\n"); 
		if(east)
			printf("East\n"); 
		if(south)
			printf("South\n"); 
		if(west)
			printf("West\n"); 
		if(!north && !east && !south && !west)
			printf("Centered\n");
		
		//Print the status of the joystick press
		if(pushed) 
			printf("Pushed\n"); 
		else
			printf("Not Pushed\n"); 	
	}
}

#elif runMain == 3
//Decimal to binary conversion function
void dec_to_binary(int *binary, int decimal)
{
	int remainder = 0;
	int i = 7;
	
	while (decimal != 0)
	{
		remainder = decimal % 2; 
		binary[i] = remainder; 
		decimal = decimal /2;
		i--; 
	} 
}
//Helper function to get size of character array
int getSize (char a[])
{
	int size = 0; 
	for (int i = 0; a[i]!= '\0'; i++)
		size++;
	return size;
}
	
int main () 
{
	//create char array to store string input from serial port
	//maximum input is 255
	char num [4]; 

	//Set LED pins to output
	LPC_GPIO1 -> FIODIR |= (0b1011u << 28); 
	LPC_GPIO2 -> FIODIR |= (0b11111 << 2);

	//Turn off all LED's (default are all on)
	LPC_GPIO1 -> FIOCLR |= (0b1011u << 28);
	LPC_GPIO2 -> FIOCLR |= (0b11111u << 2);

		
	while (true)
	{	
		printf("\nplease input a number between 0 and 255 :)\n"); 
		//this function will read serial port input until 3 characters are inputted
		//Or if ctrl+j is inputted and store input in char array
		fgets(num, 4, stdin); 
		//Print string to check input
		printf("\ninput: %s\n", num); 
		
		//Define bool to check if character has been inputted
		bool character = false;
		
		//Define size of input char array
		int size = getSize(num);
		
		//Check char array from input for any characters
		for(int i = 0; i < size-1; i++)
		{
			if (num[i] < '0' || num[i] > '9')
					//If character present in array, set bool to true
					character = true;    
		}
		
		//Convert char to decimal
		int decimal = atoi(num);  
		printf("integer: %d\n", decimal); 
		
		//Only execute turning on LED's if number is valid
		if (decimal >= 0 && decimal < 256 && !character)
		{
		
			//Convert decimal to binary number and put in array
			int binary[] = {0,0,0,0,0,0,0,0}; 
			dec_to_binary(binary, decimal);
		
			//Print binary number 
			printf("binary:"); 
			for (int i =0; i < 8; i++)
				printf("%d", binary[i]);
		
			//Clear LED's from previous setting
			LPC_GPIO1 -> FIOCLR |= (0b1011u << 28);
			LPC_GPIO2 -> FIOCLR |= (0b11111u << 2);
		
			//Turn on LED's using binary number array
			LPC_GPIO2 -> FIOSET |= (binary[0] << 6); //left most LED
			LPC_GPIO2 -> FIOSET |= (binary[1] << 5);
			LPC_GPIO2 -> FIOSET |= (binary[2] << 4);
			LPC_GPIO2 -> FIOSET |= (binary[3] << 3);
			LPC_GPIO2 -> FIOSET |= (binary[4] << 2);
			LPC_GPIO1 -> FIOSET |= (binary[5] << 31);
			LPC_GPIO1 -> FIOSET |= (binary[6] << 29);
			LPC_GPIO1 -> FIOSET |= (binary[7] << 28); //right most LED
		}
		else 
			printf("incorrect entry"); 
	}
}
#elif runMain == 4
int main ()
{
	//Power on the ADC, turn on bit 12
	LPC_SC->PCONP |= (0b1u << 12); 	
	
	//Set the correct input
	//Clear bits 18 and 19 
	LPC_PINCON->PINSEL1 &= ~(0b11u << 18); 
	//Set bits
	LPC_PINCON->PINSEL1 |= (0b01u << 18); 
	
	//Selecting AD2
	LPC_ADC->ADCR |= (0b1u << 2);
	LPC_ADC->ADCR &= ~(0b1u << 0);
	
	//Set the correct sampling rate 
	LPC_ADC->ADCR |= (0b100u << 8); 
	
	//Enable ADCR Circuitry
	LPC_ADC->ADCR |= (0b1u << 21);
	
	float analog; 
	int adc; 
	
	//Polling loop
	while(true)
	{
		//Set bit 24 to start the conversion
		LPC_ADC->ADCR |= (0b1u << 24); 
		
		//Wait while the conversion is not done
		while(!(LPC_ADC->ADGDR | (0u << 31))){}
		
		//Read in the adc result 
		adc = (0b111111111111u << 4 & (LPC_ADC->ADGDR)) >> 4;
			
		//Divide by 1240.0 to get the voltage value 
		analog = adc / 1240.0; 
			
		//Print the value of the output 
		printf("Output: %.2f,\n", analog); 
	}	
}
#elif runMain == 6
#include <stdio.h>

#include <rtx_os.h>
#include <stdio.h> 
#include <stdbool.h>
#include <lpc17xx.h>


/* #define BUTTON_FLAG 0x00000001UL //A
osEventFlagsId_t eventFlags; */

//A
osMessageQueueId_t queue; 
int msg = 0x00000001UL; 

void setupPushButton(void)
{
	LPC_GPIOINT->IO2IntEnF|=(1<<10);
	NVIC_EnableIRQ(EINT3_IRQn);
}

void ENINT_IRQHandler(void)
{
	LPC_GPIOINT -> IO2IntClr|= (1<<10); 
	//B
	osMessageQueuePut(queue,&msg,NULL,NULL);
}

void t1(void *arg)
{
	while(true)
	{
		//C
		osMessageQueueGet(queue, &msg, NULL, osWaitForever); 
		printf("button pushed\n");
	}
}
int main (void)
{
	osKernelInitialize(); 
	//D
	queue = osMessageQueueNew(1, sizeof(msg), NULL); 
	setupPushButton(); 
	osThreadNew(t1, NULL, NULL); 
	osKernelStart(); 
	for(;;);
}




#endif



