// Copyright 2024 vals. All rights reserved.
// SPDX-License-Identifier: MIT

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "utils.h"

uint16_t getCRC(const BoardConfig &conf) {
	uint8_t *buf = (uint8_t *)&conf;
	uint16_t crc = 0xffff, poly = 0xa001;
	uint16_t i = 0;
	uint16_t len = sizeof(conf);

	for (i = 0; i < len; i++) {
		crc ^= buf[i];
		for (uint8_t j = 0; j < 8; j++) {
			if (crc & 0x01) {
				crc >>= 1;
				crc ^= poly;
			} else {
				crc >>= 1;
			}
		}
	}
	return crc;
}

String getDeviceName() {
	String deviceName = String(BRAND_NAME) + "-" + ESP.getChipId();
	return deviceName;
}

String getAppName() {
	return getDeviceName() + "-" + String(FIRMWARE_VERSION);
}

void removeSlash(String &str) {
	if (str.endsWith(F("/"))) {
		str.remove(str.length() - 1);
	}
}

String getMacAddressHex() {
	uint8_t baseMac[6];
	char baseMacChr[13] = {0};
	WiFi.macAddress(baseMac);
	sprintf(baseMacChr, MAC_STR_HEX, baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
	return String(baseMacChr);
}

bool isDHCP(const BoardConfig &conf) {
	return conf.ip != 0;
}

String int2String(uint64_t input, uint8_t base) {
	String result = "";
	if (base < 2) base = 10;
	if (base > 36) base = 10;
  
	result.reserve(16);
  
	do {
	  char c = input % base;
	  input /= base;
  
	  if (c < 10)
		c += '0';
	  else
		c += 'A' - 10;
	  result = c + result;
	} while (input);
	return result;
}
  
//"http://kodi:kodi@192.168.254.10:8080/jsonrpc"
void getHost(String &url) { 
	
	int prot = url.indexOf('/') + 1;
	int begin = url.indexOf('@', prot);
	if(begin < 0) {
		begin = prot;
	}
	int end = url.indexOf(':', begin);
	if(end < 0) {
		end = url.indexOf('/', begin);
	}
	if(end > 0) {
		url.remove(end);
	}
	url.remove(0, begin + 1);
}	