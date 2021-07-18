
#include <stdio.h>
#include <cmsis_os2.h>
#include <stdio.h> 
#include <stdbool.h>
//#include <lpc17xx.h>

//Lab 3 Threading Code
//Stephanie Skarica and Elizabeth Drew
//July 5th 2020
/*
void joystick(void *arg)
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
		
		//Allow for multi-threading with delay of 10 ticks
		osDelay(10); 
	}
}

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

    float analog; 
    int adc; 

    //Polling loop
    while(true)
    {
			//Set bit 24 to start the conversion
			LPC_ADC->ADCR |= (0b1u << 24); 

			//Wait while the conversion is not done
			while(!(LPC_ADC->ADGDR & (0b1u << 31))){}

			//Read in the adc result 
			adc = (0b111111111111u << 4 & (LPC_ADC->ADGDR)) >> 4;

			//Divide by 1240.0 to get the voltage value 
			analog = adc / 1240.0; 

			//Print the value of the output 
			printf("Output: %.2f\n", analog); 

			//Thread Yield
			osThreadYield(); 
	}
}

void button(void *arg)
{
	bool buttonPushed, ledOn, toggle; 
	
	//infinite polling loop
	while(true)
	{
		
		//read the state of the button 
		buttonPushed = !((LPC_GPIO2->FIOPIN & (0b1u << 10)) >> 10);
		//set toggle default to false
		toggle = false; 
		//read the value of the LED 
		ledOn = ((LPC_GPIO2->FIOPIN & (0b1 << 6)) >> 6); 
		
		//if the button is being pushed
		while(buttonPushed)
		{
			//check for the button state 
			buttonPushed = !((LPC_GPIO2->FIOPIN & (0b1u << 10)) >> 10);
			//if the button is released, set toggle to true
 			if(!buttonPushed)
				toggle = true; 
		}
		if(ledOn && toggle) 
			LPC_GPIO2->FIOCLR |= (0b1u << 6); 
		else if(!ledOn && toggle) 	
			LPC_GPIO2->FIOSET |= (0b1u << 6); 
		//Allow for multi-threading with delay of 10 ticks
		osDelay(10);
		
	}
}

int main() 
{
	
	
	//Set LED pins to output
	LPC_GPIO1 -> FIODIR |= (0b1011u << 28); 
	LPC_GPIO2 -> FIODIR |= (0b11111 << 2);
	
	//Turn off all LED's (default are all on)
	LPC_GPIO1 -> FIOCLR |= (0b1011u << 28); 
	LPC_GPIO2 -> FIOCLR |= (0b11111 << 2);
	
	//Initialize Kernal
	osKernelInitialize();
	//Joystick Reading 
	osThreadNew(joystick, NULL, NULL); 
	//ADC Reading 
	osThreadNew(adc, NULL, NULL); 
	//Pushbutton Pressed
  osThreadNew(button, NULL, NULL); 
	//Start the Kernal
	osKernelStart();
	
}
*/
  
const osMutexAttr_t Thread_Mutex_attr = {
  "myThreadMutex",                              // human readable mutex name
  osMutexRecursive | osMutexPrioInherit,        // attr_bits
  NULL,                                         // memory for control block (default)
  0U                                            // size for control block (default)
};
  
 int main() {
  osMutexId_t mutex_id = osMutexNew(&Thread_Mutex_attr); 
	printf("%d", mutex_id.cb_size);
  //osMutexId_t mutex_id2 = osMutexNew(&Thread_Mutex_attr);   // use attributes from defined structure
}


