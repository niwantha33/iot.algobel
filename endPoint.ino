
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
  Variables :  p_s_time

  Constant :   {SSID, Password}- Wifi ; {host, port}-- platform; fingerpritn
*/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

const char *ssid = "kten";
const char *password = "1234567a";

const char *host = "iot.algobel.com";
const int httpsPort = 443;
// device key
const String key = "789bc3b3a779e95a30af1f24af9cd5b6";

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char *fingerprint = "5b 2e 4d 17 50 3e e8 1b f6 90 90 bd 9e 72 89 c1 fb a8 2d 40";

// Use WiFiClientSecure class to create TLS connection
WiFiClientSecure client;

String p_s_time;

void easyNode()
{
  //Json buffer setting
  StaticJsonBuffer<1024> jsonBuffer;

  if (!client.connect(host, httpsPort))
  {
    Serial.println(F("connection failed"));
    return;
  }

  if (client.verify(fingerprint, host))
  {
    // Serial.println(F("certificate matches"));
  }
  else
  {
    Serial.println("certificate doesn't match");
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
     
      String s_time = root["s_time"];
      
      if (p_s_time != s_time) {
        p_s_time = s_time;
        digitalWrite(13, root["pin_13"]);
        digitalWrite(12, root["pin_12"]);
        digitalWrite(14, root["pin_14"]);
        digitalWrite(16, root["pin_16"]);
        digitalWrite(2, root["pin_2"]);
      }
      /***********************************************************************************/
    }
  }
 // client.stop();
}

void setup()
{
  Serial.begin(115200);
  Serial.println();

  /***********************************************************************************/
  // Setting all pins as OUTPUT and make all pins to LOW
  pinMode(13, OUTPUT);
  digitalWrite(12, LOW);
  pinMode(12, OUTPUT);
  digitalWrite(14, LOW);
  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  /***********************************************************************************/
  Serial.print(F("connecting to "));
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("connecting to ");
  Serial.println(host);
}

void loop()
{
  //double ptimer = millis();
  easyNode();
  //double delta = millis() - ptimer;
  ///Serial.print("T:");
 // Serial.println(delta);
}
