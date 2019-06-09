
#include <Arduino.h>
#include <SPI.h>

#include "pump_controller.h"

PumpController g_pump_controller;

void setup( )
{
  g_pump_controller.begin( );
}

void loop( )
{
  g_pump_controller.loop( );
}
