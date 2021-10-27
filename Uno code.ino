//Arduino side code
//DHT11 Lib
#include "DHT.h"
//Arduino to NodeMCU Lib
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
//Initialise Arduino to NodeMCU (5=Rx & 6=Tx)
SoftwareSerial nodemcu(5, 6);
//Initialisation of DHT11 Sensor
float temp;
float hum;
DHT dht;
#define water_sensor A1
#define LDR_sensor A2
#define fire_sensor A0
#define ir_sensor A3
#define temp_sensor 2
#define buzzer 7
#define LDR_led 8
#define ir_led 9
void setup()
{
  Serial.begin(9600);
  pinMode(water_sensor, INPUT);
  pinMode(LDR_sensor, INPUT);
  pinMode(fire_sensor, INPUT);
  pinMode(ir_sensor, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(LDR_led, OUTPUT);
  pinMode(ir_led, OUTPUT);
  digitalWrite(buzzer, LOW);
  digitalWrite(LDR_led, LOW);
  digitalWrite(ir_led, LOW);
  
  
  dht.setup(2);
  nodemcu.begin(9600);
  delay(1000);
  Serial.println("Program started");
}

void loop()
{

  send_to_nodemuc();

}
void data()
{
  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  float water_level = analogRead(water_sensor);
  float LDR = analogRead(LDR_sensor);
  float fire_sensor = analogRead(fire_sensor);
  float motion_sensor = analogRead(A3);

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.print(water_level, 1);
  Serial.print("\t\t");
  Serial.print(LDR, 1);
  Serial.print("\t\t");
  Serial.println(fire_sensor, 1);
}
void send_to_nodemuc()
{
  StaticJsonDocument<1000> doc;
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  float water_detection = analogRead(water_sensor);
  float LDR = analogRead(LDR_sensor);
  float fire_state = analogRead(fire_sensor);
  float ir_state = digitalRead(ir_sensor);
  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.print(water_detection, 1);
  Serial.print("\t\t");
  Serial.print(LDR, 1);
  Serial.print("\t\t");
  Serial.println(fire_sensor, 1);
  //Obtain Temp and Hum data
  //  dht11_func();

  //Assign collected data to JSON Object
  doc["humidity"] = humidity;
  doc["temperature"] = temperature;
  doc["water_detection"] = water_detection;
  doc["LDR"] = LDR;
  doc["fire_sensor"] = 55.0;
  doc["ir_sensor"] = ir_state;
  doc["erroe"] = 3.38;
  //Send data to NodeMCU
  serializeJson(doc, nodemcu);
  if (fire_state < 1000)
  {
    digitalWrite(buzzer, HIGH);
  } else
  {
    digitalWrite(buzzer, LOW);
  }
  if (LDR<15)
  {
    digitalWrite(LDR_led, HIGH);
  }else
  {
    digitalWrite(LDR_led, LOW);
  }
  if (ir_state==0)
  {
    digitalWrite(ir_led, HIGH);
  }else
  {
    digitalWrite(ir_led, LOW);
  }
  delay(500);
}
