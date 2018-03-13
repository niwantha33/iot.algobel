
/*
   endPoint.ino sketch
                                           ___________
  endPoint (EasyNode Module)---------<---<--|iot pltform|----<----<---- End User(Mobile or browser)
                                           ___________
  ()
   Author : Niwantha
   Src: EasyNode iot.algobel.com
   Example: Connect to iot.algobel.com
   campany : Algobel Technologies & Innovations
*/

/*
  Variables : p_pin_status

  Constant :   {SSID, Password}- Wifi ; {host, port}-- platform; fingerpritn
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

const char *ssid = "xxxxxx";
const char *password = "xxxx";

const char *host = "iot.algobel.com";
const int httpsPort = 80;
// device key
const String key = "7e7a62790ee9879a8830755937d9xxxxxx";

WiFiClient client;

int p_pin_status =0;

void easyNode()
{
  //Json buffer setting
  StaticJsonBuffer<1024> jsonBuffer;

  if (!client.connect(host, httpsPort))
  {
    //Serial.println(F("connection failed"));
    return;
  }

  String url = "/connect?key=";
  url += key;

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: EasyNode\r\n" +
               "Connection: close\r\n\r\n");

  while (client.connected())
  {
    String line = client.readStringUntil('\n');
    if (line == "\r")
    {
      break;
    }
  }
  while (client.available())
  {
    String line = client.readStringUntil('\n');
    if (line.length() > 10)
    {
      JsonObject &root = jsonBuffer.parseObject(line);
      /***********************************************************************************/
     
      int current_st = root["pin_2"];     // current pin status  
      if (p_pin_status != current_st) {
        p_pin_status  = current_st;        
        digitalWrite(2, root["pin_2"]);
      }
      /***********************************************************************************/
    }
  }
 // client.stop();
}

void setup()
{
 // Serial.begin(115200);
  //Serial.println();

  /***********************************************************************************/
  // Setting all pins as OUTPUT and make all pins to LOW
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  /***********************************************************************************/
  //Serial.print(F("connecting to "));
 // Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  //  Serial.print(".");
  }
  //Serial.print("connecting to ");
 // Serial.println(host);
}

void loop()
{
  //double ptimer = millis();
  easyNode();
  //double delta = millis() - ptimer;
  ///Serial.print("T:");
 // Serial.println(delta);
}
