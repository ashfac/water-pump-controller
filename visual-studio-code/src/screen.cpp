
#include "io.h"
#include "common.h"
#include "graphics.h"
#include "lcd.h"
#include "messages.h"
#include "screen.h"

Screen::Screen( )
  : m_lcd( IO::create_lcd( ) )
  , m_water_level( WATER_LEVEL_UNKNOWN )
  , m_time_to_fill( 0 )
  , m_voltage( -1 )
  , m_current( -1 )
{
  m_meter_discard_readings = METER_DISCARD_READINGS;
}

void Screen::init( )
{
  m_lcd.begin();
  m_lcd.setRotation(1);
  
  clear( );

//  for( uint16_t i = 0; i < 320; i += 8 )
//  {
//    for( uint16_t j = 0; j < 240; j += 8 )
//    {
//      m_lcd.drawPixel( i, j, ILI9340_YELLOW );
//    }
//  }
}

void Screen::clear( )
{
  m_lcd.fillScreen( LCD_BACKGROUND_COLOR );
  m_lcd.fillRoundRect( 0, 0, LCD_WIDTH, LCD_HEIGHT, 5, Color::DarkGrey );
  m_lcd.setTextColor( LCD_FOREGROUND_COLOR );
  m_lcd.setTextSize( 1 );

  // Time to Fill
  m_lcd.fillRoundRect( TTF_LEFT - 4, TTF_TOP_H - 4, TTF_WIDTH + 8, TTF_HEIGHT_H + 8, 5, LCD_BACKGROUND_COLOR );
  m_lcd.drawString( "TIME LEFT", TTF_LEFT + 4, TTF_TOP_H + 8, 4 );
  m_lcd.drawFastHLine( TTF_LEFT + 2, TTF_TOP - 10, TTF_WIDTH - 4, Color::DarkGrey );
  m_lcd.fillRoundRect( TTF_LEFT - 4, TTF_TOP - 4, TTF_WIDTH + 8, TTF_HEIGHT + 8, 5, LCD_BACKGROUND_COLOR );
    
  // message lines
//  m_lcd.fillRoundRect( MSG_LEFT, MSG_TOP, MSG_WIDTH, MSG_HEIGHT + 6, 5, LCD_BACKGROUND_COLOR );
//  m_lcd.fillRoundRect( MSG_LEFT, MSG_TOP, MSG_WIDTH, ( MSG_HEIGHT + 6 ) * 3 , 5, LCD_BACKGROUND_COLOR );
//  print_message( MSG_PUMP_STARTED );
  
  // meter
  // voltage
  m_lcd.fillRoundRect( METER_LEFT - 4, METER_TOP_V - 4, METER_WIDTH + 8, METER_HEIGHT + 8, 5, LCD_BACKGROUND_COLOR );
  m_lcd.setTextColor( Color::White );
  m_lcd.drawString( "STARTING", METER_LEFT + 16, METER_TOP_V + 16, 4 );

  // current
  m_lcd.fillRoundRect( METER_LEFT - 4, METER_TOP_I - 4, METER_WIDTH + 8, METER_HEIGHT + 8, 5, LCD_BACKGROUND_COLOR );
  m_lcd.drawNumber( m_meter_discard_readings, METER_LEFT + 60, METER_TOP_I, 7 );
  
  // water tank
  // top
  m_lcd.fillRoundRect( WATER_TANK_LEFT - 8, WATER_TANK_TOP - 4, WATER_TANK_WIDTH + 16, 4, 3, Color::White );

  // left + right + bottom
  m_lcd.fillRoundRect( WATER_TANK_LEFT - 4, WATER_TANK_TOP - 4, WATER_TANK_WIDTH + 8, WATER_TANK_HEIGHT + 8, 5, Color::White );
}

void Screen::print_message( byte msg_id )
{
  if( msg_id < MSG_LAST )
  {
    byte msg_line = g_message_line[msg_id];
    m_message_timeout[ msg_line ] = g_message_timeout[msg_id];
    m_message_time[ msg_line ] = millis( );
    
    int y;
    switch( msg_line )
    {
      case MSG_LINE_1: y = MSG_TOP_1; break;
      case MSG_LINE_2: y = MSG_TOP_2; break;
      default: break;
    }

    m_lcd.setTextColor( g_message_color[msg_id] );
    m_lcd.setTextSize( 1 );

    //clear_message( msg_line );
    // m_lcd.drawCentreString( g_message[msg_id], MSG_LEFT + ( ( MSG_WIDTH - MSG_LEFT ) >> 1 ), y, 4 );
    //int x = m_lcd.drawString( "TIME TO FILL: ", MSG_LEFT + 20, y + 20, 4 );
    //m_lcd.drawString( "2:34", x + 8, y, 7 );
  }
}

