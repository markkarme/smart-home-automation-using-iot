
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <Servo.h>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
const char* ssid = "Smart Home";
const char* password = "12345678";
float humidity = 0;
float temperature = 0;
float water_detection = 0;
float LDR = 0;
float fire_sensor = 0;
float ir_sensor = 0;

String username_log, password_log;
#define USERNAME_LOG_ADDRESS 0x01
#define USERPASSWORD_LOG_ADDRESS 0x30
#define MAINSERVOPIN D0
#define HOMESERVOPIN D1
#define GARAGESERVOPIN D2
#define WINDOWSERVOPIN D3
#define ROOM1LIGHT D4
#define ROOM2LIGHT D5
#define ROOM3LIGHT D6
#define ROOM4LIGHT D7
#define ROOM5LIGHT D8
#define SECURITYROOMADDRESS 0x50
Servo main_servo, home_servo, garage_servo, window_servo;
bool set_window = true, window_servo_opened = false, main_servo_opened = false, garage_servo_opened = false;

// (Rx, Tx)
SoftwareSerial nodemcu(D6, D5);
ESP8266WebServer server(80);
void handleRoot() {
  Serial.print("in on");
  server.send(200, "text/plain", "READY");
}
void handle_sensor()
{
  String json = "";
  Serial.print("in on");
  int light1 = digitalRead(ROOM1LIGHT);
  int light2 = digitalRead(ROOM2LIGHT);
  int light3 = digitalRead(ROOM3LIGHT);
  int light4 = digitalRead(ROOM4LIGHT);
  int light5 = digitalRead(ROOM5LIGHT);
  json = json + "temperature: " + (String)temperature + " " + "humidity: " + (String)humidity + " " + "water_detection: " + (String)water_detection + " ";
  json = json + "LDR: " + (String)LDR + " " + "fire_sensor: " + (String)fire_sensor + " ";
  json = json + "room1: " +  String(light1) + " " + "room2: " +  String(light2) + " " + "room3: " +  String(light3) + " " + "room4: " +  String(light4) + " " ;
  json = json + "room5: " +  String(light5) + " ";
  json = json + "window_servo_opened: " + window_servo_opened + " " ;
  json = json + "main_servo_opened: " + main_servo_opened + " " ;
  json = json + "garage_servo_opened: " + garage_servo_opened;

  server.send(200, "text/plain", json);

}
// handel room 1
void room1_light_on()
{
  digitalWrite(ROOM1LIGHT, HIGH);
  server.send(200, "text/plain", "room1_light_on");

}
void room1_light_off()
{
  digitalWrite(ROOM1LIGHT, LOW);
  server.send(200, "text/plain", "room1_light_off");

}
// handel room 2
void room2_light_on()
{
  digitalWrite(ROOM2LIGHT, HIGH);
  server.send(200, "text/plain", "room1_light_on");

}
void room2_light_off()
{
  digitalWrite(ROOM2LIGHT, LOW);
  server.send(200, "text/plain", "room1_light_off");

}
// handel room 3
void room3_light_on()
{
  digitalWrite(ROOM3LIGHT, HIGH);
  server.send(200, "text/plain", "room3_light_on");

}
void room3_light_off()
{
  digitalWrite(ROOM3LIGHT, LOW);
  server.send(200, "text/plain", "room3_light_off");
}
// handel room 4
void room4_light_on()
{
  digitalWrite(ROOM4LIGHT, HIGH);
  server.send(200, "text/plain", "room4_light_on");

}
void room4_light_off()
{
  digitalWrite(ROOM4LIGHT, LOW);
  server.send(200, "text/plain", "room4_light_off");
}
// handel room 5
void room5_light_on()
{
  digitalWrite(ROOM5LIGHT, HIGH);
  server.send(200, "text/plain", "room5_light_on");

}

void room5_light_off()
{
  digitalWrite(ROOM5LIGHT, LOW);
  server.send(200, "text/plain", "room5_light_off");

}
// handel window servo
void window_servo_on()
{
  Serial.println("hi");
  window_servo_opened = true;
  window_servo.write(90);
  server.send(200, "text/plain", "window_opened");
  /* if (set_window)
    {
     window_servo.write(90);
     server.send(200, "text/plain", "window_opened");
    } else
    {
     server.send(200, "text/plain", "raining out side");
    }*/
}
void window_servo_off()
{
  window_servo_opened = false;
  if (set_window)
  {
    window_servo.write(0);
    server.send(200, "text/plain", "window_opened");
  } else
  {
    server.send(200, "text/plain", "raining out side");
  }


}
// handel main servo
void main_servo_on()
{
  Serial.println("hi");
  main_servo_opened = true;
  main_servo.write(90);
  server.send(200, "text/plain", "main_opened");

}
void main_servo_off()
{
  main_servo.write(0);
  main_servo_opened = false;
  server.send(200, "text/plain", "main_closed");

}
// handel garage servo
void garage_servo_on()
{
  Serial.println("hi");
  garage_servo_opened = true;
  garage_servo.write(90);
  server.send(200, "text/plain", "garage_opened");

}
void garage_servo_off()
{

  garage_servo.write(0);
  garage_servo_opened = false;
  server.send(200, "text/plain", "garage_close");

}

