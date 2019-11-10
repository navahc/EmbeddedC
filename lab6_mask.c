// BranchingFunctionsDelays.c Lab 6 
// Runs on LM4F120/TM4C123 
// Use simple programming structures in C to
// toggle an LED while a button is pressed and
// turn the LED on when the button is released.
// This lab will use the hardware already built into the LaunchPad.

// Saurabh Chavan
// November 10, 2019 

// built-in connection: PF0 connected to negative logic momentary switch, SW2
// built-in connection: PF1 connected to red LED
// built-in connection: PF2 connected to blue LED 
// built-in connection: PF3 connected to green LED 
// built-in connection: PF4 connected to negative logic momentary switch, SW1   

#include "TExaS.h" 
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOF      0x00000020  // port F Clock Gating Control
  

// basic functions defined at end of startup.s 
void DisableInterrupts(void); // Disable interrupts 
void EnableInterrupts(void);  // Enable interrupts  

//My program function declarations 
void GPIO_init(void); 
void delay_func(void);   

int main(void){
    unsigned long volatile delay, sw1, sw_read, led_read, led2;
        TExaS_Init(SW_PIN_PF4, LED_PIN_PF2);  // activate grader and set system clock to 80 MHz 
	
  // initialization goes here 
    GPIO_init(); 
    EnableInterrupts();          // enable interrupts for the grader
    GPIO_PORTF_DATA_R = GPIO_PORTF_DATA_R | 0x04; // PF2=1 Makes the LED ON 
  while(1){ 
    // body goes here
		delay_func();         
		sw1=0x00;  
		sw_read = GPIO_PORTF_DATA_R & 0x10; 
		led_read= GPIO_PORTF_DATA_R & 0x04; 
		if (sw1==sw_read){ 
			delay_func(); 
			if (led_read==0x00){ 
				GPIO_PORTF_DATA_R = GPIO_PORTF_DATA_R | 0x04; 
			} 
			else{ 
				GPIO_PORTF_DATA_R = GPIO_PORTF_DATA_R & (~0x04); 
			} 
		} 
		else { 
			GPIO_PORTF_DATA_R = GPIO_PORTF_DATA_R | 0x04; 
		} 
	} 
}  

//My program function definitions 
void GPIO_init(){  
	volatile unsigned long delay; 
	SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock 
	delay = SYSCTL_RCGC2_R;           // delay 
	GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PF 
	GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0 
	GPIO_PORTF_DIR_R = 0x04;          // 5) PF4 in, PF2 out 
	GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0 
	GPIO_PORTF_PUR_R = 0x10;          // enable pull-up on PF4 
	GPIO_PORTF_DEN_R = 0x14;          // 7) enable digital I/O on PF4-0 
}   

void delay_func(){
	int i = 1333333;  // this number means 100ms 
	while(i > 0){ 
		i = i - 1; 
	} 
}

