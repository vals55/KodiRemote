// Copyright 2024 vals. All rights reserved.
// SPDX-License-Identifier: MIT

#include <Arduino.h>
#include <rlog.h>
#include <WakeOnLan.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "config.h"
#include "data.h"
#include "setup.h"
#include "NecDecoder.h"
#include "http.h"
#include "wifi.h"
#include "buffer.h"
#include "utils.h"

//#define OTA_DISABLE
#ifndef OTA_DISABLE
  #include <ArduinoOTA.h>
#endif

#include <Pinger.h>
extern "C" {
  #include <lwip/icmp.h> // needed for icmp packet definitions
}

#define SETUP_LED 16

// Set global to avoid object removing after setup() routine
Pinger pinger;

NecDecoder ir;
WiFiUDP UDP;
WakeOnLan WOL(UDP);
BoardConfig conf;
Button btn;
EEPROMBuff<BoardConfig> storage(8);
String host;
bool ping = false;
bool alive = false;

IRAM_ATTR void irIsr() {
  ir.tick();
}

void flashLED(int pause) {
  digitalWrite(SETUP_LED, HIGH);
  delay(pause);
  digitalWrite(SETUP_LED, LOW);
}

void wakeKodi() {

  flashLED(300);
  
  rlog_i("info Kodi", "Send WOL command");
  WOL.sendMagicPacket(conf.MAC); // Send Wake On Lan packet with the above MAC address. Default to port 9.
  // WOL.sendMagicPacket(MAC, 7); // Change the port number
}

void Command(int ndx) {
  rlog_i("info Command", "url=%s Command = %s", conf.url, msg_rpc[ndx]);
  sendHTTP(conf.url, msg_rpc[ndx]);
}

void Click() {

  flashLED(10);

  int count = sizeof(cmd);
  int ndx = -1;
  for(int i=0; i<count; i++) {
    if(btn.address == 0x20 && cmd[i] == btn.command) {
      ndx = i;
      break;
    }
    if(btn.address == 0x0 && cmd_magic[i] == btn.command) {
      ndx = i;
      break;
    }
  }
  rlog_i("info Click", "Index = %d Count=%d", ndx, count);

  if(ndx >= count-2) {                  //two last cmd
    pinger.Ping(host, 1);
  } else if(ndx != -1) {                //cmd found
    Command(ndx);
  }
}

void setupBoard() {
  
  WiFi.persistent(false);
  WiFi.disconnect();
  WiFi.setSleepMode(WIFI_NONE_SLEEP);

  wifiSetMode(WIFI_AP_STA);
  delay(1000);

  startAP(conf);

  wifiShutdown();
  
  rlog_i("info", "Restart ESP");
  ESP.restart();
}

void setup() {
#ifdef DEBUG_INFO
  Serial.begin(115200);
  delay(1000);
  Serial.println();
#endif

  pinMode(SETUP_LED, OUTPUT);
  digitalWrite(SETUP_LED, LOW);

  rlog_i("info", "Start OK");
  bool success = loadConfig(conf);
  rlog_i("info", "loadConfig = %d", success);
  if (!success) {
    rlog_i("info", "Setup board entering");
    setupBoard();
  }
  success = wifiConnect(conf);
  rlog_i("info", "WiFi connect = %d", success);

  if (!success) {
    rlog_i("info", "Setup board entering");
    setupBoard();
  }

  if (WiFi.status() == WL_CONNECTED) {
    rlog_i("info WiFi", "Connected.");
#ifndef OTA_DISABLE
    ArduinoOTA.begin();
#endif
  } else if (WiFi.status() == WL_WRONG_PASSWORD) {
    setupBoard();
  }

  attachInterrupt(14, irIsr, FALLING);
  WOL.setRepeat(3, 100); // Optional, repeat the packet three times with 100ms between. WARNING delay() is used between send packet function.
  WOL.calculateBroadcastAddress(WiFi.localIP(), WiFi.subnetMask()); // Optional  => To calculate the broadcast address, otherwise 255.255.255.255 is used (which is denied in some networks).

  host = conf.url;
  getHost(host);
  rlog_i("info HOST", "Host = %s", host.c_str());

  pinger.OnReceive([](const PingerResponse& response) {
    if (response.ReceivedResponse) {
      rlog_i("info PING", "Reply from %s: bytes=%d time=%lums TTL=%d\n",
        response.DestIPAddress.toString().c_str(),
        response.EchoMessageSize - sizeof(struct icmp_echo_hdr),
        response.ResponseTime,
        response.TimeToLive);
      
      alive = true;
    } else {
      rlog_i("info PING", "Request timed out.\n");
      alive = false;
    }
    pinger.StopPingSequence();
    ping = true;
    return true;
  });
}

bool taboo = false;

void loop() {

#ifndef OTA_DISABLE
  ArduinoOTA.handle();
#endif

  if (ir.available() && !taboo) { 
    taboo = true;     
    btn.value = ir.readPacket();
    btn.data = ir.readData();
    btn.address = ir.readAddress();
    btn.command = ir.readCommand();
    btn.repeat = ir.isRepeated();
  
    rlog_i("info IR", "Packet = 0x%s", int2String(btn.value, HEX));
    rlog_i("info IR", "Data = 0x%s", int2String(btn.data, HEX));
    rlog_i("info IR", "Address = 0x%s", int2String(btn.address, HEX));
    rlog_i("info IR", "Command = 0x%s", int2String(btn.command, HEX));
    rlog_i("info IR", "Repeat = %d", btn.repeat);

    Click();
    taboo = false;
  }

  if(ping) {
    if(alive) {
      Command(9);
    } else {
      wakeKodi();      
    }
    ping = false;
  }
}
