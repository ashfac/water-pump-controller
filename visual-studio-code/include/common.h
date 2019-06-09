
#ifndef COMMON_H
#define COMMON_H

#include "lcd.h"

// constants
#define LCD_WIDTH  320
#define LCD_HEIGHT 240

#define MAX_ULTRASONIC_DISTANCE 200
#define LDR_DAY_THRESHOLD       500
#define METER_DISCARD_READINGS  1
#define WATER_LEVEL_UNKNOWN     101
#define TIME_TO_FILL_UNKNOWN    ( -1 )
#define TIME_TO_FILL_MAX        ( 9 * 60 + 59 )

#define METER_V_CALIBRATION     473.0
#define METER_V_PHASE           -1.9
#define METER_I_CALIBRATION     22.5

#define VOLTAGE_MIN             150 // volts
#define VOLTAGE_MAX             235 // volts

#define CURRENT_MIN             0.2 // Ampere
#define CURRENT_NOMINAL         5   // Ampere
#define CURRENT_MAX             9   // Ampere
#define OVERCURRENT_TRIP_WAIT   3000 // milli-seconds

#define PUMP_START_DELAY        10000 // milli-seconds
#define PUMP_STOP_DELAY         1000  // milli-seconds

#define LCD_BACKGROUND_COLOR  Color::Black
#define LCD_FOREGROUND_COLOR  Color::White

#define WATER_LEVEL_EMPTY  80
#define WATER_LEVEL_FULL   20

#define WATER_TANK_LEFT    180
#define WATER_TANK_RIGHT   310
#define WATER_TANK_TOP     120
#define WATER_TANK_BOTTOM  232

#define WATER_TANK_WIDTH ( WATER_TANK_RIGHT - WATER_TANK_LEFT )
#define WATER_TANK_HEIGHT ( WATER_TANK_BOTTOM - WATER_TANK_TOP )

#define METER_LEFT         10
#define METER_WIDTH        ( WATER_TANK_LEFT - METER_LEFT - 14 )
#define METER_HEIGHT       50
#define METER_TOP_I        ( LCD_HEIGHT - METER_HEIGHT - 8 )
#define METER_TOP_V        ( METER_TOP_I - METER_HEIGHT - 12 )

#define MSG_LEFT           METER_LEFT
#define MSG_WIDTH          METER_WIDTH
#define MSG_HEIGHT         50
#define MSG_TOP_1          10
#define MSG_TOP_2          ( MSG_HEIGHT + MSG_TOP_1 + 4 )

// TTF: TIME TO FILL
#define TTF_LEFT           WATER_TANK_LEFT
#define TTF_TOP_1            8
#define TTF_WIDTH          WATER_TANK_WIDTH
#define TTF_HEIGHT         METER_HEIGHT

#define TTF_LEFT           WATER_TANK_LEFT
#define TTF_TOP_H          8
#define TTF_HEIGHT_H       50
#define TTF_TOP            ( TTF_HEIGHT_H + 8 )
#define TTF_WIDTH          WATER_TANK_WIDTH
#define TTF_HEIGHT         METER_HEIGHT

#endif // COMMON_H

