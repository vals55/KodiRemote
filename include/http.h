// Copyright 2025 vals. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "rlog.h"
#ifdef DEBUG_INFO
#define DEBUG_HTTPCLIENT(fmt, ...) _rlog_printf(fmt, ##__VA_ARGS__)
#endif

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define HTTP_SEND_ATTEMPTS 1
#define SERVER_TIMEOUT 1000
#define PROTO_HTTPS "https"
#define PROTO_HTTP "http"

String getProtocol(const String &url) {
	String proto = PROTO_HTTP;
	int index = url.indexOf(':');
	if (index > 0) {
		proto = url.substring(0, index);
		proto.toLowerCase();
	}
	return proto;
}

bool postData(const String &url, String &payload) {
  void *pClient = nullptr;
  HTTPClient httpClient;
  bool result = false;
  
  String proto = getProtocol(url);
  if (proto == PROTO_HTTP) {
    pClient = new WiFiClient;
  } else if (proto == PROTO_HTTPS) {
    pClient = new WiFiClientSecure;
    (*(WiFiClientSecure *)pClient).setInsecure();
  }

  httpClient.setTimeout(SERVER_TIMEOUT);
  httpClient.setReuse(false);

  if (httpClient.begin(*(WiFiClient *)pClient, url)) {
    httpClient.addHeader(F("Content-Type"), F("application/json"));
  
    int response_code = httpClient.POST(payload);
    result = response_code == 200;
    String response_body = httpClient.getString();
    String error = httpClient.errorToString(response_code);
    rlog_i("info HTTP", "Response. code=%d body=%s error=%s", response_code, response_body.c_str(), error.c_str());
    httpClient.end();
    (*(WiFiClient *)pClient).stop();
  }
  delete (WiFiClient*)pClient;
  return result;
}

bool sendHTTP(const String &url, const String &jsonrpc) {
#ifdef DEBUG_INFO
  uint32_t start_time = millis();
#endif  
  String payload = jsonrpc;

  rlog_i("info HTTP", "url=%s rpc=%s", url.c_str(), jsonrpc.c_str());
  int attempts = HTTP_SEND_ATTEMPTS;
  bool result = false;
  do {
    result = postData(url, payload);
    rlog_i("info HTTP", "Attempt. Time %d ms", millis() - start_time);
  } while (!result && --attempts);

  if (result) {
    rlog_i("info HTTP", "Data sent. Time %d ms", millis() - start_time);
  } else {
     rlog_i("info HTTP", "Failed send. Time %d ms", millis() - start_time);
  }
  return result;
}

