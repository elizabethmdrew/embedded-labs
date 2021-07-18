#include <stdio.h>
#include <cmsis_os2.h>
#include <stdio.h> 
#include <stdbool.h>
#include <lpc17xx.h>

void joystick(void *arg)
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
		//Initialize directions 
		bool north, east, south, west, pushed; 
		
		//Read the corresponding GPIO pin for status
		north = !(LPC_GPIO1->FIOPIN &= (0b1 << 23));
		east = !(LPC_GPIO1->FIOPIN &= (0b1 << 24));
		south = !(LPC_GPIO1->FIOPIN &= (0b1 << 25));
		west = !(LPC_GPIO1->FIOPIN &= (0b1 << 26));

		//Read the GPIO Pushed bit  
		pushed = !(LPC_GPIO1->FIOPIN &= (0b1 << 20)); 
		
		//Print the status of the joystick press
		if(pushed) 
			LPC_GPIO2 -> FIOSET |= (0b1 << 3); 
		if(!pushed)
			LPC_GPIO2 -> FIOCLR |= (0b1 << 3); 

		//Print the corresponding direction
		if(north)
			LPC_GPIO1 -> FIOSET |= (0b0001u << 28); 
		if(east)
			 LPC_GPIO1 -> FIOSET |= (0b0010u << 28); 
		if(south)
			LPC_GPIO1 -> FIOSET |= (0b0011u << 28); 
		if(west)
			LPC_GPIO1 -> FIOSET |= (0b1000u << 28); 
		if(!north && !east && !south && !west)
			LPC_GPIO2 -> FIOCLR |= (0b1u << 4); 
		
		osDelay(10); 
	}
}

float analog; 
int adc1; 

void adc(void *arg)
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

    

    //Polling loop
    while(true)
    {
			//Set bit 24 to start the conversion
			LPC_ADC->ADCR |= (0b1u << 24); 

			//Wait while the conversion is not done
			while(!(LPC_ADC->ADGDR & (0b1u << 31))){printf("112\n"); }

			//Read in the adc result 
			adc1 = (0b111111111111u << 4 & (LPC_ADC->ADGDR)) >> 4;

			//Divide by 1240.0 to get the voltage value 
			analog = adc1 / 1240.0; 

			//Print the value of the output 
			printf("Output: %.2f,\n", analog); 

			osDelay(10);
	}
}
void button(void *arg)
{
	//Polling loop
	while(true)
	{
		if(!(LPC_GPIO2 -> FIOPIN & (1 << 6)))
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
		osThreadYield(); 
	}
}

int main() 
{
	
	osKernelInitialize();
	
	//Joystick Reading 
	//osThreadNew(joystick, NULL, NULL); 
	//ADC Reading 
	osThreadNew(adc, NULL, NULL); 
	//Pushbutton Pressed
//	osThreadNew(button, "Pushbutton", NULL); 
	
	osKernelStart();
}
