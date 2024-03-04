#include <UbiConstants.h>
#include <UbidotsEsp32Mqtt.h>
#include <UbiTypes.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "DHT.h"

/* CODIGO JOSÉ MIGUEL VÉLEZ */

#define DHTPIN 27   //pin 27 del ttgo     
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);
TFT_eSPI tft = TFT_eSPI();

/****************************************
 * Define Constants
 ****************************************/
const char *UBIDOTS_TOKEN = "BBUS-MhREkiB2dovQplRO9Jr77aisaszzZm";  // Put here your Ubidots TOKEN
const char *WIFI_SSID = "UPBWiFi";      // Put here your Wi-Fi SSID
const char *WIFI_PASS = "";      // Put here your Wi-Fi password
const char *DEVICE_LABEL = "esp32";   // Put here your Device label to which data  will be published
const char *VARIABLE_LABEL1 = "Temperatura"; // Put here your Variable label to which data  will be published
const char *VARIABLE_LABEL2 = "Humedad";
const int PUBLISH_FREQUENCY = 5000; // Update rate in milliseconds

unsigned long timer;


Ubidots ubidots(UBIDOTS_TOKEN);

/****************************************
 * Auxiliar Functions
 ****************************************/

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

/****************************************
 * Main Functions
 ****************************************/

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  /*tft.init();    //OLED
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_RED);
  tft.drawString("Sin conexion a", 15, 100, 2);
  tft.drawString(WIFI_SSID, 20, 120, 2);*/
  


  Serial.println(F("DHTxx test!")); //sensor
  dht.begin();
  
  tft.init();    //OLED
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("Temp.", 10, 22, 4);
  tft.drawString("Humedad", 10, 132, 4);
  tft.drawString("C", 115, 60, 4);
  tft.drawString("o", 110, 60);
  tft.drawString("%", 110, 170, 4);
  
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) { //revisar errores
    Serial.println(F("ERROR"));
    tft.setTextColor(TFT_RED);
    tft.drawString("ERROR", 10, 60, 4);
    tft.drawString("ERROR", 10, 170, 4);
    return;
    }

    tft.setTextColor(TFT_CYAN);
    tft.drawString(String(t,1), 10, 50, 6);
    tft.drawString(String(h,1), 10, 160, 6);

    ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
    ubidots.setCallback(callback);
    ubidots.setup();
    ubidots.reconnect();
  

    if (!ubidots.connected())
    {
      tft.setTextColor(TFT_RED);
      tft.drawString("Sin conexion a", 20, 200, 2);
      tft.drawString(WIFI_SSID, 20, 220, 2);
      
      ubidots.reconnect();
    }
     if (ubidots.connected())
    {
    tft.setTextColor(TFT_GREEN);
      tft.fillRect(19, 199, 100, 40, TFT_BLACK);
      tft.drawString("Conectado a", 20, 200, 2);
      tft.drawString(WIFI_SSID, 20, 220, 2);
    }

    ubidots.add(VARIABLE_LABEL1, t); // Insert your variable Labels and the value to be sent
    ubidots.add(VARIABLE_LABEL2, h);
    
   ubidots.publish(DEVICE_LABEL);
   timer = millis();

   delay(1500);



  esp_sleep_enable_timer_wakeup(15*1000000);   // 5 uS

  Serial.flush();
  
  esp_deep_sleep_start();
}

void loop()
{
  // put your main code here, to run repeatedly:
  
    
  /*if ((millis() - timer) > PUBLISH_FREQUENCY) // triggers the routine every 5 seconds
  {


    
  }
  ubidots.loop();

  
   tft.setTextColor(TFT_BLACK);
   tft.drawString(String(t,1), 10, 50, 6);
   tft.drawString(String(h,1), 10, 160, 6);*/
}
