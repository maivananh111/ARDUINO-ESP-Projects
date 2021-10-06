/*
 * TouchPad.cpp
 *
 * Created: 06/08/2021 4:12:41 CH
 *  Author: A315-56
 */ 
#define F_CPU 16000000UL

#include "TouchPad.h"
#include <util/delay.h>
#include <avr/io.h>
#include <stdint.h>

void bitWrite(uint8_t data, uint8_t num, uint8_t bit){
	data |= (bit<<num);
}
uint8_t bitRead(uint8_t stt, uint8_t i){
	return ((stt & (1<<i))>>i);
}
PS2Mouse::PS2Mouse(int clock_pin, int data_pin, int mode) {
	_clock_pin = clock_pin;
	_data_pin = data_pin;
	_mode = mode;
	_initialized = false;
	_disabled = true;
	_enabled = false;
}

void PS2Mouse::Init() {
	pull_high(_clock_pin);
	pull_high(_data_pin);
	_delay_ms(20);
	write(0xff); // Send Reset to the mouse
	read_byte();  // Read ack byte
	_delay_ms(20); // Not sure why this needs the delay
	read_byte();  // blank
	read_byte();  // blank
	_delay_ms(20); // Not sure why this needs the delay
	if (_mode == REMOTE) {
		set_remote_mode();
		} else {
		enable_data_reporting(); // Tell the mouse to start sending data again
	}
	_delay_us(100);
	_initialized = 1;
}

void PS2Mouse::set_mode(int data) {
	if (_mode == STREAM) {
		disable_data_reporting(); // Tell the mouse to stop sending data.
	}
	write(data);  // Send Set Mode
	read_byte();  // Read Ack byte
	if (_mode == STREAM) {
		enable_data_reporting(); // Tell the mouse to start sending data again
	}
	if (_initialized) {
		_delay_us(100);
	}
}

void PS2Mouse::set_remote_mode() {
	set_mode(0xf0);
	_mode = REMOTE;
}

void PS2Mouse::set_stream_mode() {
	set_mode(0xea);
	_mode = STREAM;
}

void PS2Mouse::set_sample_rate(int rate) {
	if (_mode == STREAM) {
		disable_data_reporting(); // Tell the mouse to stop sending data.
	}
	write(0xf3); // Tell the mouse we are going to set the sample rate.
	read_byte(); // Read Ack Byte
	write(rate); // Send Set Sample Rate
	read_byte(); // Read ack byte
	if (_mode == STREAM) {
		enable_data_reporting(); // Tell the mouse to start sending data again
	}
	_delay_us(100);
}

void PS2Mouse::set_scaling_2_1() {
	set_mode(0xe7); // Set the scaling to 2:1
}

void PS2Mouse::set_scaling_1_1() {
	set_mode(0xe6); // set the scaling to 1:1
}

// This only effects data reporting in Stream mode.
void PS2Mouse::enable_data_reporting() {
	if (!_enabled) {
		write(0xf4); // Send enable data reporting
		read_byte(); // Read Ack Byte
		_enabled = true;
	}
}

// Disabling data reporting in Stream Mode will make it behave like Remote Mode
void PS2Mouse::disable_data_reporting() {
	if (!_disabled) {
		write(0xf5); // Send disable data reporting
		read_byte(); // Read Ack Byte
		_disabled = true;
	}
}

void PS2Mouse::set_resolution(int resolution) {
	if (_mode == STREAM) {
		enable_data_reporting();
	}
	write(0xe8); // Send Set Resolution
	read_byte(); // Read ack Byte
	write(resolution); // Send resolution setting
	read_byte(); // Read ack Byte
	if (_mode == STREAM) {
		disable_data_reporting();
	}
	_delay_us(100);
}

void PS2Mouse::write(int data) {
	char i;
	char parity = 1;
	pull_high(_data_pin);
	pull_high(_clock_pin);
	_delay_us(300);
	pull_low(_clock_pin);
	_delay_us(300);
	pull_low(_data_pin);
	_delay_us(10);
	pull_high(_clock_pin); // Start Bit
	while (((PINB & (1<<_clock_pin))>>_clock_pin)); // wait for mouse to take control of clock)
	// clock is low, and we are clear to send data
	for (i=0; i < 8; i++) {
		if (data & 0x01) {
			pull_high(_data_pin);
			} else {
			pull_low(_data_pin);
		}
		// wait for clock cycle
		while (!((PINB & (1<<_clock_pin))>>_clock_pin));
		while (((PINB & (1<<_clock_pin))>>_clock_pin));
		parity = parity ^ (data & 0x01);
		data = data >> 1;
	}
	// parity
	if (parity) {
		pull_high(_data_pin);
		} else {
		pull_low(_data_pin);
	}
	while (!((PINB & (1<<_clock_pin))>>_clock_pin));
	while (((PINB & (1<<_clock_pin))>>_clock_pin));
	pull_high(_data_pin);
	_delay_us(50);
	while (((PINB & (1<<_clock_pin))>>_clock_pin));
	while ((!((PINB & (1<<_clock_pin))>>_clock_pin)) || (!((PINB & (1<<_data_pin))>>_data_pin))); // wait for mouse to switch modes
	pull_low(_clock_pin); // put a hold on the incoming data.
}

int16_t * PS2Mouse::report(int16_t data[]) {
	write(0xeb); // Send Read Data
	read_byte(); // Read Ack Byte
	data[0] = read(); // Status bit
	data[1] = read_movement_x(data[0]); // X Movement Packet
	data[2] = read_movement_y(data[0]); // Y Movement Packet
	return data;
}

int PS2Mouse::read() {
	return read_byte();
}

int8_t PS2Mouse::read_byte() {
	int8_t data = 0;
	pull_high(_clock_pin);
	pull_high(_data_pin);
	_delay_us(50);
	while (((PINB & (1<<_clock_pin))>>_clock_pin));
	_delay_us(5);  // not sure why.
	while (!((PINB & (1<<_clock_pin))>>_clock_pin)); // eat start bit
	for (int i = 0; i < 8; i++) {
//		bitWrite(data, i, read_bit());
		data |= (read_bit()<<i);
	}
	read_bit(); // Partiy Bit
	read_bit(); // Stop bit should be 1
	pull_low(_clock_pin);
	return data;
}

int PS2Mouse::read_bit() {
	while (((PINB & (1<<_clock_pin))>>_clock_pin));
	int bit = ((PINB & (1<<_data_pin))>>_data_pin);
	while (!((PINB & (1<<_clock_pin))>>_clock_pin));
	return bit;
}

int16_t PS2Mouse::read_movement_x(int status) {
	int16_t x = read();
	if (status&(1<<4)) {
		for(int i = 8; i < 16; ++i) {
			x |= (1<<i);
		}
	}
	return x;
}

int16_t PS2Mouse::read_movement_y(int status) {
	int16_t y = read();
	if (status&(1<<5)) {
		for(int i = 8; i < 16; ++i) {
			y |= (1<<i);
		}
	}
	return y;
}

void PS2Mouse::pull_low(int pin) {
	DDRB |= (1<<pin);
	PORTB &=~ (1<<pin);
}

void PS2Mouse::pull_high(int pin) {
	DDRB &=~ (1<<pin);
	PORTB |= (1<<pin);
}
