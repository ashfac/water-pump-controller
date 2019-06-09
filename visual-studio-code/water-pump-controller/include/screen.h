#ifndef SCREEN_H
#define SCREEN_H

#include "graphics.h"
#include "screen.h"
#include "io.h"

class Screen {
public:
  Screen( );

  void init( );
  void clear( );

  void print_message( byte msg_id );
  void clear_message( byte msg_line );
  void update_messages( );
  void print_meter( double voltage, double current );
  void print_water_level( uint16_t water_level, FloatStatus float_low, FloatStatus float_high );
  void print_time_to_fill( uint16_t time_to_fill );
  void print_ldr_value( int ldr_value );

  bool is_meter_ready( );

private:
  LCD m_lcd;

  uint8_t m_meter_discard_readings;
  int  m_voltage;
  int  m_current;
  
  uint16_t m_time_to_fill;
  uint16_t m_water_level;
  FloatStatus m_float_low;
  FloatStatus m_float_high;

  unsigned long m_message_time[3];
  unsigned long m_message_timeout[3];
};

#endif // SCREEN_H
