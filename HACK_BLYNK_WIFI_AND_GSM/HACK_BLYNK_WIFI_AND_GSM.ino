//V0 in the Blynk: switch between wifi and modem connection. 
//Now you can dynamically change type of connections on demand! 
//FOR ESP32 only!!!
//////////////////////////////////////////////////////////////////
//PLEASE FILL IN THIS:
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL4fa-kn3Dz"
#define BLYNK_TEMPLATE_NAME "CAR GPS"
#define BLYNK_AUTH_TOKEN "HDl45JpsYRZgO3nOyNjqZLBxPqDeuFlx"

const char apn[] = "internet";   // Here - your SIM card APN
const char wifiSSID[] = "";     // Here - your router SSID
const char wifiPASSWORD[] = "";// Here - your router PASSWORD

//////////////////////////////////////////////////////////////////
#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_RX_BUFFER 512  // buffer for internet =  0.5 Кб

#include "BlynkWiFiAndGSM.h"

WiFiData wifiData(BLYNK_AUTH_TOKEN, wifiSSID, wifiPASSWORD);

#define SerialAT Serial2
TinyGsm modem(SerialAT);

GsmData gsmData(BLYNK_AUTH_TOKEN, modem, apn);

int changeConnectionFlag = -1;

void setup() {
  Serial.begin(115200);
  SerialAT.begin(19200, SERIAL_8N1, 18, 19);

  pinMode(0, INPUT);

  wifiData.auth = BLYNK_AUTH_TOKEN;
  wifiData.ssid = "PASSAT-3X3";
  wifiData.pass = "";

  gsmData.auth = BLYNK_AUTH_TOKEN;
  gsmData.modem = &modem;
  gsm.apn = apn;
////////////////////////////////////////
  modem.restart();
  Blynk.setConnectionData(gsmData);
  if (Blynk.connectNetwork())
  {
    while (!Blynk.connect()) {}
    Blynk.virtualWrite(V20, 1);
    Blynk.virtualWrite(V21, 1);
    Blynk.virtualWrite(V13, 1);
  }
}

void loop() {
  Blynk.run();

  switch (changeConnectionFlag) {
    case 0:
      changeConnectionFlag = -1;

      Blynk.setConnectionData(wifiData);
      if (Blynk.connectNetwork()) {
        while (!Blynk.connect()) {}
        Blynk.virtualWrite(V21, 0);
        Blynk.virtualWrite(V20, 0);
        Blynk.virtualWrite(V13, 1);
      }
      
      break;
    case 1:
      changeConnectionFlag = -1;

      modem.restart();
      Blynk.setConnectionData(gsmData);
      if (Blynk.connectNetwork()) {
        while (!Blynk.connect()) {}
        Blynk.virtualWrite(V20, 1);
        Blynk.virtualWrite(V21, 1);
        Blynk.virtualWrite(V13, 1);
      }

      break;
  }
}

//switch V0
BLYNK_WRITE(V0) {
  changeConnectionFlag = param.asInt();  // wifi 0, modem 1
}
///////////////////////