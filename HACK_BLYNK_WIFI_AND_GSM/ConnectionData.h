#ifndef CONNECTION_H
#define CONNECTION_H

#include <Blynk.h>
#include <WiFi.h>
#include <TinyGsmClient.h>

// Base class for the connection data
class ConnectionData {
public:
  inline ConnectionData() = default;

  inline ConnectionData(const char* auth,
                    const char* pass,
                    const char* domain,
                    uint16_t port)
    : auth(auth), pass(pass), domain(domain), port(port) {
  }

  inline ConnectionData(const char* auth,
                    const char* pass,
                    IPAddress ip,
                    uint16_t port = BLYNK_DEFAULT_PORT)
    : auth(auth), pass(pass), ip(ip), port(port) {
  }

  inline ConnectionData(const String& auth,
                    const String& pass,
                    const String& domain,
                    uint16_t port)
    : auth(auth), pass(pass), domain(domain), port(port) {
  }

  inline ConnectionData(const String& auth,
                    const String& pass,
                    IPAddress ip,
                    uint16_t port = BLYNK_DEFAULT_PORT)
    : auth(auth), pass(pass), ip(ip), port(port) {
  }

  String auth;
  String pass;
  String domain = BLYNK_DEFAULT_DOMAIN;
  IPAddress ip;
  uint16_t port = BLYNK_DEFAULT_PORT;
};

// WiFi connection data
class WiFiData : public ConnectionData {
public:
  inline WiFiData() = default;

  inline WiFiData(const char* auth,
                        const char* ssid,
                        const char* pass = "",
                        const char* domain = BLYNK_DEFAULT_DOMAIN,
                        uint16_t port = BLYNK_DEFAULT_PORT)
    : ConnectionData(auth, pass, domain, port), ssid(ssid) {
  }

  inline WiFiData(const char* auth,
                        const char* ssid,
                        const char* pass,
                        IPAddress ip,
                        uint16_t port = BLYNK_DEFAULT_PORT)
    : ConnectionData(auth, pass, ip, port), ssid(ssid) {
  }

  inline WiFiData(const String& auth,
                        const String& ssid,
                        const String& pass = "",
                        const String& domain = BLYNK_DEFAULT_DOMAIN,
                        uint16_t port = BLYNK_DEFAULT_PORT)
    : ConnectionData(auth, pass, domain, port), ssid(ssid) {
  }

  inline WiFiData(const String& auth,
                        const String& ssid,
                        const String& pass,
                        IPAddress ip,
                        uint16_t port = BLYNK_DEFAULT_PORT)
    : ConnectionData(auth, pass, ip, port), ssid(ssid) {
  }

  String ssid;
};

// GSM connection data
class GsmData : public ConnectionData {
public:
  inline GsmData() = default;

  inline GsmData(const char* auth,
                       TinyGsm& modem,
                       const char* apn,
                       const char* user = "",
                       const char* pass = "",
                       const char* domain = BLYNK_DEFAULT_DOMAIN,
                       uint16_t port = BLYNK_DEFAULT_PORT)
    : ConnectionData(auth, pass, domain, port), modem(&modem), apn(apn), user(user) {
  }

  inline GsmData(const char* auth,
                       TinyGsm& modem,
                       const char* apn,
                       const char* user,
                       const char* pass,
                       IPAddress ip,
                       uint16_t port = BLYNK_DEFAULT_PORT)
    : ConnectionData(auth, pass, ip, port), modem(&modem), apn(apn), user(user) {
  }

  inline GsmData(const String& auth,
                       TinyGsm& modem,
                       const String& apn,
                       const String& user = "",
                       const String& pass = "",
                       const String& domain = BLYNK_DEFAULT_DOMAIN,
                       uint16_t port = BLYNK_DEFAULT_PORT)
    : ConnectionData(auth, pass, domain, port), modem(&modem), apn(apn), user(user) {
  }

  inline GsmData(const String& auth,
                       TinyGsm& modem,
                       const String& apn,
                       const String& user,
                       const String& pass,
                       IPAddress ip,
                       uint16_t port = BLYNK_DEFAULT_PORT)
    : ConnectionData(auth, pass, ip, port), modem(&modem), apn(apn), user(user) {
  }

  TinyGsm* modem = nullptr;
  String apn;
  String user;
};

#endif