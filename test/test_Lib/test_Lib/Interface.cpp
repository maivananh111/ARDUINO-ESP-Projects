/*
 * Interface.cpp
 *
 * Created: 22/07/2021 2:24:32 CH
 *  Author: A315-56
 */ 
#define F_CPU 16000000UL
#include "Interface.h"



uint8_t UART_buffer[BUF_SIZE] = {0};
uint8_t SPI_buffer = 0;

/*UART*/
 UART::UART(){}
 void UART::Init(uint32_t baudrate){
	uint16_t baud = 0;
	baud = F_CPU/(16*baudrate) - 1;

	UBRR0H = (baud>>8);
	UBRR0L = (baud&0xff);
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	UCSR0B |= (1<<TXEN0)|(1<<RXEN0)|(1<<TXCIE0)|(1<<RXCIE0);
	UCSR0A |=  0;
}
void UART::Transmit(uint8_t data){
	while(bit_is_clear(UCSR0A,UDRE0)){}
	UDR0 = data;
}
void UART::Write(char *str){
  //unsigned char type = *str;
  while(*str){
	  Transmit(*str++);
  }
}
uint8_t UART::Read(void){ 
	static uint16_t rx_read_pos = 0;
	uint8_t data = 0;
	data = UART_buffer[rx_read_pos];
	rx_read_pos++;
	if(rx_read_pos >= BUF_SIZE){
		rx_read_pos = 0;
	}
	return data;
}
ISR(USART_RX_vect){ 
	volatile static uint16_t rx_pos = 0;
	UART_buffer[rx_pos] = UDR0;
	rx_pos++;
	if(rx_pos >= BUF_SIZE){
		rx_pos = 0;
	}
}
/*End*/
/*SPI*/
SPI::SPI(){}
void SPI::Init(uint8_t mode, bool x2speed){
	if (mode == SPI_Master){
		DDRB |= (1<<PB5)|(1<<PB3);
		PORTB |= (1<<PB4);
		SPCR = (1<<SPIE)|(1<<SPE)|(1<<MSTR);
		if (x2speed) SPCR |= ((1<<SPR0)|(1<<SPR1));
		else SPCR |= (1<<SPR1);
	}
	else if(mode == SPI_Slave){
		DDRB |= (1<<PB4);
		PORTB |= (1<<PB2)|(1<<PB3);
		SPCR = (1<<SPIE)|(1<<SPE);
		if (x2speed) SPCR |= ((1<<SPR0)|(1<<SPR1));
		else SPCR |= (1<<SPR1);
	}
}
void SPI::Transmit(uint8_t data){
	SPDR = data;
	while(bit_is_clear(SPSR, SPIF));
}
uint8_t SPI::Read(void){
	return SPI_buffer;
}
ISR(SPI_STC_vect){
	SPI_buffer = SPDR;
}
/*End*/
/*I2C*/
I2C::I2C(uint32_t sp){
   speed = sp;
}
void I2C::Init(void){
uint8_t val;
   val = ((F_CPU/speed)-16)/2;
	TWSR = 0; 
	TWBR = val;
	TWCR = (1<<TWEN);
}
void I2C::Start(uint8_t Addr){
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); 
	while ((TWCR & (1<<TWINT)) == 0);
	if((TWSR&0xF8) != 0x08) { 
		Init();
		return;
	}

	TWDR = Addr; 
	TWCR = (1<<TWINT)|(1<<TWEN); 
	while ((TWCR & (1<<TWINT)) == 0);  
	if((TWSR&0xF8) != 0x18) { 
		Init();
		return;
	}
}
void I2C::Transmit(uint8_t data){
	TWDR = data; 
	TWCR = (1<<TWINT)|(1<<TWEN); 
	while ((TWCR & (1<<TWINT)) == 0);  
	if((TWSR&0xF8) != 0x28) { 
		Init();
		return;
	}
}
void I2C::Stop(void){
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
    Init();
}
/*End*/