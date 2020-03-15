/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/dirkhesse/code/BME680/src/BME680.ino"
/*
 * Project BME680
 * Description:
 * Author:
 * Date:
 */

#include "lib/bme_sensor.hpp"
#include "lib/ujson.hpp"

void setup();
void loop();
#line 11 "/Users/dirkhesse/code/BME680/src/BME680.ino"
BME680 *gas_sensor;

void setup()
{
  gas_sensor = new BME680();
  gas_sensor->get_data();
}

uint meas_delay_seconds = 60*15;


// loop() runs over and over again, as quickly as it can execute.
void loop()
{
  bme680_field_data data = gas_sensor->get_data();

  JSON json;
  json.push("temperature", data.temperature / 100.0f);
  json.push("pressure", data.pressure / 100.0f);
  json.push("humidity", data.humidity / 1000.0f);
  if(data.status & BME680_GASM_VALID_MSK)
            json.push("resistance", data.gas_resistance);
  char res[80];
  json.format(res);
  Serial.println(res);
  Particle.publish("ambience/living_room", res, PRIVATE);
  System.sleep(SLEEP_MODE_DEEP, meas_delay_seconds);
};