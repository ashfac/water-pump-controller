
#include "pump_controller.h"
#include "messages.h"

PumpController::PumpController( )
  : m_meter( IO::create_meter( ) )
  , m_ultrasonic( IO::create_ultrasonic_sensor( MAX_ULTRASONIC_DISTANCE ) )
  , m_touch( IO::create_touch( ) )
  , m_interrupt_time( 0 )
  , m_pump_start_stop_time( 0 )
{
}

void PumpController::begin( )
{
  // pin configuration
  IO::init_pins( );

  // enable push button and touch screen interrupts
  IO::attach_button_interrupt( on_button_interrupt, this );
  IO::attach_touch_interrupt( on_touch_interrupt, this );
  
  m_screen.init( );
  m_touch.begin( LCD_HEIGHT, LCD_WIDTH );
}

void PumpController::loop( )
{
  read_inputs( );
  take_actions( );
  update_status( );
}

void PumpController::read_inputs( )
{
  //m_meter.calcVI( 20,1000 );

  m_ldr_value = IO::get_ldr_value( );
  
  m_float_low = ( digitalRead( FLOAT_LOW ) == LOW ) ? FloatStatus::Dropped : FloatStatus::Floating;
  m_float_high = ( digitalRead( FLOAT_HIGH ) == LOW ) ? FloatStatus::Dropped : FloatStatus::Floating;

  uint16_t ping_distance_cm = static_cast< uint16_t >( m_ultrasonic.ping_cm( ) );
  uint16_t water_level = estimate_water_level( ping_distance_cm, m_float_low );
  m_time_to_fill = estimate_time_to_fill( water_level );
  m_water_level = water_level;
}

void PumpController::take_actions( )
{
  if( is_contactor_closed( ) || is_pump_running( ) )
  {
    if( m_float_high == FloatStatus::Floating )
    {
      stop_pump( );
      m_screen.print_message( MSG_TANK_FILLED );
    }
    else if( overcurrent_detected( ) )
    {
      stop_pump( );
      m_screen.print_message( MSG_OVER_CURRENT_TRIPPED );
    }
  }
  else if( m_float_low == FloatStatus::Dropped && IO::is_autostart_enabled( ) && IO::is_day( ) )
  {
    start_pump( );
  }
}

void PumpController::update_status( )
{
  // LED light in push button
  // RED: when lower float is dropped, else GREEN
  IO::update_status_led( ( m_float_low == FloatStatus::Dropped ) ? StatusLED::LedRed : StatusLED::LedGreen );

  // voltage and current
  m_meter.Vrms = 0;
  m_meter.Irms = IO::get_current( );
  
  m_screen.print_meter( m_meter.Vrms, m_meter.Irms );

  // water tank level
  m_screen.print_water_level( m_water_level, m_float_low, m_float_high );
  m_screen.print_time_to_fill( m_time_to_fill );

//  m_screen.print_ldr_value( m_ldr_value );

  // Clear timed-out messages from screen
//  m_screen.update_messages( );
}

bool PumpController::start_pump( )
{
  unsigned long current_time = millis( );
  double rms_current;
  
  if( m_screen.is_meter_ready( ) && m_float_high == FloatStatus::Dropped
      && ( m_pump_start_stop_time == 0 || ( current_time - m_pump_start_stop_time ) > PUMP_START_DELAY ) )
  {
    IO::start_pump( );
    m_pump_start_stop_time = current_time;

    m_screen.print_message( MSG_PUMP_STARTED );
    
    return true;
  }

  return false;
}

bool PumpController::stop_pump( )
{
  unsigned long current_time = millis( );
  
  if( is_pump_running( ) && ( ( current_time - m_pump_start_stop_time ) > PUMP_STOP_DELAY ) )
  {
    IO::stop_pump( );
    m_pump_start_stop_time = current_time;

    m_screen.print_message( MSG_PUMP_STOPPED );
    
    return true;
  }

  return false;
}

uint16_t PumpController::estimate_water_level( uint16_t ping_distance_cm, FloatStatus float_low )
{
  if( ping_distance_cm < WATER_LEVEL_FULL || ping_distance_cm > WATER_LEVEL_EMPTY )
  {
    if( float_low == FloatStatus::Dropped )
    {
      ping_distance_cm = WATER_LEVEL_EMPTY;
    }
    else
    {
      ping_distance_cm = WATER_LEVEL_FULL;
    }
  }

  return ( map( ping_distance_cm, WATER_LEVEL_EMPTY, WATER_LEVEL_FULL, 0, 100 ) );
}

uint16_t PumpController::estimate_time_to_fill( uint16_t water_level )
{
  if( m_water_level == WATER_LEVEL_UNKNOWN )
  {
    return TIME_TO_FILL_UNKNOWN;
  }

  return water_level;
}

bool PumpController::overcurrent_detected( )
{
  unsigned long time_elapsed = millis( ) - m_pump_start_stop_time;
  
  return( time_elapsed > OVERCURRENT_TRIP_WAIT && m_meter.Irms > CURRENT_MAX );
}

void PumpController::on_button_action( )
{
  unsigned long interrupt_time = millis( );
  
  if( interrupt_time - m_interrupt_time > 200 )
  {
    if( is_pump_running( ) )
    {
      stop_pump( );
    }
    else
    {
      start_pump( );
    }
    
    m_interrupt_time = interrupt_time;
  }
}

void PumpController::on_touch_action( )
{
  unsigned long interrupt_time = millis( );
  
  if( interrupt_time - m_interrupt_time > 200 )
  {
    
    m_interrupt_time = interrupt_time;
  }
}

bool PumpController::is_contactor_closed( )
{
  return IO::is_contactor_closed( );
}

bool PumpController::is_pump_running( )
{
  return ( m_meter.Irms > CURRENT_MIN );
}

void PumpController::on_button_interrupt( void* callback_data )
{
  if( callback_data != NULL )
  {
    PumpController* self = static_cast< PumpController* >( callback_data );
    self->on_button_action( );
  }
}

void PumpController::on_touch_interrupt( void* callback_data )
{
  if( callback_data != NULL )
  {
    PumpController* self = static_cast< PumpController* >( callback_data );
    self->on_touch_action( );
  }

}

