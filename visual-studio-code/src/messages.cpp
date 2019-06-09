
#include <Arduino.h>
#include "messages.h"

// message text
char g_message[MSG_LAST][22] = {
  "PLEASE WAIT!",
  "READY",
  "PUMP STARTED",
  "PUMP STOPPED",
  "PUMP TRIPPED",
  "TANK FILLED",
  "WATER LEVEL LOW",
  "VOLTAGE IS LOW",
  "OVER-CURRENT",
};

// corresponding message lines
byte g_message_line[MSG_LAST] = {
  MSG_LINE_1, // MSG_PLEASE_WAIT
  MSG_LINE_1, // MSG_READY
  MSG_LINE_1, // MSG_PUMP_STARTED
  MSG_LINE_1, // MSG_PUMP_STOPPED
  MSG_LINE_1, // MSG_PUMP_TRIPPED
  MSG_LINE_2, // MSG_TANK_FILLED
  MSG_LINE_2, // MSG_WATER_LEVEL_LOW
  MSG_LINE_2, // MSG_VOLTAGE_LOW
  MSG_LINE_2, // MSG_OVER_CURRENT_TRIPPED
};

// corresponding message timeouts
unsigned long g_message_timeout[MSG_LAST] = {
  TIMEOUT_NIL,      // MSG_PLEASE_WAIT
  TIMEOUT_NIL,      // MSG_READY
  TIMEOUT_NIL,      // MSG_PUMP_STARTED
  TIMEOUT_10_MIN,   // MSG_PUMP_STOPPED
  TIMEOUT_10_MIN,   // MSG_PUMP_TRIPPED
  TIMEOUT_NIL,      // MSG_TANK_FILLED
  TIMEOUT_NIL,      // MSG_WATER_LEVEL_LOW
  TIMEOUT_NIL,      // MSG_VOLTAGE_LOW
  TIMEOUT_10_MIN,   // MSG_OVER_CURRENT_TRIPPED
};

// corresponding message colors
Color g_message_color[MSG_LAST] = {
  Color::Yellow,    // MSG_PLEASE_WAIT
  Color::White,     // MSG_READY
  Color::White,     // MSG_PUMP_STARTED
  Color::White,     // MSG_PUMP_STOPPED
  Color::White,     // MSG_PUMP_TRIPPED
  Color::White,     // MSG_TANK_FILLED
  Color::Yellow,    // MSG_WATER_LEVEL_LOW
  Color::Yellow,    // MSG_VOLTAGE_LOW
  Color::Yellow,    // MSG_OVER_CURRENT_TRIPPED
};


