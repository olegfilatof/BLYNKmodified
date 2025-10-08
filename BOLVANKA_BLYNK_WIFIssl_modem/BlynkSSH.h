#ifndef BLYNK_ESP32_WIFI_AND_GSM_SSL_H
#define BLYNK_ESP32_WIFI_AND_GSM_SSL_H

#ifndef ESP32
#error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.
#endif

#include <Blynk.h>
#include <Blynk/BlynkProtocol.h>
#include <Adapters/BlynkArduinoClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <TinyGsmClient.h>

static const char BLYNK_DEFAULT_ROOT_CA[] =
#include "certs_pem.h"

#ifndef BLYNK_INFO_CONNECTION
#if defined(TINY_GSM_MODEM_SIM800)
#define BLYNK_INFO_CONNECTION "SIM800"
#elif defined(TINY_GSM_MODEM_SIM900)
#define BLYNK_INFO_CONNECTION "SIM900"
#elif defined(TINY_GSM_MODEM_UBLOX)
#define BLYNK_INFO_CONNECTION "UBLOX"
#elif defined(TINY_GSM_MODEM_BG96)
#define BLYNK_INFO_CONNECTION "BG96"
#elif defined(TINY_GSM_MODEM_A6)
#define BLYNK_INFO_CONNECTION "A6"
#elif defined(TINY_GSM_MODEM_A7)
#define BLYNK_INFO_CONNECTION "A7"
#elif defined(TINY_GSM_MODEM_M590)
#define BLYNK_INFO_CONNECTION "M590"
#elif defined(TINY_GSM_MODEM_XBEE)
#define BLYNK_INFO_CONNECTION "XBEE"
#else
#define BLYNK_INFO_CONNECTION "TinyGSM"
#endif
#endif

#ifndef BLYNK_HEARTBEAT
#define BLYNK_HEARTBEAT 60
#endif

#ifndef BLYNK_TIMEOUT_MS
#define BLYNK_TIMEOUT_MS 6000
#endif

#define BLYNK_SEND_ATOMIC

  enum class BlynkConnectionType {
    None,
    WiFi,
    Gsm
  };

