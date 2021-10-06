/*
 * Interface.h
 *
 * Created: 22/07/2021 2:24:14 CH
 *  Author: A315-56
 */ 


#ifndef INTERFACE_H_
#define INTERFACE_H_

#include <avr/io.h>
#include <avr/interrupt.h>


#define BUF_SIZE 128
#define SPI_Master 1
#define SPI_Slave  0


/*UART*/
class UART {
	public:
        UART();
		void Init(uint32_t baudrate);
		void Transmit(uint8_t data);
        void Write(char *str);
		uint8_t Read(void);
	private:
       //ISR(USART_RX_vect);
       
};
/*end*/
/*SPI*/
class SPI {
	public:
        SPI();
		void Init(uint8_t mode, bool x2speed);
		void Transmit(uint8_t data);
		uint8_t Read(void);
	private:
        //ISR(SPI_STC_vect);
        
};
/*End*/
/*I2C*/
class I2C {
	public:
      I2C(uint32_t sp);
      void Init(void);
      void Start(uint8_t Addr);
      void Stop(void);
      void Transmit(uint8_t data);
	private:
      uint32_t speed;
};
/*End*/


#endif /* INTERFACE_H_ */