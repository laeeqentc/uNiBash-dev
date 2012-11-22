/****************************************************************************************
FileName		:	Serial.h
ProjectName		:	UART TEST 
Target			:	ATmega128
CPU Clock		:	16Mhz
Author			:	TEAM
Description		:	This is a header file for using UART1 of ATmega128.
****************************************************************************************/

#ifndef Serial_H
	#include<util/delay.h>
	#define Serial_H
	#define FOSC 16000000	/* CPU Clock Frequency */
	#define BAUD 9600		/* Serial transfer rate (Baudrate) */
	#define MYUBRR FOSC/16/BAUD-1

	void USART_Init( unsigned int ubrr );
	void Serial_Init(void);
	void USART_Transmit( unsigned char data );
	unsigned char USART_Receive( void );
	unsigned char * UART_Receive_string(void);
	unsigned char * UART_Receive_string_pass(void);
	void print_string(unsigned char *Mystr_ptr);

	void Serial_Init(void)
	{
		USART_Init ( MYUBRR );	/* SET THE BAUD RATE, ENABLE THE TRANSMISSION AND RECEPTION OF SERIAL, SET THE FRAME FORMAT */
	}

	void USART_Init( unsigned int ubrr )
	{
	  UBRR1H = (unsigned char)(ubrr>>8); /* Set baud rate */
	  UBRR1L = (unsigned char)ubrr;

	  UCSR1B = (1<<RXEN)|(1<<TXEN);	/* Enable receiver and transmitter */

	  UCSR1C = (1<<USBS)|(3<<UCSZ0);/* Set frame format: 8data, 2stop bit */
	}

	unsigned char USART_Receive( void )
	{
	  while (!(UCSR1A & (1<<RXC))); /* Wait for data to be received */

	  return UDR1;	/* Get and return received data from buffer */
	}

	void USART_Transmit( unsigned char data )
	{
	  while (!(UCSR1A & (1<<UDRE)));	/* Wait for empty transmit buffer */

	  UDR1 = data;	/* Put data into buffer, sends the data */
	}

	void print_string(unsigned char *Mystr_ptr)
	{
		while((*Mystr_ptr) != '\0')	/* print char from str pointer and point to next location */
				USART_Transmit((*Mystr_ptr++));
	}
/* function to receive a string usins UART*/
unsigned char * UART_Receive_string(void)
{
	unsigned char ch1,str[100],count=0;
	while((ch1=USART_Receive())!=13)
	{
		str[count]=ch1;
		_delay_ms(10);
		USART_Transmit(ch1);
		count++;
	}
	str[count]='\0';
	return str;
}
unsigned char * UART_Receive_string_pass(void)
{
	unsigned char ch1,str[100],count=0;
	while((ch1=USART_Receive())!=13)
	{
		str[count]=ch1;
		_delay_ms(10);
		USART_Transmit('*');
		count++;
	}
	str[count]='\0';
	return str;
}

void UART_Transmit_num(unsigned int num)
{
	unsigned char ch[3],count=2;	

	ch[0]=0;
	ch[1]=0;
	ch[2]=0;

	while(num!=0)
	{
		ch[count]=num%10;
		num=num/10;
		count--;
	}
	for(count=0;count<3;count++)
	{
		//UART_Transmit_char('0'+ch[count]);
		USART_Transmit('0'+ch[count]);
	}
}


#endif
