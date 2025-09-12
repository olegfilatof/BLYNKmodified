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

// PLEASE, PUT HERE full parh to this file on your PC!!! libraries/Blynk/src/Adapters/BlynkGsmClient.h
#include </home/e/Arduino/libraries/Blynk/src/Adapters/BlynkGsmClient.h> // full parh to this file on my PC

static BlynkArduinoClient _blynkSIMTransport;
BlynkSIM g_blynkSIM(_blynkSIMTransport);

#endif

// Your GPRS credentials
// Leave empty, if missing user or pass
char apn[] = "internet";
char GSMuser[] = "";
char GSMpass[] = "";

//Modem:
#define RXmodem 18 // this is  ESP port 18, connect here TX from modem side!
#define TXmodem 19 // this is  ESP port 19, connect here RX from modem side!

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