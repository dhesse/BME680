/*
 * Project BME680
 * Description:
 * Author:
 * Date:
 */

#include "lib/bme_sensor.hpp"
#include "lib/ujson.hpp"

TCPClient client;
// EXAMPLE USAGE

byte server[] = { 192, 168, 0, 163 }; // Server
uint port = 8080; // Port

uint meas_delay_seconds = 60 * 15;

BME680 *gas_sensor;

void setup()
{
  gas_sensor = new BME680();
  gas_sensor->get_data();


}

// loop() runs over and over again, as quickly as it can execute.
void loop()
{
  bme680_field_data data = gas_sensor->get_data();

  JSON json;
  json.push("temperature", data.temperature / 100.0f);
  json.push("pressure", data.pressure / 100.0f);
  json.push("humidity", data.humidity / 1000.0f);
  if (data.status & BME680_GASM_VALID_MSK)
    json.push("resistance", data.gas_resistance);
  char res[100];
  int size =  json.format(res);
  Serial.println(size);
  if (client.connect(server, port))
  {
    Serial.println("connected");
    client.println("GET / HTTP/1.0");
    client.println("Content-Type: application/json");
    client.println("Content-Length: " + String(size));
    client.println();
    client.print(res);
  }
  else
  {
    Serial.println("connection failed");
  }
  client.stop();
  Serial.println(res);
  // Particle.publish("ambience/living_room", res, PRIVATE);
  delay(2000);
  // System.sleep(SLEEP_MODE_DEEP, meas_delay_seconds);
};