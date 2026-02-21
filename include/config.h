// Copyright 2025 vals. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include <Arduino.h>
#include <IPAddress.h>
#include "buffer.h"
#include <rLog.h>

#define VERSION 1
#define FIRMWARE_VERSION "0.1"
#define HARDWARE_VERSION "0.1"
#define BRAND_NAME "KodiRemote"

#define SSID_LEN 32
#define PASSW_LEN 64

#define KODI_HOST_LEN 64
#define DEFAULT_KODI_HOST "http://kodi:kodi@192.168.254.10:8080/jsonrpc"
#define DEFAULT_KODI_MAC "E8:FF:1E:D7:0D:CF"

#define DEFAULT_GATEWAY "192.168.0.1"
#define DEFAULT_MASK "255.255.255.0"

struct Button {
  uint32_t value;
  uint16_t data;
  uint8_t address;
  uint8_t command;
  bool repeat;
};

struct BoardConfig {
  uint8_t version = VERSION;

  char ssid[SSID_LEN] = {0}; 
  char password[PASSW_LEN] = {0};
  char url[KODI_HOST_LEN] = {0};
  char MAC[18] = {0};
  char bssid[18] = {0};

  uint32_t ip;
  uint32_t gateway;
  uint32_t mask;

  uint8_t wifi_bssid[6] = {0};
  uint8_t wifi_channel = 0;
  uint8_t wifi_phy_mode = 3;
};  

inline void strncpy0(char *dest, const char *src, const size_t len) {
  strncpy(dest, src, len - 1);
  dest[len - 1] = 0;
}

bool loadConfig(BoardConfig &conf);
bool testConfig(BoardConfig &conf);
void storeConfig(const BoardConfig &conf);
