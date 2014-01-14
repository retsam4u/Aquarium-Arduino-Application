// *** Date and time ***
#define TIME_MODE_24H                       0 // time is displayed as 24 hour mode (00-23)
#define TIME_MODE_AM_PM                     1 // time is displayed as 12 hour mode with AM/PM (01-12)

// *** Aquarium light ***
#define AQUARIUM_LIGHT_MODE_AUTO            0 // lights turn on/off based on time and alarms - user can control them but cannot override alarms
#define AQUARIUM_LIGHT_MODE_MANUAL          1 // lights are turned on/off on user command - application doesn't intervene
#define AQUARIUM_LIGHT_STATUS_OFF           0 // must be identical to relay switch position off; it reflects it's state
#define AQUARIUM_LIGHT_STATUS_ON            1 // must be identical to relay switch position on; it reflects it's state

// *** Aquarium vent ***
#define AQUARIUM_VENT_MODE_AUTO             0 // vents turn on/off based on light time and alarms - user can control them but cannot override alarms
#define AQUARIUM_VENT_MODE_MANUAL           1 // vents are turned on/off on user command - application doesn't intervene
#define AQUARIUM_VENT_STATUS_OFF            0 // must be identical to relay switch position off; it reflects it's state
#define AQUARIUM_VENT_STATUS_ON             1 // must be identical to relay switch position on; it reflects it's state

// *** Speaker settings ***
#define SPEAKER_MODE_OFF                    0 // speaker is sillet (no sounds are made except the system staring beep)
#define SPEAKER_MODE_ON                     1 // all sounds are turned on: when users pushes a button, when different event occurs and on alarms
#define SPEAKER_MODE_ALARMS                 2 // only alarms sound are turned on
#define SPEAKER_MODE_CRITICAL               3 // only the critical limits alarms trigger sounds

// *** Alarms settings ***
#define ALARMS_MODE_ON                      0 // all alarms are on
#define ALARMS_MODE_OFF                     1 // no alarms are turned on: no limits are checked (not even critical one)... no actions are made
#define ALARMS_MODE_CRITICAL                2 // only critical limits are checked and their actions perfomed

// *** LCD settings ***
#define LCD_MODE_AUTO                       0 // LCD is turned on at startup, after or before some actions, and when user requests it (pussing on/off button)... and after timeout it turns off
#define LCD_MODE_MANUAL                     1 // LCD is turned on/off on user command - application doesn't intervene
#define LCD_STATUS_OFF                      0 // must be identical to LCD backlight pin status - it reflects it's state
#define LCD_STATUS_ON                       1 // must be identical to LCD backlight pin status - it reflects it's state

// *** Water temperature settings ***
// *** Water level settings ***

// *** Menu constants ***
#define SETTUP_DATE_DAY       0
#define SETTUP_DATE_MONTH     1
#define SETTUP_DATE_YEAR      2
#define SETTUP_TIME_HOUR      3
#define SETTUP_TIME_MINUTE    4
#define SETTUP_TIME_MODE      5

// *** Alarms constants ***
#define ALARM_NO_ACTION        0
#define ALARM_VENT_ON          1
#define ALARM_VENT_OFF         2
#define ALARM_LCD_ON           3
#define ALARM_LCD_OFF          4
#define ALARM_LCD_BLINK        5
#define ALARM_BEEP_0SEC        6
#define ALARM_BEEP_5SEC        7
#define ALARM_BEEP_1MIN        8
#define ALARM_BEEP_5MIN        9
#define ALARM_AQL_ON          10
#define ALARM_AQL_OFF         11
#define ALARM_AQL_BLINK_OFF   12
#define ALARM_AQL_BLINK_ON    13

// *** LCD info screen ***
#define LCD_INFO_DATE_TIME        0
#define LCD_INFO_WATER_TEMP       1
#define LCD_INFO_WATER_LEVEL      2
#define LCD_INFO_ROOM_TEMP        3
#define LCD_INFO_ROOM_HUMIDITY    4
#define LCD_INFO_EXT_LIGHT        5
#define LCD_INFO_AQ_LIGHT         6
#define LCD_INFO_AQ_VENTILATION   7
#define LCD_INFO_LCD_STATUS       8
#define LCD_INFO_ALARMS_STATUS    9
#define LCD_INFO_SPEAKER_STATUS  10
#define LCD_INFO_LAST_FEED_TIME  11
#define LCD_INFO_REMAIN_TO_FEED  12
#define LCD_INFO_DESPRE          13

