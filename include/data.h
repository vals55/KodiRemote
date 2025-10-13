// Copyright 2025 vals. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include <Arduino.h>

const char *msg_rpc[] = {
    "{\"jsonrpc\":\"2.0\", \"method\":\"Input.Home\", \"id\":1}",
    "{\"jsonrpc\":\"2.0\", \"method\":\"Input.Back\", \"id\":1}",
    "{\"jsonrpc\":\"2.0\", \"method\":\"Input.Down\", \"id\":1}",
    "{\"jsonrpc\":\"2.0\", \"method\":\"Input.Up\", \"id\":1}",
    "{\"jsonrpc\":\"2.0\", \"method\":\"Input.Left\", \"id\":1}",
    "{\"jsonrpc\":\"2.0\", \"method\":\"Input.Right\", \"id\":1}",
    "{\"jsonrpc\":\"2.0\", \"method\":\"Input.Select\", \"id\":1}",
    "{\"jsonrpc\":\"2.0\", \"method\":\"Input.Info\", \"id\":1}",
    "{\"jsonrpc\":\"2.0\", \"method\":\"Input.ContextMenu\", \"id\":1}",
    
    "{\"jsonrpc\":\"2.0\", \"method\":\"System.Shutdown\", \"id\":1}",
    "{\"jsonrpc\":\"2.0\", \"method\":\"System.Reboot\", \"id\":1}",
    
    "{\"jsonrpc\":\"2.0\", \"method\":\"Application.SetMute\", \"id\":1, \"params\":{\"mute\":\"toggle\"}}",
    "{\"jsonrpc\":\"2.0\", \"method\":\"Application.SetVolume\", \"id\":1, \"params\":{\"volume\":\"increment\"}}",
    "{\"jsonrpc\":\"2.0\", \"method\":\"Application.SetVolume\", \"id\":1, \"params\":{\"volume\":\"decrement\"}}",
    ""
};

const u_int8_t cmd[] {
    0x0,  // Input.Home
    0x14,  // Input.Back
    0x82,  // Input.Down
    0x02,  // Input.Up
    0xE0,  // Input.Left
    0x60,  // Input.Right
    0x22,  // Input.Select
    0x55,  // Input.Info
    0x98,  // Input.ContextMenu
    
    0x10,  // System.Shutdown,
    0x0,  // System.Reboot
    
    0x90,  // Application.SetMute
    0x40,  // Application.SetVolume
    0xC0,  // Application.SetVolume
    0x9C,  // WOL (subtitle)
    0x3E   // WOL (Home on air)
};

const u_int8_t cmd_magic[] {
  0x0,  // Input.Home
  0x40,  // Input.Back
  0xF0,  // Input.Down
  0xD0,  // Input.Up
  0x92,  // Input.Left
  0x52,  // Input.Right
  0xB0,  // Input.Select
  0xD8,  // Input.Info
  0xF8,  // Input.ContextMenu
  
  0x0,   // System.Shutdown,
  0x0,   // System.Reboot
  
  0x0,   // Application.SetMute
  0x0,   // Application.SetVolume
  0x0,   // Application.SetVolume
  0xA8,  // WOL (channel 1)
  0x3E   // WOL (Home on air)
};
