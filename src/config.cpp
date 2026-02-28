// Copyright 2024 vals. All rights reserved.
// SPDX-License-Identifier: MIT

#include <Arduino.h>
#include <IPAddress.h>
#include "config.h"
#include <rLog.h>

extern EEPROMBuff<BoardConfig> storage;

bool testConfig(BoardConfig &conf) {
    
    BoardConfig test;
    return storage.get(test);
}

void storeConfig(const BoardConfig &conf) {

    storage.add(conf);
}

bool loadConfig(BoardConfig &conf) {
    
    if (storage.get(conf)) {
        conf.url[KODI_HOST_LEN - 1] = 0;
        return true;
    } else {
        // Конфигурация не была сохранена в EEPROM, инициализируем с нуля
        conf.version = VERSION;
        String kodi_url(DEFAULT_KODI_HOST);
        strncpy0(conf.url, kodi_url.c_str(), kodi_url.length() + 1);
        String mac(DEFAULT_KODI_MAC);
        strncpy0(conf.MAC, mac.c_str(), mac.length() + 1);
        String bssid(17);
        strncpy0(conf.bssid, bssid.c_str(), bssid.length() + 1);

        conf.ip = 0;
        IPAddress network_gateway;
        network_gateway.fromString(DEFAULT_GATEWAY);
        conf.gateway = network_gateway;
        IPAddress network_mask;
        network_mask.fromString(DEFAULT_MASK);
        conf.mask = network_mask;
        conf.wifi_channel = 0;
        conf.wifi_phy_mode = 0;
        conf.wifi_bssid[0] = 0;
        return false;
    }
}

