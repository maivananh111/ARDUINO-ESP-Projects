/*
 * TouchPad.h
 *
 * Created: 06/08/2021 4:12:26 CH
 *  Author: A315-56
 */ 


#ifndef TOUCHPAD_H_
#define TOUCHPAD_H_


#define REMOTE 1
#define STREAM 2

#include <stdint.h>

class PS2Mouse{
	public:
		PS2Mouse(int clock_pin, int data_pin, int mode);
		void Init();
		int read();
		int16_t* report(int16_t data[]);
		void write(int);
		void enable_data_reporting();
		void disable_data_reporting();
		void set_remote_mode();
		void set_stream_mode();
		void set_resolution(int);
		void set_scaling_2_1();
		void set_scaling_1_1();
		void set_sample_rate(int);
	
	private:
		int _clock_pin;
		int _data_pin;
		int _mode;
		int _initialized;
		int _enabled;
		int _disabled;
		int8_t read_byte();
		int read_bit();
		int16_t read_movement_x(int);
		int16_t read_movement_y(int);
		void pull_high(int);
		void pull_low(int);
		void set_mode(int);
};


#endif /* TOUCHPAD_H_ */