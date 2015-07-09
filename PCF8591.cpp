#include "PCF8591.h"

#include <Wire.h>

PCF8591::PCF8591()
{
	_i2c_addr = PCF8591_ADDRESS;
	_power_save = false;
}

PCF8591::PCF8591( uint8_t addr, bool power_save )
{
	if ( addr > 7 )
	{
		addr = 0;
	}
	_i2c_addr = PCF8591_ADDRESS | addr;
	_power_save = power_save;
}

void PCF8591::begin( void )
{
	dac_write(0);
	if ( _power_save )
	{
		dac_shutdown();
	}
}

uint8_t PCF8591::adc_raw_read(uint8_t channel)
{
	uint8_t retval = 0;
	if (channel < 4)
	{
		/*
		*	select channel
		*/
		Wire.beginTransmission( _i2c_addr );
		Wire.write( 0x40 | channel );
		if ( not _dac_enable )
		{
			delayMicroseconds( 100 );
		}
		Wire.endTransmission();

		/*
		* request data
		*/
		Wire.requestFrom( _i2c_addr, 2 );
		Wire.read();
		retval = Wire.read();

		/*
		* shut down the oscilator
		*/
		if ( _power_save and not _dac_enable )
		{
			dac_shutdown();
		}
	}
	return (retval);
}

void PCF8591::adc_bulk_raw_read( uint8_t* res)
{
	/*
	 *	select channel
	 */
	Wire.beginTransmission( _i2c_addr );
	Wire.write( 0x44 );
	if ( not _dac_enable )
	{
		delayMicroseconds( 100 );
	}
	Wire.endTransmission();
	
	/*
	 * request data
	 */
	Wire.requestFrom( _i2c_addr, 5 );
	Wire.read();
	res[0] = Wire.read();
	res[1] = Wire.read();
	res[2] = Wire.read();
	res[3] = Wire.read();

	/*
	 * shut down the oscilator
	 */
	if ( _power_save and not _dac_enable )
	{
		dac_shutdown();
	}
}

double PCF8591::adc_read( uint8_t channel, double v_ref )
{
	return ( v_ref * adc_raw_read( channel ) / 255.0 );
}

void PCF8591::adc_bulk_read( double* res, double v_ref )
{
	uint8_t adc_data[4];
	adc_bulk_raw_read( &adc_data[0] );
	res[0] = v_ref * adc_data[0] / 255.0;
	res[1] = v_ref * adc_data[1] / 255.0;
	res[2] = v_ref * adc_data[2] / 255.0;
	res[3] = v_ref * adc_data[3] / 255.0;
}
void PCF8591::dac_shutdown()
{
	Wire.beginTransmission( _i2c_addr );
	Wire.write( 0 );
	Wire.endTransmission();
}

void PCF8591::dac_write( uint8_t value )
{
	_dac_enable = true;
	Wire.beginTransmission( _i2c_addr );
	Wire.write( 0x40 );
	Wire.write( value );
	Wire.endTransmission();
}
