// Copyright 2025 vals. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include <Arduino.h>
#include "config.h"

#define MAC_STR "%02X:%02X:%02X:%02X:%02X:%02X"
#define MAC_STR_HEX "%02X%02X%02X%02X%02X%02X"
#define PROTO_HTTPS "https"
#define PROTO_HTTP "http"

uint16_t getCRC(const BoardConfig &conf);
String getDeviceName();
String getAppName();
void removeSlash(String &str);
String getMacAddressHex();
bool isDHCP(const BoardConfig &conf);
String int2String(uint64_t input, uint8_t base);
void getHost(String &url); 