void set_username_password_log(String password)
{
  String username = "admin";
  for (int i = 0; i < username.length(); i++)
  {
    EEPROM.write(USERNAME_LOG_ADDRESS + i, username[i]); //Write one by one with starting address of 0x0F
  }

  for (int i = 0; i < password.length(); i++)
  {
    EEPROM.write(USERPASSWORD_LOG_ADDRESS + i, password[i]); //Write one by one with starting address of 0x0F
  }
  EEPROM.commit();
}
void get_username_password_log()
{
  username_log = "";
  password_log = "";
  for (int i = 0; i < 4; i++)
  {
    username_log = username_log + char(EEPROM.read(USERNAME_LOG_ADDRESS + i)); //Read one by one with starting address of 0x0F
  }
  for (int i = 0; i < 4; i++)
  {
    password_log = password_log + char(EEPROM.read(USERPASSWORD_LOG_ADDRESS + i)); //Read one by one with starting address of 0x0F
  }

  Serial.println(username_log + " " + password_log);
}

void log_in()
{
  String name1 = server.arg("name_log");
  String pass = server.arg("password_log");
  Serial.println(name1);
  Serial.println(pass);
  get_username_password_log();
  if (username_log == name1 && password_log == pass)
  {
    server.send(200, "text/plain", "Ok");
  } else
  {
    server.send(200, "text/plain", "NO");
  }
}
void update_log_in()
{
  String new_password = server.arg("update_log_in_password");
  set_username_password_log(new_password);
  server.send(200, "text/plain", "update_login_password");

}
void set_security_room_password(String password)
{
  for (int i = 0; i < password.length(); i++)
  {
    EEPROM.write(SECURITYROOMADDRESS + i, password[i]); //Write one by one with starting address of 0x0F
  }
}
String  get_security_room_password(String entered_password)
{
  String password = "";
  for (int i = 0; i < 6; i++)
  {
    password = password + char(EEPROM.read(SECURITYROOMADDRESS + i)); //Read one by one with starting address of 0x0F
  }
  return password;
}
void handel_security_room_on()
{
  String pass = server.arg("password_security");
  String saved_password = get_security_room_password(pass);
  Serial.println(pass);
  Serial.println(saved_password);
  if (pass == saved_password)
  {
    server.send(200, "text/plain", "Secuirty-room-servo-on");
  } else
  {
    server.send(200, "text/plain", "Wrong-passsword");
  }
}
void update_security_room()
{
  String pass = server.arg("update_password_security");
  set_security_room_password(pass);
  server.send(200, "text/plain", "update_security_pass");
}
void handleOff()
{
  digitalWrite(LED_BUILTIN, LOW);
  Serial.print("in off");
  server.send(200, "text/plain", "DONE");
}
void setup()
{
  EEPROM.begin(512);
  Serial.begin(9600);
  nodemcu.begin(9600);
  delay(1000);
  WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
//  WiFi.softAP(ssid, password);
//  WiFi.setOutputPower(20.5);

  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ROOM1LIGHT, OUTPUT);
  pinMode(ROOM2LIGHT, OUTPUT);
  pinMode(ROOM3LIGHT, OUTPUT);
  pinMode(ROOM4LIGHT, OUTPUT);
  pinMode(ROOM5LIGHT, OUTPUT);
  //setup home servo
  home_servo.attach(HOMESERVOPIN);
  home_servo.write(0);
  //setup main servo
  main_servo.attach(MAINSERVOPIN);
  main_servo.write(0);
  //setup garage servo
  garage_servo.attach(GARAGESERVOPIN); //D1
  garage_servo.write(0);
  //setup window servo
  window_servo.attach(WINDOWSERVOPIN); //D1
  window_servo.write(0);
  //set_username_password_log("2020");
  set_security_room_password("246810");
  server.on("/open.security.room", handel_security_room_on);
  server.on("/update.security.room", update_security_room);
  server.on("/", handleRoot);
  server.on("/sensors.state", handle_sensor);
  server.on("/login", log_in);
  // handel room 1 light
  server.on("/room1.light.on", room1_light_on);
  server.on("/room1.light.off", room1_light_off);
  // handel room 2 light
  server.on("/room2.light.on", room2_light_on);
  server.on("/room2.light.off", room2_light_off);
  // handel room 3 light
  server.on("/room3.light.on", room3_light_on);
  server.on("/room3.light.off", room3_light_off);
  // handel room 4 light
  server.on("/room4.light.on", room4_light_on);
  server.on("/room4.light.off", room4_light_off);
  // handel room 3 light
  server.on("/room5.light.on", room5_light_on);
  server.on("/room5.light.off", room5_light_off);
  server.on("/window.servo.on", window_servo_on);
  server.on("/window.servo.off", window_servo_off);
  server.on("/main.servo.on", main_servo_on);
  server.on("/main.servo.off", main_servo_off);
  server.on("/garage.servo.on", garage_servo_on);
  server.on("/garage.servo.off", garage_servo_off);
  server.begin();
  while (!Serial) continue;
}
void loop()
{
  server.handleClient();
  read_data_from_uno();
}
void read_data_from_uno()
{
  StaticJsonDocument<1000> doc;
  DeserializationError error = deserializeJson(doc, Serial);
  Serial.println("JSON Object Recieved");
  Serial.print("Recieved Humidity:  ");
  humidity = doc["humidity"];
  temperature = doc["temperature"];
  water_detection = doc["water_detection"];
  LDR = doc["LDR"];
  fire_sensor = doc["fire_sensor"];
  ir_sensor = doc["ir_sensor"];

  Serial.println(humidity);
  Serial.print("Recieved Temperature:  ");
  Serial.println(temperature);
  Serial.print("Recieved water_detection:  ");
  Serial.println(water_detection);
  Serial.print("Recieved LDR:  ");
  Serial.println(LDR);
  Serial.print("Recieved fire_sensor:  ");
  Serial.println(fire_sensor);
  float err = doc["error"];
  Serial.println("-----------------------------------------");
  if (fire_sensor < 1000)
  {
    set_window = false;
    window_servo.write(90);
  } else
  {
    set_window = true;
  }


}
