
#include <Arduino.h>
#include "io.h"

// static member initialization
interrupt_callback_t IO::m_on_button_interrupt = NULL;
interrupt_callback_t IO::m_on_touch_interrupt = NULL;
void* IO::m_callback_data = NULL;

void IO::init_pins( )
{
  // initialize all output pins
  pinMode( PUSHBUTTON_LED, OUTPUT );
  pinMode( CONTACTOR, OUTPUT );

  // initialize all input pins
  pinMode( PUSHBUTTON, INPUT_PULLUP );
  pinMode( TOUCH_IRQ, INPUT_PULLUP );
  pinMode( FLOAT_LOW, INPUT_PULLUP );
  pinMode( FLOAT_HIGH, INPUT_PULLUP );
//  pinMode( CONTACTOR_AUX, INPUT_PULLUP );
  pinMode( MODE_SWITCH, INPUT_PULLUP );
  pinMode( LDR, INPUT_PULLUP );

  //pinMode( ECHO, INPUT_PULLUP ); // PULL UP or DOWN?

  digitalWrite( PUSHBUTTON_LED, LED_RED );
  digitalWrite( CONTACTOR, LOW );
  analogWrite( LCD_BACKLIGHT, 100 );
}

void IO::attach_button_interrupt( interrupt_callback_t on_button_interrupt, void* callback_data )
{
  m_on_button_interrupt = on_button_interrupt;
  m_callback_data = callback_data;
  enable_button_interrupt( );
}

void IO::attach_touch_interrupt( interrupt_callback_t on_touch_interrupt, void* callback_data )
{
  m_on_touch_interrupt = on_touch_interrupt;
  m_callback_data = callback_data;
  enable_touch_interrupt( );
}

void IO::enable_button_interrupt( )
{
  attachInterrupt( digitalPinToInterrupt( PUSHBUTTON ), button_ISR, FALLING );
}

void IO::enable_touch_interrupt( )
{
  attachInterrupt( digitalPinToInterrupt( TOUCH_IRQ ), touch_ISR, FALLING );
}

void IO::disable_button_interrupt( )
{
  detachInterrupt( digitalPinToInterrupt( PUSHBUTTON ) );
}

void IO::disable_touch_interrupt( )
{
  detachInterrupt( digitalPinToInterrupt( TOUCH_IRQ ) );
}

LCD IO::create_lcd( )
{
  return LCD( LCD_CS, LCD_DC, LCD_RST );
}

Touch IO::create_touch( )
{
  return Touch( TOUCH_CS, TOUCH_IRQ );
}

Ultrasonic IO::create_ultrasonic_sensor( uint16_t max_cm_distance )
{
  return Ultrasonic( TRIGGER, ECHO, max_cm_distance );
}

Meter IO::create_meter( )
{
  return Meter( PT_PIN, METER_V_CALIBRATION, METER_V_PHASE, CT_PIN, METER_I_CALIBRATION );
}

void IO::start_pump( )
{
  digitalWrite( CONTACTOR, HIGH );
}

void IO::stop_pump( )
{
  digitalWrite( CONTACTOR, LOW );
}

int IO::get_ldr_value( )
{
  return analogRead( LDR );
}

bool IO::is_day( )
{
  return ( get_ldr_value( ) < LDR_DAY_THRESHOLD );
}

bool IO::is_contactor_closed( )
{
  return ( digitalRead( CONTACTOR_AUX ) == LOW );
}

bool IO::is_autostart_enabled( )
{
  return ( digitalRead( MODE_SWITCH ) == LOW );
}

double IO::get_current( )
{
  double current = 0.0;

  for( byte i = 0; i < 200; i++ )
  {
    current += analogRead( CONTACTOR_AUX );
    delay( 5 );
  }

  current /= 200; // take average
  current -= 750; // 743; // remove offset
  current = current * 3.3 / 0.185 / 1024;
  
  return current;
}

void IO::update_status_led( StatusLED status )
{
  digitalWrite( PUSHBUTTON_LED, status );
}

void IO::button_ISR( )
{
  if( m_on_button_interrupt != NULL )
  {
    m_on_button_interrupt( m_callback_data );
  }
}
void IO::touch_ISR( )
{
  if( m_on_touch_interrupt != NULL )
  {
    m_on_touch_interrupt( m_callback_data );
  }
}

