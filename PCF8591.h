#ifndef PCF8591_H
#define PCF8591_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class PCF8591
{
private:
	uint8_t _i2c_addr;
	bool _power_save;
	bool _dac_enable;
public:
	PCF8591();
	PCF8591( uint8_t addr, bool power_save );
	void begin(void);

	uint8_t adc_raw_read( uint8_t channel );
	void adc_bulk_raw_read( uint8_t* res );

	double adc_read( uint8_t port, double v_ref );
	void adc_bulk_read( double* res, double v_ref );

	void dac_write( uint8_t value );
	void dac_shutdown();
};

#define PCF8591_ADDRESS 0x48

#endif //PCF8591_H

