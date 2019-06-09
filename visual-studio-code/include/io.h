#ifndef PINS_H
#define PINS_H

#include <Arduino.h>

#include "common.h"
#include "graphics.h"
#include "lcd.h"
#include "touch.h"
#include "ultrasonic.h"
#include "meter.h"

#define PUSHBUTTON_LED  1
#define PUSHBUTTON      2

#define FLOAT_LOW       4
#define FLOAT_HIGH      5

#define TOUCH_IRQ       3
#define TOUCH_CS        7

#define LCD_BACKLIGHT   6
#define LCD_RST         8
#define LCD_DC          9
#define LCD_CS          10

// The following three don't need to be defined
#define SPI_MOSI        11
#define SPI_MISO        12
#define SPI_SCK         13

#define CONTACTOR       A0
#define CONTACTOR_AUX   A1
#define MODE_SWITCH     A4
#define LDR             A5

#define TRIGGER         A2
#define ECHO            A3

#define CT_PIN          A6
#define PT_PIN          A7

#define LED_RED         LOW
#define LED_GREEN       HIGH

typedef void ( *interrupt_callback_t )( void* );

enum FloatStatus{
  Dropped = LOW,
  Floating = HIGH
};

enum StatusLED{
  LedRed = LOW,
  LedGreen = HIGH
};

class IO {
public:
  
  static void init_pins( );
  static void attach_button_interrupt( interrupt_callback_t on_button_interrupt, void* callback_data );
  static void attach_touch_interrupt( interrupt_callback_t on_touch_interrupt, void* callback_data );
  static void enable_button_interrupt( );
  static void enable_touch_interrupt( );
  static void disable_button_interrupt( );
  static void disable_touch_interrupt( );

  static LCD create_lcd( );
  static Touch create_touch( );
  static Ultrasonic create_ultrasonic_sensor( uint16_t max_cm_distance );
  static Meter create_meter( );

  static void start_pump( );
  static void stop_pump( );
  
  static int get_ldr_value( );
  static bool is_day( );
  static bool is_contactor_closed( );
  static bool is_autostart_enabled( );

  static double get_current( );
  
  static void update_status_led( StatusLED status );

private:
  static void button_ISR( );
  static void touch_ISR( );
  
private:
  static interrupt_callback_t m_on_button_interrupt;
  static interrupt_callback_t m_on_touch_interrupt;
  static void* m_callback_data;
};

#endif // PINS_H