class BlynkEsp32WiFiAndGsm
  : public BlynkProtocol<BlynkArduinoClient> {
  typedef BlynkProtocol<BlynkArduinoClient> Base;
public:
  BlynkEsp32WiFiAndGsm(BlynkArduinoClient& transp)
    : Base(transp) {
  }

  // Remove the old connection and config the new
  void configWiFi(const char* auth,
                  const char* ssid,
                  const char* pass,
                  const char* domain = BLYNK_DEFAULT_DOMAIN,
                  uint16_t port = BLYNK_DEFAULT_PORT_SSL,
                  const char* root_ca = BLYNK_DEFAULT_ROOT_CA) {
    // Disconnect the last connection
    disconnectNetwork();

    connectionType = BlynkConnectionType::WiFi;
    Base::begin(auth);
    this->conn.setClient(&wifiClientSecure);
    this->conn.begin(domain, port);
    wifiClientSecure.setHandshakeTimeout(30);
    wifiClientSecure.setCACert(root_ca);
    wifiData = { ssid, pass };
  }

  void configWiFi(const char* auth,
                  const char* ssid,
                  const char* pass,
                  IPAddress ip,
                  uint16_t port = BLYNK_DEFAULT_PORT_SSL,
                  const char* root_ca = BLYNK_DEFAULT_ROOT_CA) {
    // Disconnect the last connection
    disconnectNetwork();

    connectionType = BlynkConnectionType::WiFi;
    Base::begin(auth);
    this->conn.setClient(&wifiClientSecure);
    this->conn.begin(ip, port);
    wifiClientSecure.setHandshakeTimeout(30);
    wifiClientSecure.setCACert(root_ca);
    wifiData = { ssid, pass };
  }

  // Remove the old connection and config the new
  void configGsm(const char* auth,
                 TinyGsm& gsm,
                 const char* apn,
                 const char* user,
                 const char* pass,
                 const char* domain = BLYNK_DEFAULT_DOMAIN,
                 uint16_t port = BLYNK_DEFAULT_PORT_SSL) {
    // Disconnect the last connection
    disconnectNetwork();

    connectionType = BlynkConnectionType::Gsm;
    Base::begin(auth);
    gsmClient.init(&gsm);
    this->conn.setClient(&gsmClient);
    this->conn.begin(domain, port);
    gsmData = { &gsm, apn, user, pass };
  }

  // Remove the old connection and config the new
  void configGsm(const char* auth,
                 TinyGsm& gsm,
                 const char* apn,
                 const char* user,
                 const char* pass,
                 IPAddress ip,
                 uint16_t port = BLYNK_DEFAULT_PORT_SSL) {
    // Disconnect the last connection
    disconnectNetwork();

    connectionType = BlynkConnectionType::Gsm;
    Base::begin(auth);
    gsmClient.init(&gsm);
    this->conn.setClient(&gsmClient);
    this->conn.begin(ip, port);
    gsmData = { &gsm, apn, user, pass };
  }

  void disconnectNetwork() {
    // Desconnect connection depending on the connection type
    switch (connectionType) {
      case BlynkConnectionType::WiFi:
        disconnectWiFi();
        break;
      case BlynkConnectionType::Gsm:
        disconnectGsm();
        break;
    }

    // Reset connection type
    connectionType = BlynkConnectionType::None;
  }

  bool connectNetwork() {
    switch (connectionType) {
      case BlynkConnectionType::WiFi:
        return connectWiFi();
      case BlynkConnectionType::Gsm:
        return connectGsm();
    }

    return false;
  }

  void begin() {
    while (!connectNetwork() && !connect()) {}
  }

  BlynkConnectionType getConnectionType() const {
    return connectionType;
  }

private:
  struct WiFiData {
    const char* ssid;
    const char* pass;
  };

  struct GsmData {
    TinyGsm* modem;
    const char* apn;
    const char* user;
    const char* pass;
  };

  bool connectWiFi() {
    BLYNK_LOG2(BLYNK_F("Connecting to "), wifiData.ssid);
    WiFi.mode(WIFI_STA);

    if (wifiData.pass && strlen(wifiData.pass)) {
      WiFi.begin(wifiData.ssid, wifiData.pass);
    } else {
      WiFi.begin(wifiData.ssid);
    }

    // Wait 10 seconds connect to WiFi
    uint8_t delayCounter = 20;
    while ((delayCounter != 0) && (WiFi.status() != WL_CONNECTED)) {
      BlynkDelay(500);
      --delayCounter;
    }

    if ((WiFi.status() != WL_CONNECTED)) {
      BLYNK_LOG1(BLYNK_F("Cannot connect to WiFi"));
      return false;
    }

    IPAddress myip = WiFi.localIP();
    (void)myip;  // Eliminate warnings about unused myip
    BLYNK_LOG_IP("IP: ", myip);

    return true;
  }

  bool connectGsm() {
    BLYNK_LOG1(BLYNK_F("Modem init..."));

    switch (gsmData.modem->getSimStatus()) {
      case SIM_ERROR:
        BLYNK_LOG1(BLYNK_F("SIM is missing"));
        return false;
      case SIM_LOCKED:
        BLYNK_LOG1(BLYNK_F("SIM is PIN-locked"));
        return false;
    }

    BLYNK_LOG1(BLYNK_F("Connecting to network..."));
    if (gsmData.modem->waitForNetwork(60000)) {
      String op = gsmData.modem->getOperator();
      BLYNK_LOG2(BLYNK_F("Network: "), op);
    } else {
      BLYNK_LOG1(BLYNK_F("Register in network failed"));
      return false;
    }

    BLYNK_LOG3(BLYNK_F("Connecting to "), gsmData.apn, BLYNK_F(" ..."));
    if (!gsmData.modem->gprsConnect(gsmData.apn, gsmData.user, gsmData.pass)) {
      BLYNK_LOG1(BLYNK_F("Connect Gsm failed"));
      return false;
    }

    BLYNK_LOG1(BLYNK_F("Connected to Gsm"));

    return true;
  }

  void disconnectWiFi() {
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    BLYNK_LOG1(BLYNK_F("WiFi desconnected"));
  }

  void disconnectGsm() {
    gsmData.modem->poweroff();
    BLYNK_LOG1(BLYNK_F("Gsm desconnected"));
  }

  union {
    WiFiData wifiData;
    GsmData gsmData;
  };

  BlynkConnectionType connectionType = BlynkConnectionType::None;
  WiFiClientSecure wifiClientSecure;
  TinyGsmClient gsmClient;
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_BLYNK)
static BlynkArduinoClient _blynkTransport;
BlynkEsp32WiFiAndGsm Blynk(_blynkTransport);
#else
extern BlynkEsp32WiFiAndGsm Blynk;
#endif

#include <BlynkWidgets.h>

#endif