void Screen::clear_message( byte msg_line )
{
  if( msg_line >= MSG_LINE_1 && msg_line <= MSG_LINE_2 )
  {
    int y;
    switch( msg_line )
    {
      case MSG_LINE_1: y = MSG_TOP_1; break;
      case MSG_LINE_2: y = MSG_TOP_2; break;
      default: break;
    }
    
    m_lcd.fillRoundRect( MSG_LEFT, y, MSG_WIDTH, MSG_HEIGHT, 5, LCD_BACKGROUND_COLOR );
  }
}

void Screen::update_messages( )
{
  unsigned long current_time = millis( );

  for( byte i = MSG_LINE_1; i <= MSG_LINE_2; ++i )
  {
    if( m_message_timeout[i] != 0 && ( ( current_time - m_message_time[i] ) > m_message_timeout[i] ) )
    {
      clear_message( i );
    }
  }
}

void Screen::print_meter( double voltage, double current )
{
  if( m_meter_discard_readings > 0 )
  {
    --m_meter_discard_readings;

    m_lcd.setTextColor( Color::White );

    if( m_meter_discard_readings == 0 )
    {
      //print_message( MSG_READY );
      m_lcd.fillRoundRect( METER_LEFT - 4, METER_TOP_V - 4, METER_WIDTH + 8, METER_HEIGHT + 8, 5, LCD_BACKGROUND_COLOR );
      m_lcd.fillRoundRect( METER_LEFT - 4, METER_TOP_I - 4, METER_WIDTH + 8, METER_HEIGHT + 8, 5, LCD_BACKGROUND_COLOR );
      m_lcd.drawChar( 'V', METER_LEFT + METER_WIDTH - 20, METER_TOP_V + 26, 4 );
      m_lcd.drawChar( 'A', METER_LEFT + METER_WIDTH - 20, METER_TOP_I + 26, 4 );
    }
    else
    {
      m_lcd.fillRoundRect( METER_LEFT - 4, METER_TOP_I - 4, METER_WIDTH - 24, METER_HEIGHT + 8, 5, LCD_BACKGROUND_COLOR );
      m_lcd.drawNumber( m_meter_discard_readings, METER_LEFT + 60, METER_TOP_I, 7 );
    }
  }
  else
  {
    m_lcd.setTextSize( 1 );

    if( voltage < 20 )
    {
      voltage = 0;
    }
    else if( voltage >= 999 )
    {
      voltage = m_voltage;
    }
      
    // voltage
    if( (int)voltage != m_voltage )
    {
      m_voltage = voltage;
      
      m_lcd.fillRoundRect( METER_LEFT - 4, METER_TOP_V - 4, METER_WIDTH - 20, METER_HEIGHT + 8, 5, LCD_BACKGROUND_COLOR );
      m_lcd.setTextColor( ( voltage < VOLTAGE_MIN ) ? Color::Yellow : ( voltage > VOLTAGE_MAX ) ? Color::Red : Color::Green );
      
      if( voltage < 10 )
      {
        m_lcd.drawFloat( voltage, 2, METER_LEFT + 4 , METER_TOP_V, 7 );
      }
      else if( voltage < 100 )
      {
        m_lcd.drawFloat( voltage, 1, METER_LEFT + 4 , METER_TOP_V, 7 );
      }
      else
      {
        m_lcd.drawFloat( voltage, 0, METER_LEFT + 24 , METER_TOP_V, 7 );
      }
    }

    // current
//    if( current < 0.2 )
//    {
//      current = 0.0;
//    }
//    else if( current > 99 )
//    {
//      current = 99.0;
//    }

    int scaled_current = static_cast< int >( current * 100.0 );
    
    if( scaled_current != m_current )
    {
      m_current = scaled_current;

      m_lcd.fillRoundRect( METER_LEFT - 4, METER_TOP_I - 4, METER_WIDTH - 20, METER_HEIGHT + 8, 5, LCD_BACKGROUND_COLOR );
      m_lcd.setTextColor( ( current < CURRENT_MAX ) ? Color::Green : Color::Red );
      
      if( current < 10 )
      {
        m_lcd.drawFloat( current, 2, METER_LEFT + 4, METER_TOP_I, 7 );
      }
      else
      {
        m_lcd.drawFloat( current, 1, METER_LEFT + 4, METER_TOP_I, 7 );
      }
    }
  }
}

