# KodiRemote
A compact IR receiver for controlling the Kodi media player with a TV remote.
Uses the ESP8266 Nodemcu module and any 38 kHz IR receiver, such as the TSOP-4838.
Connects to the same network as Kodi via WiFi, turns the media player on/off with the WOL command, and controls the media center via the JSON-RPC API.