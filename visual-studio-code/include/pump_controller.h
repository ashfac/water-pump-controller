#ifndef PUMP_CONTROLLER_H
#define PUMP_CONTROLLER_H

#include "common.h"
#include "io.h"
#include "graphics.h"
#include "lcd.h"
#include "touch.h"
#include "screen.h"

class PumpController{
public:
  PumpController( );
  void begin( );
  void loop( );

private:
  void read_inputs( );
  void take_actions( );
  void update_status( );

  bool start_pump( );
  bool stop_pump( );
  uint16_t estimate_water_level( uint16_t ping_distance_cm, FloatStatus float_low );
  uint16_t estimate_time_to_fill( uint16_t water_level );

  bool overcurrent_detected( );
  bool is_contactor_closed( );
  bool is_pump_running( );
  
  void on_button_action( );
  void on_touch_action( );
  
  static void on_button_interrupt( void* callback_data );
  static void on_touch_interrupt( void* callback_data );

private:
  Meter m_meter;
  Ultrasonic m_ultrasonic;
  Touch m_touch;
  Screen m_screen;

  uint16_t m_water_level;
  uint16_t m_time_to_fill;
  int m_ldr_value;
  FloatStatus m_float_low;
  FloatStatus m_float_high;

  unsigned long m_interrupt_time;
  unsigned long m_pump_start_stop_time;
};

#endif // PUMP_CONTROLLER_H