void Screen::print_water_level( uint16_t water_level, FloatStatus float_low, FloatStatus float_high )
{
  if( m_water_level == WATER_LEVEL_UNKNOWN
      || water_level != m_water_level
      || float_low != m_float_low
      || float_high != m_float_high )
  {
    m_float_low   = float_low;
    m_float_high  = float_high;

    // estimate time to fill
    m_water_level = water_level;
    
    uint16_t tank_level = map( water_level, 0, 100, WATER_TANK_BOTTOM, WATER_TANK_TOP );

    uint16_t color = ( float_low == FloatStatus::Dropped ) ? ( ( float_high == FloatStatus::Dropped ) ? Color::Red : Color::Purple )
                   : ( float_high == FloatStatus::Dropped ) ? Color::DarkGreen : Color::Blue;


    // empty portion
    m_lcd.fillRect( WATER_TANK_LEFT, WATER_TANK_TOP, WATER_TANK_WIDTH, tank_level - WATER_TANK_TOP, LCD_BACKGROUND_COLOR );
    
    // filled portion
    m_lcd.fillRect( WATER_TANK_LEFT, tank_level, WATER_TANK_WIDTH, WATER_TANK_BOTTOM - tank_level, color );

    // print water level in percent
    int16_t x = WATER_TANK_LEFT + 24;
    int16_t y = WATER_TANK_TOP + 4;
    
    if( water_level < 50 )
    {
      if( water_level >= 0 && water_level < 10 )
        x += 16;
    }
    else
    {
      y = WATER_TANK_BOTTOM - 54;
    }

    m_lcd.setTextColor( Color::White );
    m_lcd.setCursor( x, y );
    if( float_low == FloatStatus::Floating && float_high == FloatStatus::Floating && water_level >= 100 )
    {
      m_lcd.setTextSize( 2 );
      m_lcd.drawCentreString( "FULL",
                              WATER_TANK_LEFT + ( ( WATER_TANK_RIGHT - WATER_TANK_LEFT ) >> 1 ),
                              WATER_TANK_TOP - 20 + ( ( WATER_TANK_BOTTOM - WATER_TANK_TOP ) >> 1 ),
                              4 );        
    }
    else if ( float_low == FloatStatus::Dropped && float_high == FloatStatus::Floating && water_level == 0 )
    {
      m_lcd.setTextSize( 1 );
      m_lcd.drawCentreString( "ERROR",
                              WATER_TANK_LEFT + ( ( WATER_TANK_RIGHT - WATER_TANK_LEFT ) >> 1 ),
                              WATER_TANK_TOP - 20 + ( ( WATER_TANK_BOTTOM - WATER_TANK_TOP ) >> 1 ),
                              4 );        
    }
    else if ( float_low == FloatStatus::Dropped && water_level == 0 )
    {
      m_lcd.setTextSize( 1 );
      m_lcd.drawCentreString( "EMPTY",
                              WATER_TANK_LEFT + ( ( WATER_TANK_RIGHT - WATER_TANK_LEFT ) >> 1 ),
                              WATER_TANK_TOP - 20 + ( ( WATER_TANK_BOTTOM - WATER_TANK_TOP ) >> 1 ),
                              4 );        
    }
    else
    {
      m_lcd.setTextSize( 1 );
      int dx;
      
      if( water_level >= 100 )
      {
        dx = m_lcd.drawNumber( 99, x, y, 7 );
      }
      else
      {
        dx = m_lcd.drawNumber( water_level, x, y, 7 );
      }
  
      m_lcd.drawChar( '%', x + dx + 2, y + 26, 4 );
    }
  }
}

void Screen::print_time_to_fill( uint16_t time_to_fill )
{
  if( time_to_fill != m_time_to_fill )
  {
    m_lcd.setTextSize( 1 );
    m_lcd.setTextColor( Color::White );
    
    m_lcd.fillRoundRect( TTF_LEFT - 4, TTF_TOP - 4, TTF_WIDTH + 8, TTF_HEIGHT + 8, 5, LCD_BACKGROUND_COLOR );
    
    if( time_to_fill == TIME_TO_FILL_UNKNOWN || time_to_fill > TIME_TO_FILL_MAX )
    {
      m_lcd.drawString( ".:..", TTF_LEFT, TTF_TOP, 7 );
    }
    else
    {
      unsigned int hours = time_to_fill / 60;
      unsigned int minutes = time_to_fill % 60;

      m_lcd.drawNumber( hours, TTF_LEFT + 8, TTF_TOP, 7 );
      
      m_lcd.drawChar( ':', TTF_LEFT + 8 + 36, TTF_TOP, 7 );

      if( minutes < 10 )
      {
        m_lcd.drawNumber( 0, TTF_LEFT + 8 + 50, TTF_TOP, 7 );
        m_lcd.drawNumber( minutes, TTF_LEFT + 8 + 84, TTF_TOP, 7 );
      }
      else if( minutes < 20 )
      {
        m_lcd.drawNumber( minutes, TTF_LEFT + 8 + 40, TTF_TOP, 7 );
      }
      else
      {
        m_lcd.drawNumber( minutes, TTF_LEFT + 8 + 50, TTF_TOP, 7 );
      }
    }

    m_time_to_fill = time_to_fill;
  }
}

void Screen::print_ldr_value( int ldr_value )
{
  m_lcd.fillRoundRect( 0, 0, METER_WIDTH + 8, METER_HEIGHT + 8, 5, LCD_BACKGROUND_COLOR );
  m_lcd.drawNumber( ldr_value, 10, 4, 7 );
}

bool Screen::is_meter_ready( )
{
  return ( m_meter_discard_readings == 0 );
}


