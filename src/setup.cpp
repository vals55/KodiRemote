// Copyright 2025 vals. All rights reserved.
// SPDX-License-Identifier: MIT
#include <WiFiManager.h>

#include "setup.h"
#include "rlog.h"
#include "utils.h"

WiFiManager wm;
bool shouldSaveConfig = false;

#define SETUP_TIME_SEC 300

void saveConfigCallback () {
  shouldSaveConfig = true;
}

void wifiInfo() {
  // can contain gargbage on esp32 if wifi is not ready yet
  rlog_i("info", "[wifiInfo] WIFI INFO DEBUG");
  rlog_i("info", "[wifiInfo] SAVED: %s", (String)(wm.getWiFiIsSaved() ? "YES" : "NO"));
  rlog_i("info", "[wifiInfo] SSID: %s", (String)wm.getWiFiSSID());
  rlog_i("info", "[wifiInfo] PASS: %s", (String)wm.getWiFiPass());
  rlog_i("info", "[wifiInfo] HOSTNAME: %s", (String)WiFi.getHostname());
}

void startAP(BoardConfig &conf) {

#ifdef WIFI_DEBUG_INFO
  rlog_i("info", "WiFi debug info enabled");
  wm.setDebugOutput(true);
  wm.debugPlatformInfo();

  //reset settings - for testing
  // wm.resetSettings();    
  // wm.erase();
#endif

  if (conf.ssid[0]) {
      struct station_config sconf;
      sconf.bssid_set = 0;
      memcpy(sconf.ssid, conf.ssid, sizeof(sconf.ssid));
      if (conf.password[0]) {
          memcpy(sconf.password, conf.password, sizeof(sconf.password));
      } else {
          sconf.password[0] = 0;
      }    
      wifi_station_set_config(&sconf);
  }

  WiFiManagerParameter subtitle_kodi("<h3>KODI</h3>");
  wm.addParameter(&subtitle_kodi);

  WiFiManagerParameter param_kodi_url("url", "URL Kodi:", conf.url, KODI_HOST_LEN-1);
  wm.addParameter(&param_kodi_url);

  WiFiManagerParameter param_kodi_mac("MAC", "MAC адрес:", conf.MAC, 18);
  wm.addParameter(&param_kodi_mac);
  
//extra conf ------------------------------------------------------------------------------------
  WiFiManagerParameter checkbox("<input type='checkbox' id='chbox' name='chbox' onclick='extraConf()'><label for='chbox'>Расширенные настройки</label>");
  wm.addParameter(&checkbox);
  WiFiManagerParameter div_start("<div id='extra_conf' style='display:none'>");
  wm.addParameter(&div_start);

  WiFiManagerParameter subtitle_network("<h3>Сетевые настройки</h3>");
  wm.addParameter(&subtitle_network);

  String mac("<label>MAC адрес: ");
  mac += WiFi.macAddress();
  mac += "</label><br /><br />";
  WiFiManagerParameter label_mac(mac.c_str());
  wm.addParameter(&label_mac);

  IPAddressParameter param_ip("ip", "Статический IP (оставьте 0.0.0.0 для DHCP):", IPAddress(conf.ip));
  wm.addParameter(&param_ip);
  IPAddressParameter param_gateway("gateway", "Шлюз:", IPAddress(conf.gateway));
  wm.addParameter(&param_gateway);
  IPAddressParameter param_mask("mask", "Маска подсети:", IPAddress(conf.mask));
  wm.addParameter(&param_mask);

  WiFiManagerParameter label_phy_mode("<label>Режим работы WiFi:</label>");
  wm.addParameter(&label_phy_mode);
  DropdownParameter dropdown_phy_mode("wifi_phy_mode");
  dropdown_phy_mode.add_option(0, "Авто", conf.wifi_phy_mode);
  dropdown_phy_mode.add_option(1, "11b",  conf.wifi_phy_mode);
  dropdown_phy_mode.add_option(2, "11g",  conf.wifi_phy_mode);
  dropdown_phy_mode.add_option(3, "11n",  conf.wifi_phy_mode);
  wm.addParameter(&dropdown_phy_mode);

//extra conf end---------------------------------------------------------------------------------
  WiFiManagerParameter div_end("</div>");
  wm.addParameter(&div_end);

  wm.setSaveConfigCallback(saveConfigCallback);

  // set custom channel
  // wm.setWiFiAPChannel(13);
  
  // set AP hidden
  // wm.setAPHidden(true);

  wm.setConfigPortalTimeout(SETUP_TIME_SEC);
  wm.setBreakAfterConfig(true);
  WiFi.setPhyMode(WIFI_PHY_MODE_11N);
  // set custom webserver port, automatic captive portal does not work with custom ports!
  // wm.setHttpPort(8080);

  //bool result = wm.startConfigPortal(getAppName().c_str());
  // if (!wm.autoConnect(getAppName().c_str())) {
  if (!wm.startConfigPortal(getAppName().c_str())) {
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    // ESP.restart();
    // delay(5000);
  }

#ifdef WIFI_DEBUG_INFO
   wifiInfo();
#endif  
    
  strncpy0(conf.ssid, wm.getWiFiSSID().c_str(), SSID_LEN);
  strncpy0(conf.password, wm.getWiFiPass().c_str(), PASSW_LEN);

  strncpy0(conf.url, param_kodi_url.getValue(), KODI_HOST_LEN);
  strncpy0(conf.MAC, param_kodi_mac.getValue(), 18);

  conf.ip = param_ip.getValue();
  if (conf.ip) {
    conf.gateway = param_gateway.getValue();
    conf.mask = param_mask.getValue();
  }
  conf.wifi_phy_mode = dropdown_phy_mode.getValue();

  if (shouldSaveConfig) {
    storeConfig(conf);
  }
}