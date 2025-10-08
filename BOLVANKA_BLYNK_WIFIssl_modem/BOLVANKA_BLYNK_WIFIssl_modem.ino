//GOOD NEWS! Now you can use #include <WiFiClientSecure.h> and #include <TinyGsmClient.h> in the same code.
//And have a choice of Wi-Fi or GSM backup connection, using a convenient flag, without rebooting.
// Try first to put wrong wifi passport and you will see: code not stack, after 20 tryes to connect - next sentence in console:
// "ANY Connections not in use, but code is not stack! Fantastic!"
//////////////////////////////////////////////////////////////////////////////////////////
//  Blynk.configWiFi(auth, ssid, pass, domain, port, root_ca); // Set WiFi connection type
//  Blynk.configGsm(auth, modem, apn, user, pass, domain, port); // Set GSM connection type
//  Blynk.getConnectionType() // Get the type of the connection
//  Blynk.connectNetwork(); // Connect network depending on the connection type
//  Blynk.disconnectNetwork(); // Disconnect network and forget the connection type
//  Blynk.connect(); // Connect to the Blynk
//  Blynk.disconnect(); // Disconnect from the Blynk
//  Blynk.begin(); // Try to connect network and the Blynk in the loop
//  Blynk.connected(); // is The Blynk connected? 
//////////////////////////////////////////////////////////////////////////////////////////
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here, but realy possible to put it dynamicaly(!), in WEB interface, see my precompilled proof: https://github.com/olegfilatof/BLYNKmodified/blob/main/BOLVANKA_Blynk_WEB_auth_OK.ino.merged.bin
write to me if you want to compile your code with this functionality: olegfilatov@gmx.com
 */
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

// Your GPRS credentials
// Leave empty, if missing user or pass
char apn[] = "internet";
char userGSM[] = "";
char passGSM[] = "";
//Your WIFI credentials ssid and pass, do not forget to put it here:
char ssid[] = "PASSAT-3X3";
char passWIFI[] = "";

//Modem, connected to this ports:
#define RXmodem 18  // this ESP port, connect here TX from modem side
#define TXmodem 19  // this ESP port, connect here RX from modem side

// Select your modem here:
#define TINY_GSM_MODEM_SIM800
//#define TINY_GSM_MODEM_SIM900
//#define TINY_GSM_MODEM_M590
//#define TINY_GSM_MODEM_A6
//#define TINY_GSM_MODEM_A7
//#define TINY_GSM_MODEM_BG96
//#define TINY_GSM_MODEM_XBEE

// Default heartbeat interval for GSM is 60
// If you want override this value, uncomment and set this option:
//#define BLYNK_HEARTBEAT 30

#include "BlynkSSH.h"

//#define SerialMon Serial
#define SerialAT Serial2
TinyGsm modem(SerialAT);  // это название экземпляра модема

void setup() {
  Serial.begin(115200);
  //////////////////////////////////////////////////////////////////////////
  SerialAT.begin(19200, SERIAL_8N1, RXmodem, TXmodem);  // for GSM modem
  //////////////////////////////////////////////////////////////////////////
  //const char *auth, const char *ssid, const char *pass, const char *domain = BLYNK_DEFAULT_DOMAIN, uint16_t port = //BLYNK_DEFAULT_PORT)

  Blynk.configWiFi(BLYNK_AUTH_TOKEN, ssid, passWIFI);
  if (Blynk.connectNetwork()) {
    Blynk.connect();
  } else {  
    Serial.println("ANY Connections not in use, but code is not stack! Fantastic! ");
   
    
    Blynk.configGsm(BLYNK_AUTH_TOKEN, modem, apn, userGSM, passGSM);
    if (Blynk.connectNetwork())
      Blynk.connect();
    
  }


}

void loop() {
  if (Blynk.connected())   
      Blynk.run();
}
