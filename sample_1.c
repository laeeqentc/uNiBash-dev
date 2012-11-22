#include "includes.h"
#include"Serial.h"
#include <stdio.h>
#include<string.h>
#include"I2C_RTC_atm128.h"
#if defined(__AVR_AT90S2313__) || defined(__AVR_AT90S2333__) || defined(__AVR_AT90S4433__) || \
        defined(__AVR_AT90S4434__) || defined(__AVR_AT90S8535__) || defined(___AVR_ATmega8535__)
# error "Your MCU probably does not have enough SRAM to run this program"
#elif defined(__AVR_AT90S4414__) 
# error "Your MCU probably does not have enough program memory to run this program"
#elif defined(__AVR_AT90S8515__) || \
       defined(__AVR_ATmega161__) || defined(__AVR_ATmega162__) || defined(__AVR_ATmega163__) || \
       defined(___AVR_ATmega8515__) || defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || \
       defined(__AVR_ATmega8__)
#   define TCCR0VAL (_BV(CS02) | _BV(CS00))
#elif defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__) || defined(__AVR_ATmega103__)
#   define TCCR0VAL (_BV(CS02) | _BV(CS01) | _BV(CS00))
#else
#  error "Don't know what kind of MCU you are compiling for"
#endif


/***************************************************************************************************************
*                                               CONSTANTS
**************************************************************************************************************/
#define TASK_STACK 512
void Task1( void *Task1Data ); 
void Task2( void *Task2Data ); 

/*************************************************************************************************************
                                            GLOBAL VARIABLES
**************************************************************************************************************/

OS_STK Task1Stack[ TASK_STACK ];
OS_STK Task2Stack[ TASK_STACK ];

extern OS_EVENT  *pSem; //This semaphore has been declared in VT102lib.h to guard VT library since UART is a shared resource

/************************************************************************************************************/
char user1[8]="laeeq";
char pass1[8]="laeeq123";
char user2[8]="user";
char pass2[8]="user123";


int main(void) 
{
	char *utemp1, *ptemp1;
	unsigned char ret1;	
	Serial_Init(); 
	print_string("\t Welcome to uNiBash enviroment");  	
	print_string("\r Username:");
	utemp1=UART_Receive_string();
	ret1=strcmp(user1,utemp1);
	print_string("\r Password:");
	ptemp1=UART_Receive_string_pass();
	if(ret1!=0 && strcmp(user2,utemp1)!=0 && strcmp(pass1,ptemp1)!=0){
	print_string("\rInvalid username or password");
	return 0;
	}
	else
	
	OSInit(); //required for generating the idle task and statistic task
	//Creating tasks
	OSTaskCreate( Task1, (void *)0, &Task1Stack[ TASK_STACK - 1 ], 0 );
	OSTaskCreate( Task2, (void *)0, &Task2Stack[ TASK_STACK - 1 ], 1 );
	
	OSStart(); //Start multi-tasking

	return 0;
}

void Task1( void *Task1Data ) 
{
	char *temp;
	INT8U cc;		//int of size 8bits, just like char
	INT8U i, flag, a ;
  	char s[10];
	char s1[10];
	Task1Data=Task1Data;
        print_string("\rtask 1 ");
	OS_ENTER_CRITICAL();
  	TCCR0=TCCR0VAL;                               /* Set TIMER0 prescaler to CLK/1024            */
  	TIMSK=_BV(TOIE0);                         /* Enable TIMER0 overflow interrupt               */
  	TCNT0=256-(CPU_CLOCK_HZ/OS_TICKS_PER_SEC/1024); /* Set the counter initial value              */
  	OS_EXIT_CRITICAL();
	OSStatInit();            /* Initialize uC/OS-II's statistics                 */
	
// command list and checking it , should be exhaustive covering all important commands 
	char masterlist[6][10] = {"time","date","who","whoami","stat","help"};
	
	static unsigned char SEC_var = 0;
	static unsigned char MIN_var = 0;
	static unsigned char HRS_var = 0;
	static unsigned char DAY_var = 0;
	static unsigned char MONTH_var = 0;
	static unsigned char YEAR_var = 0;

	char Debug_str[80];
	char time_str[15];

	Serial_Init();
	unsigned char tt;

	/**************************************
	here initialization required for RTC
	**************************************/
	Initialize_RTC();	/*  this is the Intialization for RTC and writes the */

			/* this is required to getting the date and time from funcn given below*/ 
		
	for(;;) 
	{
		Update_RTC_variables();
		YEAR_var = RTC_getYear();
		MONTH_var = RTC_getMonth();
		DAY_var = RTC_getDay();
		HRS_var = RTC_getHour();
		MIN_var = RTC_getMinute();
		SEC_var = RTC_getSecond();
		print_string("\rlaeeq@uNiBash:~$");
		temp=UART_Receive_string();
		strcpy(s1,temp);
		
		//checking syntax and all
		
		if(strcmp("time",s1)==0|strcmp("TIME",s1)==0){
			print_string("\r");		
		sprintf(time_str,"HRS = %d MIN = %02d SEC = %03d",HRS_var,MIN_var,SEC_var);
		print_string(time_str);
					//print_string("\rtime is ");
				}
				if(strcmp("who",s1)==0|strcmp("whoami",s1)==0|strcmp("WHOAMI",s1)==0)
					{
					print_string("\rlaeeq \t tty1 \t time \rlaeeq \t ps1 \t time ");
					}
				if(strcmp("stat",s1)==0|strcmp("STAT",s1)==0){
					print_string("\rCPU:");
					UART_Transmit_num(OSCPUUsage);
					print_string("%");   
					}	
	// use switch , add more api's 	
	OSTimeDlyHMSM(0, 0, 1,0); /* calls scheduler and makes the task wait for 1 second */     
	}// for (;;) ends hee 
}

void Task2( void *Task2Data ) 
{
    Task2Data = Task2Data;

	for(;;) 
	{
	    print_string("\rtask 2 ");
	OSTimeDlyHMSM(0, 0, 1,0); /* calls scheduler and makes the task wait for 1 second */	
	}
}
