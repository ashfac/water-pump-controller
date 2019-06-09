#ifndef MESSAGES_H
#define MESSAGES_H

#include "screen.h"

// message lines
#define MSG_LINE_1    0
#define MSG_LINE_2    1

// message definitions
#define MSG_PLEASE_WAIT           0
#define MSG_READY                 1
#define MSG_PUMP_STARTED          2
#define MSG_PUMP_STOPPED          3
#define MSG_PUMP_TRIPPED          4
#define MSG_TANK_FILLED           5
#define MSG_WATER_LEVEL_LOW       6
#define MSG_VOLTAGE_LOW           7
#define MSG_OVER_CURRENT_TRIPPED  8

#define MSG_LAST 9 // last message num + 1

// some common timeouts
#define TIMEOUT_NIL     ( 0UL )
#define TIMEOUT_1_SEC   ( 1000UL )
#define TIMEOUT_5_SEC   ( 5UL  * TIMEOUT_1_SEC )
#define TIMEOUT_10_SEC  ( 10UL * TIMEOUT_1_SEC )
#define TIMEOUT_30_SEC  ( 30UL * TIMEOUT_1_SEC )
#define TIMEOUT_1_MIN   ( 60UL * TIMEOUT_1_SEC )
#define TIMEOUT_2_MIN   ( 2UL  * TIMEOUT_1_MIN )
#define TIMEOUT_5_MIN   ( 5UL  * TIMEOUT_1_MIN )
#define TIMEOUT_10_MIN  ( 10UL * TIMEOUT_1_MIN )
#define TIMEOUT_20_MIN  ( 20UL * TIMEOUT_1_MIN )
#define TIMEOUT_30_MIN  ( 30UL * TIMEOUT_1_MIN )

// message text
extern char g_message[MSG_LAST][22];

// corresponding message lines
extern byte g_message_line[MSG_LAST];

// corresponding message timeouts
extern unsigned long g_message_timeout[MSG_LAST];

// corresponding message colors
extern Color g_message_color[MSG_LAST];

#endif // MESSAGES_H
