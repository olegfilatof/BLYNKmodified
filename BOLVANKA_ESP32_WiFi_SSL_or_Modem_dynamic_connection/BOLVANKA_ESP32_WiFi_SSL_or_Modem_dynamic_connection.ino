/* Use in Blynk  V0 switch, that in real time changing type of connection: wifi or modem. 

*/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "PASSAT-3X3";
char pass[] = "";
//////////////////////////////////////////////
//*** MODEM /**********************************************************************
// Select your modem:
#define TINY_GSM_MODEM_SIM800
//#define TINY_GSM_MODEM_SIM900
//#define TINY_GSM_MODEM_A6
//#define TINY_GSM_MODEM_A7
#define TINY_GSM_RX_BUFFER   512  // buffer for internet =  0.5 Кб

#include <TinyGsmClient.h>
// Default heartbeat interval for GSM is 60
// If you want override this value, uncomment and set this option:
//#define BLYNK_HEARTBEAT 30
//*** MODEM /**********************************************************************

//#include "MyBlynk.h"

/**
 * @file       BlynkSimpleTinyGSM.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Nov 2016
 * @brief
 *
 */

#ifndef MyBlynkSimpleTinyGSM_h
#define MyBlynkSimpleTinyGSM_h

#include </home/e/Arduino/libraries/Blynk/src/Adapters/BlynkGsmClient.h> // full parh to this file on your PC

static BlynkArduinoClient _blynkSIMTransport;
BlynkSIM g_blynkSIM(_blynkSIMTransport);

#endif

// Your GPRS credentials
// Leave empty, if missing user or pass
char apn[] = "internet";
char GSMuser[] = "";
char GSMpass[] = "";

//Modem:
#define RXmodem 18 // это обозначен порт ESP, подключать сюда TX modem side!
#define TXmodem 19 // это обозначен порт ESP, подключать сюда RX modem side!

#define SerialMon Serial
#define SerialAT Serial2
TinyGsm modem(SerialAT);


#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <BlynkSimpleEsp32_SSL.h>
#endif
/////////////////////////////////////////////
bool startModem_F = true; 
bool connectionSwitch_F = false; // wifi start first

void setup()
{
  // Debug console
  Serial.begin(115200);
SerialAT.begin(19200, SERIAL_8N1, RXmodem, TXmodem);
// wifi connect first (for example: may be modem first, or start after timer or some if(something happen)... )
WIFI_START();
//MODEM_START();
}

void loop()
{
    if (startModem_F)        { g_blynkSIM.run(); } // GSM modem
  else   { Blynk.run();      }                    // wifi blynk
}

//switch V0
  BLYNK_WRITE(V0) 
  {
  int startModem_F1 = param.asInt(); // wifi ON - startModem_F1 == false
  
  if (startModem_F1) {  Serial.println("MODEM IS ON! Wifi stop "); WiFi.disconnect(); Blynk.disconnect(); MODEM_START(); } //WiFi.mode(WIFI_OFF);
  else { Serial.println("WIFI IS ON! Modem stop "); Blynk.disconnect(); WIFI_START();  }
  }

void MODEM_START() {
modem.init();

  int csq;
  for (uint8_t counterLimit = 0; counterLimit != 5; ++counterLimit) {
    csq = modem.getSignalQuality();
    if (csq > 5) 
      break;
    delay(1000);
  };

 if ((modem.getSimStatus() == 0) or (csq <= 5)) { 
    Serial.println("MODEM NOT STARTED, reason: no SIM, weak GSM sygnal, you're all thumbs... "); 
  Serial.println(" WIFI start again after restart ESP");
  delay (3000);
ESP.restart();
    }
  
  else {

  g_blynkSIM.config(modem, BLYNK_AUTH_TOKEN, "blynk.cloud", 8080);
  g_blynkSIM.connectNetwork(apn, GSMuser, GSMpass);
  g_blynkSIM.connect();
  startModem_F = true;
  }
}

void WIFI_START() {
  WiFi.mode(WIFI_STA);
Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
startModem_F = false;
}
