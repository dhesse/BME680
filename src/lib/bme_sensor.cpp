#include "bme_sensor.hpp"
#include <Wire.h>

// some code taken from the Adafruit BME680 library
// which can be found here
// https://github.com/adafruit/Adafruit_BME680
// and is distributed under BSD license

BME680::BME680()
{
    // Put initialization like pinMode and begin functions here.
  Wire.begin();

  gas_sensor.dev_id = 0x77;
  gas_sensor.intf = BME680_I2C_INTF;
  gas_sensor.read = user_i2c_read;
  gas_sensor.write = user_i2c_write;
  gas_sensor.delay_ms = user_delay_ms;
  /* amb_temp can be set to 25 prior to configuring the gas sensor 
     * or by performing a few temperature readings without operating the gas sensor.
     */
  gas_sensor.amb_temp = 25;

  int8_t rslt = BME680_OK;
  rslt = bme680_init(&gas_sensor);
#ifdef BME680_DEBUG
  Serial.println(rslt);
#endif
  bme680_get_profile_dur(&meas_period, &gas_sensor);

  uint8_t set_required_settings;

  /* Set the oversampling */
  gas_sensor.tph_sett.os_hum = BME680_OS_8X;
  gas_sensor.tph_sett.os_pres = BME680_OS_8X;
  gas_sensor.tph_sett.os_temp = BME680_OS_8X;
  gas_sensor.tph_sett.filter = BME680_FILTER_SIZE_3;

  /* Set the remaining gas sensor settings and link the heating profile */
  gas_sensor.gas_sett.run_gas = BME680_ENABLE_GAS_MEAS;
  /* Create a ramp heat waveform in 3 steps */
  gas_sensor.gas_sett.heatr_temp = 320; /* degree Celsius */
  gas_sensor.gas_sett.heatr_dur = 150;  /* milliseconds */

  /* Select the power mode */
  /* Must be set before writing the sensor configuration */
  gas_sensor.power_mode = BME680_FORCED_MODE;

  /* Set the required sensor settings needed */
  set_required_settings = BME680_OST_SEL | BME680_OSP_SEL | BME680_OSH_SEL | BME680_FILTER_SEL | BME680_GAS_SENSOR_SEL;

  /* Set the desired sensor configuration */
  rslt = bme680_set_sensor_settings(set_required_settings, &gas_sensor);

  /* Set the power mode */
  rslt = bme680_set_sensor_mode(&gas_sensor);
}

const struct bme680_field_data& BME680::get_data(){
     // The core of your code will likely live here.
  
  int8_t rslt = BME680_OK;
  /* Trigger the next measurement if you would like to read data out continuously */
  if (gas_sensor.power_mode == BME680_FORCED_MODE)
  {
    rslt = bme680_set_sensor_mode(&gas_sensor);
  } 
  user_delay_ms(meas_period); /* Delay till the measurement is ready */
  rslt = bme680_get_sensor_data(&data, &gas_sensor);
  return data;
}

void user_delay_ms(uint32_t period)
{
  /*
     * Return control or wait,
     * for a period amount of milliseconds
     */
  delay(period);
}

int8_t user_i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
#ifdef BME680_DEBUG
    Serial.print("\tI2C $");
    Serial.print(reg_addr, HEX);
    Serial.print(" => ");
#endif

    Wire.beginTransmission((uint8_t)dev_id);
    Wire.write((uint8_t)reg_addr);
    Wire.endTransmission();
    if (len != Wire.requestFrom((uint8_t)dev_id, (byte)len))
    {
#ifdef BME680_DEBUG
        Serial.print("Failed to read ");
        Serial.print(len);
        Serial.print(" bytes from ");
        Serial.println(dev_id, HEX);
#endif
        return 1;
    }
    while (len--)
    {
        *reg_data = (uint8_t)Wire.read();
#ifdef BME680_DEBUG
        Serial.print("0x");
        Serial.print(*reg_data, HEX);
        Serial.print(", ");
#endif
        reg_data++;
    }
#ifdef BME680_DEBUG
    Serial.println("");
#endif
    return 0;
}

int8_t user_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
#ifdef BME680_DEBUG
    Serial.print("\tI2C $");
    Serial.print(reg_addr, HEX);
    Serial.print(" <= ");
#endif
    Wire.beginTransmission((uint8_t)dev_id);
    Wire.write((uint8_t)reg_addr);
    while (len--)
    {
        Wire.write(*reg_data);
#ifdef BME680_DEBUG
        Serial.print("0x");
        Serial.print(*reg_data, HEX);
        Serial.print(", ");
#endif
        reg_data++;
    }
    Wire.endTransmission();
#ifdef BME680_DEBUG
    Serial.println("");
#endif
    return 0;
}