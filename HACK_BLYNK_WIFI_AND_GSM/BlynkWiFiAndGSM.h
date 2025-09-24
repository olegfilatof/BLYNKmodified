#ifndef BLYNK_WIFI_AMD_MODEM_H
#define BLYNK_WIFI_AMD_MODEM_H

#ifndef ESP32
#error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.
#endif

#include "ConnectionData.h"

#include <Blynk/BlynkProtocol.h>
#include <Adapters/BlynkArduinoClient.h>
#include <WiFiClient.h>

#define BLYNK_SEND_ATOMIC

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

class BlynkWiFiAndGSM
  : public BlynkProtocol<BlynkArduinoClient> {
	typedef BlynkProtocol<BlynkArduinoClient> Base;
public:
	enum class ConnectionType {
		None,
		WiFi,
		GSM
	};

	BlynkWiFiAndGSM(BlynkArduinoClient& transp)
	  : Base(transp) {
	}

	~BlynkWiFiAndGSM() {
		removeConnection();
	}

	// Remove the old connection and config the new
	void setConnectionData(WiFiData& wifiData) {
		// Remove the old data firstly
		removeConnection();

		// Set the new connection
		connectionData = &wifiData;
		connectionType = ConnectionType::WiFi;

		// Config Blynk depending on the host identifier (domain/ip)
		if (wifiData.ip)
			configWiFi(wifiData.auth.c_str(), wifiData.ip, wifiData.port);
		else
			configWiFi(wifiData.auth.c_str(), wifiData.domain.c_str(), wifiData.port);
	}

	// Remove the old connection and config the new
	void setConnectionData(GsmData& gsmData) {
		// Remove the old data firstly
		removeConnection();

		if (!gsmData.modem) {
			BLYNK_LOG1(BLYNK_F("Pointer to modem is empty. Set the TinyGsm modem to continue"));
			return;
		}

		// Set the new connection
		connectionData = &gsmData;
		connectionType = ConnectionType::GSM;

		// Config Blynk depending on the host identifier (domain/ip)
		if (gsmData.ip)
			configGSM(*gsmData.modem, gsmData.auth.c_str(), gsmData.ip, gsmData.port);
		else
			configGSM(*gsmData.modem, gsmData.auth.c_str(), gsmData.domain.c_str(), gsmData.port);
	}

	void removeConnection() {
		// Skip if there no last data
		if (!connectionData)
			return;

		// Disconnect Blynk
		Base::disconnect();

		// Desconnect connection depending on the connection type
		switch (connectionType) {
			case ConnectionType::WiFi:
				wifiClient.stop();
				disconnectWiFi(static_cast<WiFiData&>(*connectionData));
				break;
			case ConnectionType::GSM:
				gsmClient.stop();
				disconnectGsm(static_cast<GsmData&>(*connectionData));
				break;
		}

		// Reset connection
		connectionData = nullptr;
		connectionType = ConnectionType::None;
	}

	ConnectionType getConnectionType() const {
		return connectionType;
	}

	bool connectNetwork() {
		switch (connectionType) {
			case ConnectionType::WiFi:
				return connectWiFi(static_cast<WiFiData&>(*connectionData));
			case ConnectionType::GSM:
				return connectGsm(static_cast<GsmData&>(*connectionData));
		}

		return false;
	}

	void begin() {
		connectNetwork();
		while (this->connect() != true) {}
	}

private:
	void configWiFi(const char* auth,
	                const char* domain,
	                uint16_t port) {
		Base::begin(auth);
		this->conn.setClient(&wifiClient);
		this->conn.begin(domain, port);
	}

	void configWiFi(const char* auth,
	                IPAddress ip,
	                uint16_t port) {
		Base::begin(auth);
		this->conn.setClient(&wifiClient);
		this->conn.begin(ip, port);
	}

	void configGSM(TinyGsm& modem,
	               const char* auth,
	               const char* domain,
	               uint16_t port) {
		Base::begin(auth);
		gsmClient.init(&modem);
		this->conn.setClient(&gsmClient);
		this->conn.begin(domain, port);
	}

	void configGSM(TinyGsm& modem,
	               const char* auth,
	               IPAddress ip,
	               uint16_t port) {
		Base::begin(auth);
		gsmClient.init(&modem);
		this->conn.setClient(&gsmClient);
		this->conn.begin(ip, port);
	}

	bool connectWiFi(WiFiData& wifiData) {
		BLYNK_LOG2(BLYNK_F("Connecting to "), wifiData.ssid);
		WiFi.mode(WIFI_STA);
		if (WiFi.status() != WL_CONNECTED) {
			if (!wifiData.pass.isEmpty()) {
				WiFi.begin(wifiData.ssid.c_str(), wifiData.pass.c_str());
			} else {
				WiFi.begin(wifiData.ssid.c_str());
			}
		}

		uint8_t count = 10;
		while ((count != 0) || (WiFi.status() != WL_CONNECTED)) {
			BlynkDelay(500);
			--count;
		}

		if ((WiFi.status() != WL_CONNECTED)) {
			BLYNK_LOG1(BLYNK_F("Cannot connect to WiFi"));
			return false;
		}

		BLYNK_LOG1(BLYNK_F("Connected to WiFi"));

		IPAddress myip = WiFi.localIP();
		(void)myip;  // Eliminate warnings about unused myip
		BLYNK_LOG_IP("IP: ", myip);

		return true;
	}

	bool connectGsm(GsmData& gsmData) {
		BLYNK_LOG1(BLYNK_F("Modem init..."));
		if (!gsmData.modem->begin()) {
			BLYNK_FATAL(BLYNK_F("Cannot init"));
		}

		switch (gsmData.modem->getSimStatus()) {
			case SIM_ERROR: BLYNK_FATAL(BLYNK_F("SIM is missing")); break;
			case SIM_LOCKED: BLYNK_FATAL(BLYNK_F("SIM is PIN-locked")); break;
			default: break;
		}

		BLYNK_LOG1(BLYNK_F("Connecting to network..."));
		if (gsmData.modem->waitForNetwork()) {
			String op = gsmData.modem->getOperator();
			BLYNK_LOG2(BLYNK_F("Network: "), op);
		} else {
			BLYNK_FATAL(BLYNK_F("Register in network failed"));
		}

		BLYNK_LOG3(BLYNK_F("Connecting to "), gsmData.apn.c_str(), BLYNK_F(" ..."));
		if (!gsmData.modem->gprsConnect(gsmData.apn.c_str(), gsmData.user.c_str(), gsmData.pass.c_str())) {
			BLYNK_FATAL(BLYNK_F("Connect GSM failed"));
			return false;
		}

		BLYNK_LOG1(BLYNK_F("Connected to GSM"));
		return true;
	}

	void disconnectWiFi(WiFiData& wifiData) {
		WiFi.disconnect();
		WiFi.mode(WIFI_OFF);
		BLYNK_LOG1(BLYNK_F("WiFi desconnected"));
	}

	void disconnectGsm(GsmData& gsmData) {
		gsmData.modem->poweroff();
		BLYNK_LOG1(BLYNK_F("GPRS desconnected"));
	}

	WiFiClient wifiClient;
	TinyGsmClient gsmClient;
	ConnectionData* connectionData = nullptr;
	ConnectionType connectionType = ConnectionType::None;
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_BLYNK)
static BlynkArduinoClient _blynkTransport;
BlynkWiFiAndGSM Blynk(_blynkTransport);
#else
extern BlynkWiFiAndGSM Blynk;
#endif

#include <BlynkWidgets.h>

#endif