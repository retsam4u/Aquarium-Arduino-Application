/*
* Aquarium Application for Arduino
* ver. 1.3
* Changes:
* > version 1.3:
*     - make change value work in selection menu (write setCurrentSettupDataValue method)
*     - create value-description pairs for variable values that have meaning 
*       (ex. for aquariumLightsStatus make pairs: AQUARIUM_LIGHT_STATUS_OFF=0-"OFF";AQUARIUM_LIGHT_STATUS_ON=1-"ON")
*     - make value limits (ex. for setting time->minutes limits are: 0 and 59)
* > version 1.2:
*     - fixes
*     - cancel, previous value, next value, load selected value in selection section from menu
* > version 1.1:
*     - change appconstants and application variables
*     - change how ir commands are read
*     - add menu option settup mode
* > version 1.0:
*     - add settings menu (with MenuSystem library)
* (c) 2014 Protiuc Valentin
*/

// === Include section ===
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <IRremote.h>
#include <DHT.h>
#include "pindef.h"
#include "pitches.h"
#include "appconstants.h"
#include "ir_codes.h"
#include <MenuSystem.h>

#define APP_VERSION_NUMBER "1.0"

// === init LCD ===
LiquidCrystal_I2C lcd(0x20, 2,1,0,4,5,6,7,3, POSITIVE);

// === init IR control ===
IRrecv irrecv(PIN_IR_SENSOR);
decode_results ircommand;

// === init DTH11 ===
DHT dht(PIN_DHT11, DHT11);

// === init RTC ===
RTC_DS1307 RTC;


// === setting variables ===
// internal
int lcdDelayTime                   = 5000;
int lcdDelayTimeInPauseMode        = 1000;

// user controlled
// MENU 1. Date and time
// date and time read and write directly on rtc
int timeMode                       = TIME_MODE_24H;

// MENU 2. Aquarium lights
int aquariumLightsStatus           = AQUARIUM_LIGHT_STATUS_OFF;
int aquariumLightsMode             = AQUARIUM_LIGHT_MODE_AUTO;
int aquariumLightsOn1TimeHour      = 9;
int aquariumLightsOn1TimeMinute    = 0;
int aquariumLightsOff1TimeHour     = 13;
int aquariumLightsOff1TimeMinute   = 0;
int aquariumLightsOn2TimeHour      = 15;
int aquariumLightsOn2TimeMinute    = 0;
int aquariumLightsOff2TimeHour     = 21;
int aquariumLightsOff2TimeMinute   = 30;

// MENU 3. Aquarium vent
int aquariumVentStatus             = AQUARIUM_VENT_STATUS_OFF;
int aquariumVentMode               = AQUARIUM_VENT_MODE_AUTO;

// MENU 4. Speaker
int speakerMode                    = SPEAKER_MODE_ON;

// MENU 5. Alarms mode
int alarmsMode                     = ALARMS_MODE_ON;

// MENU 6. LCD
int lcdStatus                      = LCD_STATUS_ON;
int lcdMode                        = LCD_MODE_AUTO;
int lcdTimeout                     = 60;

// MENU 7. Water temperature alarm
int waterTempCriticalLowLimit          = 21;
int waterTempCriticalLowAlarmAction1   = ALARM_VENT_OFF;
int waterTempCriticalLowAlarmAction2   = ALARM_LCD_BLINK;
int waterTempCriticalLowAlarmAction3   = ALARM_BEEP_0SEC;
int waterTempCriticalLowAlarmAction4   = ALARM_NO_ACTION;

int waterTempLowLimit                  = 24;
int waterTempLowAlarmAction1           = ALARM_VENT_OFF;
int waterTempLowAlarmAction2           = ALARM_NO_ACTION;
int waterTempLowAlarmAction3           = ALARM_NO_ACTION;
int waterTempLowAlarmAction4           = ALARM_NO_ACTION;

int waterTempHighLimit                 = 26;
int waterTempHighAlarmAction1          = ALARM_VENT_OFF;
int waterTempHighAlarmAction2          = ALARM_NO_ACTION;
int waterTempHighAlarmAction3          = ALARM_NO_ACTION;
int waterTempHighAlarmAction4          = ALARM_NO_ACTION;

int waterTempCriticalHighLimit         = 29;
int waterTempCriticalHighAlarmAction1  = ALARM_VENT_ON;
int waterTempCriticalHighAlarmAction2  = ALARM_LCD_BLINK;
int waterTempCriticalHighAlarmAction3  = ALARM_BEEP_0SEC;
int waterTempCriticalHighAlarmAction4  = ALARM_AQL_OFF;

// MENU 8. Water level alarm
int waterLevelCriticalLowLimit          = 50;
int waterLevelCriticalLowAlarmAction1   = ALARM_VENT_OFF;
int waterLevelCriticalLowAlarmAction2   = ALARM_LCD_BLINK;
int waterLevelCriticalLowAlarmAction3   = ALARM_BEEP_0SEC;
int waterLevelCriticalLowAlarmAction4   = ALARM_NO_ACTION;

int waterLevelLowLimit                  = 75;
int waterLevelLowAlarmAction1           = ALARM_VENT_OFF;
int waterLevelLowAlarmAction2           = ALARM_LCD_ON;
int waterLevelLowAlarmAction3           = ALARM_BEEP_1MIN;
int waterLevelLowAlarmAction4           = ALARM_NO_ACTION;

int waterLevelHighLimit                 = 100;
int waterLevelHighAlarmAction1          = ALARM_NO_ACTION;
int waterLevelHighAlarmAction2          = ALARM_LCD_ON;
int waterLevelHighAlarmAction3          = ALARM_BEEP_1MIN;
int waterLevelHighAlarmAction4          = ALARM_NO_ACTION;

int waterLevelCriticalHighLimit         = 120;
int waterLevelCriticalHighAlarmAction1  = ALARM_NO_ACTION;
int waterLevelCriticalHighAlarmAction2  = ALARM_LCD_BLINK;
int waterLevelCriticalHighAlarmAction3  = ALARM_BEEP_0SEC;
int waterLevelCriticalHighAlarmAction4  = ALARM_NO_ACTION;

int temporarySettupDataOldValue = 0;
int temporarySettupDataNewValue = 0;
boolean settupChanged = false;

// ==================================
// === internal program variables ===
// ==================================
const int UI_MODE_INFO           = 0;
const int UI_MODE_SETTINGS_MENU  = 1;
const int UI_MODE_SETTUP         = 2;
int userInterfaceMode      = UI_MODE_INFO;

const int SETTUP_MODE_DATE_DAY      =  0;
const int SETTUP_MODE_DATE_MONTH    =  1;
const int SETTUP_MODE_DATE_YEAR     =  2;
const int SETTUP_MODE_TIME_HOUR     =  3;
const int SETTUP_MODE_TIME_MINUTE   =  4;
const int SETTUP_MODE_TIME_MODE     =  5;
const int SETTUP_MODE_AQL_CONTROL   =  6;
const int SETTUP_MODE_AQL_MODE      =  7;
const int SETTUP_MODE_AQL_ON1_HOUR  =  8;
const int SETTUP_MODE_AQL_ON1_MIN   =  9;
const int SETTUP_MODE_AQL_OFF1_HOUR = 10;
const int SETTUP_MODE_AQL_OFF1_MIN  = 11;
const int SETTUP_MODE_AQL_ON2_HOUR  = 12;
const int SETTUP_MODE_AQL_ON2_MIN   = 13;
const int SETTUP_MODE_AQL_OFF2_HOUR = 14;
const int SETTUP_MODE_AQL_OFF2_MIN  = 15;
const int SETTUP_MODE_AQV_CONTROL   = 16;
const int SETTUP_MODE_AQV_MODE      = 17;
const int SETTUP_MODE_SPEAKER_MODE  = 18;
const int SETTUP_MODE_ALARMS_MODE   = 19;
const int SETTUP_MODE_LCD_CONTROL   = 20;
const int SETTUP_MODE_LCD_MODE      = 21;
const int SETTUP_MODE_LCD_TIMEOUT   = 22;
const int SETTUP_MODE_WT_LIM_CLOW   = 23;
const int SETTUP_MODE_WT_ACT_CLOW   = 24;
const int SETTUP_MODE_WT_LIM_LOW    = 25;
const int SETTUP_MODE_WT_ACT_LOW    = 26;
const int SETTUP_MODE_WT_LIM_HIGH   = 27;
const int SETTUP_MODE_WT_ACT_HIGH   = 28;
const int SETTUP_MODE_WT_LIM_CHIGH  = 29;
const int SETTUP_MODE_WT_ACT_CHIGH  = 30;
const int SETTUP_MODE_WL_LIM_CLOW   = 31;
const int SETTUP_MODE_WL_ACT_CLOW   = 32;
const int SETTUP_MODE_WL_LIM_LOW    = 33;
const int SETTUP_MODE_WL_ACT_LOW    = 34;
const int SETTUP_MODE_WL_LIM_HIGH   = 35;
const int SETTUP_MODE_WL_ACT_HIGH   = 36;
const int SETTUP_MODE_WL_LIM_CHIGH  = 37;
const int SETTUP_MODE_WL_ACT_CHIGH  = 38;
int settupMode = SETTUP_MODE_DATE_DAY;

// === LCD Information ===
int displayedPage = 0;
boolean pauseDisplay = false;
long lastDisplayTime = 0;

String info1 = "N/A";
String info2 = "N/A";

// === LCD info screen ===
const int LCD_INFO_MAX_SCREEN     = 14;

// === text constants for info and menu ===
const String lcdInfoDescrEN[] = {"Date and time", "Temperature", "Level", "Ext temperature", "Ext humidity", 
                               "Ext light", "Light", "Ventilation", "LCD", 
                               "Alarms", "Speaker", "Last feed time", "Next feed in", "About"};

const String lcdSettupInfoEN[] = {
    "clock day  =", "clock month=", "clock year =", "clock hour =", "clock min  =", 
    "time mode  =", "lights ctrl=", "lights mode=", "AQL on1 H  =", "AQL on1 M  =", 
    "AQL off1 H =", "AQL off1 M =", "AQL on2 H  =", "AQL on2 M  =", "AQL off2 H =", 
    "AQL off2 M =", "vent ctrl  =", "vent mode  =", "speak.mode =", "alarms mode=", 
    "LCD ctrl   =", "LCD mode   =", "LCD timeout=", "temp.lim.vv=", "temp.act.vv=", 
    "temp.lim.v =", "temp.act.v =", "temp.lim.^ =", "temp.act.^ =", "temp.lim.^^=", 
    "temp.act.^^=", "lvl.lim.vv =", "lvl.act.vv =", "lvl.lim.v  =", "lvl.act.v  =", 
    "lvl.lim.^  =", "lvl.act.^  =", "lvl.lim.^^ =", "lvl.act.^^ ="};
// === LCD Settings Menu ===

// Menu
boolean menuChanged = false;
MenuSystem menuSystem;
Menu mRoot("3A Settings");
Menu m1("Date and Time");
Menu m1_m1("Set date");
MenuItem m1_m1_i1("day:");
MenuItem m1_m1_i2("month:");
MenuItem m1_m1_i3("year:");
Menu m1_m2("Set time");
MenuItem m1_m2_i1("hour:");
MenuItem m1_m2_i2("minute:");
MenuItem m1_m2_i3("mode:");
Menu m2("Aquarium lights");
MenuItem m2_i1("control:");
MenuItem m2_i2("mode:");
Menu m2_m3("Auto");
MenuItem m2_m3_i1("on1 hour:");
MenuItem m2_m3_i2("on1 minute:");
MenuItem m2_m3_i3("off1 hour:");
MenuItem m2_m3_i4("off1 minute:");
MenuItem m2_m3_i5("on2 hour:");
MenuItem m2_m3_i6("on2 minute:");
MenuItem m2_m3_i7("off2 hour:");
MenuItem m2_m3_i8("off2 minute:");
Menu m3("Aquarium vent");
MenuItem m3_i1("control:");
MenuItem m3_i2("mode:");
MenuItem i3("Speaker:");
MenuItem i4("Alarms:");
Menu m5("LCD");
MenuItem m5_i1("control:");
MenuItem m5_i2("mode:");
MenuItem m5_i3("timeout:");
Menu m6("Water temp");
MenuItem m6_i1("Lim. CLow:");
MenuItem m6_i2("Act. CLow:");
MenuItem m6_i3("Lim. Low:");
MenuItem m6_i4("Act. Low:");
MenuItem m6_i5("Lim. High:");
MenuItem m6_i6("Act. High:");
MenuItem m6_i7("Lim. CHigh:");
MenuItem m6_i8("Act. CHigh:");
Menu m7("Water level");
MenuItem m7_i1("Lim. CLow:");
MenuItem m7_i2("Act. CLow:");
MenuItem m7_i3("Lim. Low:");
MenuItem m7_i4("Act. Low:");
MenuItem m7_i5("Lim. High:");
MenuItem m7_i6("Act. High:");
MenuItem m7_i7("Lim. CHigh:");
MenuItem m7_i8("Act. CHigh:");


// === sensor data ===

void setup() {
  RTC.begin();
  
  lcd.begin(16,2);

  dht.begin();
  
  // Start the ir receiver
  pinMode(PIN_IR_SENSOR, INPUT);
  irrecv.enableIRIn();
  
  pinMode(PIN_RELAY_CH_1, OUTPUT);
  pinMode(PIN_RELAY_CH_2, OUTPUT);
  
  displayedPage = LCD_INFO_MAX_SCREEN;

  // Menu setup
  initSettupMenu();
  
  pinMode(PIN_SOUND, OUTPUT);
  toneManual(NOTE_C4, 200);
  delay(200);
}

void initSettupMenu() {
  mRoot.add_menu(&m1);
  m1.add_menu(&m1_m1);
  m1_m1.add_item(&m1_m1_i1, &onSelect, SETTUP_MODE_DATE_DAY);
  m1_m1.add_item(&m1_m1_i2, &onSelect, SETTUP_MODE_DATE_MONTH);
  m1_m1.add_item(&m1_m1_i3, &onSelect, SETTUP_MODE_DATE_YEAR);
  m1.add_menu(&m1_m2);
  m1_m2.add_item(&m1_m2_i1, &onSelect, SETTUP_MODE_TIME_HOUR);
  m1_m2.add_item(&m1_m2_i2, &onSelect, SETTUP_MODE_TIME_MINUTE);
  m1_m2.add_item(&m1_m2_i3, &onSelect, SETTUP_MODE_TIME_MODE);
  mRoot.add_menu(&m2);
  m2.add_item(&m2_i1, &onSelect, SETTUP_MODE_AQL_CONTROL);
  m2.add_item(&m2_i2, &onSelect, SETTUP_MODE_AQL_MODE);
  m2.add_menu(&m2_m3);
  m2_m3.add_item(&m2_m3_i1, &onSelect, SETTUP_MODE_AQL_ON1_HOUR);
  m2_m3.add_item(&m2_m3_i2, &onSelect, SETTUP_MODE_AQL_ON1_MIN);
  m2_m3.add_item(&m2_m3_i3, &onSelect, SETTUP_MODE_AQL_OFF1_HOUR);
  m2_m3.add_item(&m2_m3_i4, &onSelect, SETTUP_MODE_AQL_OFF1_MIN);
  m2_m3.add_item(&m2_m3_i5, &onSelect, SETTUP_MODE_AQL_ON2_HOUR);
  m2_m3.add_item(&m2_m3_i6, &onSelect, SETTUP_MODE_AQL_ON2_MIN);
  m2_m3.add_item(&m2_m3_i7, &onSelect, SETTUP_MODE_AQL_OFF2_HOUR);
  m2_m3.add_item(&m2_m3_i8, &onSelect, SETTUP_MODE_AQL_OFF2_MIN);
  mRoot.add_menu(&m3);
  m2.add_item(&m3_i1, &onSelect, SETTUP_MODE_AQV_CONTROL);
  m2.add_item(&m3_i2, &onSelect, SETTUP_MODE_AQV_MODE);
  mRoot.add_item(&i3, &onSelect, SETTUP_MODE_SPEAKER_MODE);
  mRoot.add_item(&i4, &onSelect, SETTUP_MODE_ALARMS_MODE);
  mRoot.add_menu(&m5);
  m5.add_item(&m5_i1, &onSelect, SETTUP_MODE_LCD_CONTROL);
  m5.add_item(&m5_i2, &onSelect, SETTUP_MODE_LCD_MODE);
  m5.add_item(&m5_i3, &onSelect, SETTUP_MODE_LCD_TIMEOUT);
  mRoot.add_menu(&m6);
  m6.add_item(&m6_i1, &onSelect, SETTUP_MODE_WT_LIM_CLOW);
  m6.add_item(&m6_i2, &onSelect, SETTUP_MODE_WT_ACT_CLOW);
  m6.add_item(&m6_i3, &onSelect, SETTUP_MODE_WT_LIM_LOW);
  m6.add_item(&m6_i4, &onSelect, SETTUP_MODE_WT_ACT_LOW);
  m6.add_item(&m6_i5, &onSelect, SETTUP_MODE_WT_LIM_HIGH);
  m6.add_item(&m6_i6, &onSelect, SETTUP_MODE_WT_ACT_HIGH);
  m6.add_item(&m6_i7, &onSelect, SETTUP_MODE_WT_LIM_CHIGH);
  m6.add_item(&m6_i8, &onSelect, SETTUP_MODE_WT_ACT_CHIGH);
  mRoot.add_menu(&m7);
  m7.add_item(&m7_i1, &onSelect, SETTUP_MODE_WL_LIM_CLOW);
  m7.add_item(&m7_i2, &onSelect, SETTUP_MODE_WL_ACT_CLOW);
  m7.add_item(&m7_i3, &onSelect, SETTUP_MODE_WL_LIM_LOW);
  m7.add_item(&m7_i4, &onSelect, SETTUP_MODE_WL_ACT_LOW);
  m7.add_item(&m7_i5, &onSelect, SETTUP_MODE_WL_LIM_HIGH);
  m7.add_item(&m7_i6, &onSelect, SETTUP_MODE_WL_ACT_HIGH);
  m7.add_item(&m7_i7, &onSelect, SETTUP_MODE_WL_LIM_CHIGH);
  m7.add_item(&m7_i8, &onSelect, SETTUP_MODE_WL_ACT_CHIGH);

  menuSystem.set_root_menu(&mRoot);
}

void onSelect(MenuItem* menuItem) {
  //char* menuItemName = menuItem->get_name();
  int menuItemActionCode = menuItem->get_action_code();
  settupMode = menuItemActionCode;
  userInterfaceMode = UI_MODE_SETTUP;
  temporarySettupDataOldValue = getCurrentSettupDataValue();
  temporarySettupDataNewValue = temporarySettupDataOldValue;
}

int getCurrentSettupDataValue() {
  int value = -1;
  switch (settupMode) {
    case SETTUP_MODE_DATE_DAY      : value = readDateDay();break;
    case SETTUP_MODE_DATE_MONTH    : value = readDateMonth();break;
    case SETTUP_MODE_DATE_YEAR     : value = readDateYear();break;
    case SETTUP_MODE_TIME_HOUR     : value = readTimeHour();break;
    case SETTUP_MODE_TIME_MINUTE   : value = readTimeMinute();break;
    case SETTUP_MODE_TIME_MODE     : value = timeMode;break;
    case SETTUP_MODE_AQL_CONTROL   : value = aquariumLightsStatus;break;
    case SETTUP_MODE_AQL_MODE      : value = aquariumLightsMode;break;
    case SETTUP_MODE_AQL_ON1_HOUR  : value = aquariumLightsOn1TimeHour;break;
    case SETTUP_MODE_AQL_ON1_MIN   : value = aquariumLightsOn1TimeMinute;break;
    case SETTUP_MODE_AQL_OFF1_HOUR : value = aquariumLightsOff1TimeHour;break;
    case SETTUP_MODE_AQL_OFF1_MIN  : value = aquariumLightsOff1TimeMinute;break;
    case SETTUP_MODE_AQL_ON2_HOUR  : value = aquariumLightsOn2TimeHour;break;
    case SETTUP_MODE_AQL_ON2_MIN   : value = aquariumLightsOn2TimeMinute;break;
    case SETTUP_MODE_AQL_OFF2_HOUR : value = aquariumLightsOff2TimeHour;break;
    case SETTUP_MODE_AQL_OFF2_MIN  : value = aquariumLightsOff2TimeMinute;break;
    case SETTUP_MODE_AQV_CONTROL   : value = aquariumVentStatus;break;
    case SETTUP_MODE_AQV_MODE      : value = aquariumVentMode;break;
    case SETTUP_MODE_SPEAKER_MODE  : value = speakerMode;break;
    case SETTUP_MODE_ALARMS_MODE   : value = alarmsMode;break;
    case SETTUP_MODE_LCD_CONTROL   : value = lcdStatus;break;
    case SETTUP_MODE_LCD_MODE      : value = lcdMode;break;
    case SETTUP_MODE_LCD_TIMEOUT   : value = lcdTimeout;break;
    case SETTUP_MODE_WT_LIM_CLOW   : value = waterTempCriticalLowLimit;break;
    case SETTUP_MODE_WT_ACT_CLOW   : value = 0;break;
    case SETTUP_MODE_WT_LIM_LOW    : value = waterTempLowLimit;break;
    case SETTUP_MODE_WT_ACT_LOW    : value = 0;break;
    case SETTUP_MODE_WT_LIM_HIGH   : value = waterTempHighLimit;break;
    case SETTUP_MODE_WT_ACT_HIGH   : value = 0;break;
    case SETTUP_MODE_WT_LIM_CHIGH  : value = waterTempCriticalHighLimit;break;
    case SETTUP_MODE_WT_ACT_CHIGH  : value = 0;break;
    case SETTUP_MODE_WL_LIM_CLOW   : value = waterLevelCriticalLowLimit;break;
    case SETTUP_MODE_WL_ACT_CLOW   : value = 0;break;
    case SETTUP_MODE_WL_LIM_LOW    : value = waterLevelLowLimit;break;
    case SETTUP_MODE_WL_ACT_LOW    : value = 0;break;
    case SETTUP_MODE_WL_LIM_HIGH   : value = waterLevelHighLimit;break;
    case SETTUP_MODE_WL_ACT_HIGH   : value = 0;break;
    case SETTUP_MODE_WL_LIM_CHIGH  : value = waterLevelCriticalHighLimit;break;
    case SETTUP_MODE_WL_ACT_CHIGH  : value = 0;break;
  }
  return value;
}

void setCurrentSettupDataValue(int value) {
}

void loop() {
  checkUserCommand();
  
  if (userInterfaceMode == UI_MODE_INFO) {
    displayOnLCD();
    
  } else if (userInterfaceMode == UI_MODE_SETTINGS_MENU) {
    displayMenu();
    
  } else if (userInterfaceMode == UI_MODE_SETTUP) {
    displaySettupControl();
  }
  
  runRules();
}

void runRules() {
  // IN  : date, time, water temp, water level, room temp, room hum, ext light, alarm status, speaker status
  // OUT : aq lights, ventilation, feeder, lcd light, speaker, 
  // LIMITS / TRIGERS : date/time limits, temp limits, level limits, external light level, settings for alarms and speaker
  
  // MONITOR : water temp 
  // --> ventilation : if wt<24 then vent=off; if wt>=24 then vent=on;
  // --> if daytime & wt>=28 then lcd=on ; if daytime & wt>=29 then lcd=on & speaker=beep/5min & aq light=off;
  // --> if nighttime & wt>=30 then (CRITICAL:) lcd=blink/1sec & speaker=beep & aq light=off & aq light=blink/1min;
  
  // MONITOR : water level
  // --> if wl<20% then (CRITICAL:)
  // --> if wl<80% then lcd=on
  
  // MONITOR : time for aq lights
  // --> if time is one of start lights then aq light=on
  // --> if time is one of end lights then aq light=off
  int hour = readTimeHour();
  int minute = readTimeMinute();

  // check start time
  if ((aquariumLightsOn1TimeHour == hour && aquariumLightsOn1TimeMinute == minute) || 
      (aquariumLightsOn2TimeHour == hour && aquariumLightsOn2TimeMinute == minute)) {
    aquariumLightsStatus = AQUARIUM_LIGHT_STATUS_ON;
    digitalWrite(PIN_RELAY_CH_1, HIGH);
  }
  
  // check stop time
  if ((aquariumLightsOff1TimeHour == hour && aquariumLightsOff1TimeMinute == minute) || 
      (aquariumLightsOff2TimeHour == hour && aquariumLightsOff2TimeMinute == minute)) {
    aquariumLightsStatus = AQUARIUM_LIGHT_STATUS_OFF;
    digitalWrite(PIN_RELAY_CH_1, LOW);
  }
  
  // MONITOR : 
}

void checkUserCommand() {
  if (irrecv.decode(&ircommand)) {
    unsigned long command = ircommand.value;

    checkGeneralIRCommand(command);
    
    if (userInterfaceMode == UI_MODE_INFO) {
      checkInfoIRCommand(command);
      
    } else if (userInterfaceMode == UI_MODE_SETTINGS_MENU) {
      checkSettingsMenuIRCommand(command);
      
    } else if (userInterfaceMode == UI_MODE_SETTUP) {
      checkSettupIRCommand(command);
    }

    irrecv.resume(); // Receive the next value
  } else {
    //Serial.println("ERROR decoding!");
  }
}

void checkGeneralIRCommand(unsigned long command) {
    // use int: ircommand.value
    if (command == KEY_ON_OFF) { // on/off
      if (lcdStatus == LCD_STATUS_ON) {
        lcdStatus = LCD_STATUS_OFF;
        lcd.noBacklight();
      } else if (lcdStatus == LCD_STATUS_OFF) {
        lcdStatus = LCD_STATUS_ON;
        lcd.backlight();
      }
      lastDisplayTime = 0;
      displayedPage = LCD_INFO_LCD_STATUS - 1;
    }
    
    // switch mode between INFO and SETTINGS MENU
    if (command == KEY_MODE) { // mode
      if (userInterfaceMode == UI_MODE_INFO) {
        userInterfaceMode = UI_MODE_SETTINGS_MENU;
        menuChanged = true;
      } else if (userInterfaceMode == UI_MODE_SETTINGS_MENU) {
        userInterfaceMode = UI_MODE_INFO;
      }
    }
    
}

void checkInfoIRCommand(unsigned long command) {
    if (command == KEY_PLAY_PAUSE) { // play/pause
      pauseDisplay = !pauseDisplay;
    }
    
    if (command == KEY_PREV) { // prev
      lastDisplayTime = 0;
      if (!pauseDisplay) {
        displayedPage = displayedPage - 2;
      } else {
        displayedPage = displayedPage - 1;
      }
      if (displayedPage == -2) displayedPage = LCD_INFO_REMAIN_TO_FEED;
      if (displayedPage == -1) displayedPage = LCD_INFO_DESPRE;
    }
    
    if (command == KEY_NEXT) { // next
      lastDisplayTime = 0;
      if (!pauseDisplay) {
        displayedPage = displayedPage + 0;
      } else {
        displayedPage = displayedPage + 1;
      }
      if (displayedPage >= LCD_INFO_MAX_SCREEN) displayedPage = LCD_INFO_DATE_TIME;
    }

    if (command == KEY_0) { // key 0
      if (aquariumLightsStatus == AQUARIUM_LIGHT_STATUS_ON) {
        aquariumLightsStatus = AQUARIUM_LIGHT_STATUS_OFF;
        digitalWrite(PIN_RELAY_CH_1, LOW);
      } else if (aquariumLightsStatus == AQUARIUM_LIGHT_STATUS_OFF) {
        aquariumLightsStatus = AQUARIUM_LIGHT_STATUS_ON;
        digitalWrite(PIN_RELAY_CH_1, HIGH);
      }
      lastDisplayTime = 0;
      displayedPage = LCD_INFO_AQ_LIGHT;
      if (!pauseDisplay) displayedPage = displayedPage - 1;
    }
    
    if (command == KEY_1) { // key 1
      if (aquariumVentStatus == AQUARIUM_VENT_STATUS_ON) {
        aquariumVentStatus = AQUARIUM_VENT_STATUS_OFF;
        digitalWrite(PIN_RELAY_CH_2, LOW);
      } else if (aquariumVentStatus == AQUARIUM_VENT_STATUS_OFF) {
        aquariumVentStatus = AQUARIUM_VENT_STATUS_ON;
        digitalWrite(PIN_RELAY_CH_2, HIGH);
      }
      lastDisplayTime = 0;
      displayedPage = LCD_INFO_AQ_VENTILATION;
      if (!pauseDisplay) displayedPage = displayedPage - 1;
    }

}

void checkSettingsMenuIRCommand(unsigned long command) {
    if (command == KEY_MUTE) { // mute
      menuSystem.back();
      menuChanged = true;
    }
    
    if (command == KEY_PLAY_PAUSE) { // play/pause
      menuSystem.select();
      menuChanged = true;
      settupChanged = true;
    }
    
    if (command == KEY_PREV) { // prev
      menuSystem.prev();
      menuChanged = true;
    }
    
    if (command == KEY_NEXT) { // next
      menuSystem.next();
      menuChanged = true;
    }
}

void checkSettupIRCommand(unsigned long command) {
    if (command == KEY_MUTE) { // mute = cancel
      lcd.setCursor(0, 1); lcd.print("<   CANCELED   >");
      delay(1000);
      userInterfaceMode = UI_MODE_SETTINGS_MENU;
      //menuSystem.back();
      menuChanged = true;
    }
    
    if (command == KEY_PLAY_PAUSE) { // play/pause = save
      temporarySettupDataOldValue = temporarySettupDataNewValue;
      settupChanged = true;
    }
    
    if (command == KEY_PREV) { // prev = -value
      temporarySettupDataNewValue--;
      settupChanged = true;
    }
    
    if (command == KEY_NEXT) { // next = +value
      temporarySettupDataNewValue++;
      settupChanged = true;
    }
    
}

void displayOnLCD() {
  long currentTime = millis();
  long timePassed = currentTime - lastDisplayTime;
  
  if (timePassed >= lcdDelayTime || lastDisplayTime == 0 || (pauseDisplay && timePassed >= lcdDelayTimeInPauseMode)) {
    lastDisplayTime = currentTime;
    
    if (!pauseDisplay) {
      displayedPage = displayedPage + 1;
    }
    if (displayedPage >= LCD_INFO_MAX_SCREEN) displayedPage = LCD_INFO_DATE_TIME;
    
    String lcd_1 = lcdInfoDescrEN[displayedPage];
    String lcd_2 = "n/a";

    switch (displayedPage) {
      case LCD_INFO_DATE_TIME:
        lcd_2 = readDateAndTime();
        break;
      case LCD_INFO_WATER_TEMP:
        lcd_2 = String(readWaterTemp());
        break;
      case LCD_INFO_WATER_LEVEL:
        lcd_2 = String(readWaterLevel());
        break;
      case LCD_INFO_ROOM_TEMP:
        lcd_2 = String(readExtTemp());
        break;
      case LCD_INFO_ROOM_HUMIDITY:
        lcd_2 = String(readExtHum());
        break;
      case LCD_INFO_EXT_LIGHT:
        lcd_2 = String(readExtLight());
        break;
      case LCD_INFO_AQ_LIGHT:
        lcd_2 = getAquariumLightInfo();
        break;
      case LCD_INFO_AQ_VENTILATION:
        lcd_2 = getAquariumVentInfo();
        break;
      case LCD_INFO_LCD_STATUS:
        lcd_2 = getLCDInfo();
        break;
      case LCD_INFO_ALARMS_STATUS:
        lcd_2 = getAlarmsInfo();
        break;
      case LCD_INFO_SPEAKER_STATUS:
        lcd_2 = getSpeakerInfo();
        break;
      case LCD_INFO_LAST_FEED_TIME:
        break;
      case LCD_INFO_REMAIN_TO_FEED:
        break;
      case LCD_INFO_DESPRE:
        lcd_1 = " 3A - ver.1.2";// + APP_VERSION_NUMBER;
        lcd_2 = "(c)2014 ValiP";
        break;
    }

    lcd.clear();
    lcd.setCursor(0, 0);lcd.print(lcd_1);
    lcd.setCursor(0, 1);lcd.print(lcd_2);
  }
}

void displayMenu() {
  if (menuChanged) {
    lcd.clear();
  
    Menu const* cp_menu = menuSystem.get_current_menu();
  
    lcd.setCursor(0, 0);
    lcd.print(cp_menu->get_name());
    
    lcd.setCursor(0, 1);
    lcd.print(cp_menu->get_selected()->get_name());
    
    menuChanged = false;
  }
}

void displaySettupControl() {
  if (settupChanged) {
    lcd.clear();
    // first line is info: what is changing and old value
    // "                "
    lcd.setCursor(0, 0);
    String info = lcdSettupInfoEN[settupMode];
    lcd.print(info + temporarySettupDataOldValue);
    // second line is (<) =value (>)
    // "                "
    lcd.setCursor(0, 1); lcd.print("<");
    lcd.setCursor(5, 1); lcd.print(temporarySettupDataNewValue);
    lcd.setCursor(15,1); lcd.print(">");
    settupChanged = false;
  }
}

// === Sensors data read methods ===
int readExtLight() {
  return analogRead(PIN_EXT_LIGHT);
}

int readExtTemp() {
  return (int)dht.readTemperature();
}

int readExtHum() {
  return (int)dht.readHumidity();
}

int readWaterLevel() {
  return analogRead(PIN_WATER_LEVEL);
}

int readWaterTemp() {
  int rawADC = analogRead(PIN_WATER_TEMP);
  long resistance;
  float pad = 10000;
  float temp;  // Dual-Purpose variable to save space.

  resistance=((1024 * pad / rawADC) - pad); 
  temp = log(resistance); // Saving the Log(resistance) so not to calculate  it 4 times later
  temp = 1 / (0.001129148 + (0.000234125 * temp) + (0.0000000876741 * temp * temp * temp));
  temp = temp - 273.15; // Convert Kelvin to Celsius                      
  // Uncomment this line for the function to return Fahrenheit instead.
  //temp = (temp * 9.0)/ 5.0 + 32.0; // Convert to Fahrenheit
  return (int)temp; // Return the Temperature
}

String getAquariumLightInfo() {
  if (aquariumLightsStatus == AQUARIUM_LIGHT_STATUS_OFF) {
    info1 = "OFF";
  } else if (aquariumLightsStatus == AQUARIUM_LIGHT_STATUS_ON) {
    info1 = "ON";
  }
  if (aquariumLightsMode == AQUARIUM_LIGHT_MODE_AUTO) {
    info2 = "AUTO";
  } else if (aquariumLightsMode == AQUARIUM_LIGHT_MODE_MANUAL) {
    info2 = "MANUAL";
  }
  return info1 + " - " + info2;
}

String getAquariumVentInfo() {
  if (aquariumVentStatus == AQUARIUM_VENT_STATUS_OFF) {
    info1 = "OFF";
  } else if (aquariumVentStatus == AQUARIUM_VENT_STATUS_ON) {
    info1 = "ON";
  }
  if (aquariumVentMode == AQUARIUM_VENT_MODE_AUTO) {
    info2 = "AUTO";
  } else if (aquariumVentMode == AQUARIUM_VENT_MODE_MANUAL) {
    info2 = "MANUAL";
  }
  return info1 + " - " + info2;
}

String getLCDInfo() {
  if (lcdStatus == LCD_STATUS_OFF) {
    info1 = "OFF";
  } else if (lcdStatus == LCD_STATUS_ON) {
    info1 = "ON - Timeout=" + String(lcdTimeout);
  }
  if (lcdMode == LCD_MODE_AUTO) {
    info2 = "AUTO";
  } else if (lcdMode == LCD_MODE_MANUAL) {
    info2 = "MANUAL";
  }
  return info1 + " - " + info2;
}

String getAlarmsInfo() {
  if (alarmsMode == ALARMS_MODE_OFF) {
    info1 = "OFF";
  } else if (alarmsMode == ALARMS_MODE_ON) {
    info1 = "ON";
  } else if (alarmsMode == ALARMS_MODE_CRITICAL) {
    info1 = "CRITICAL";
  }
  return info1;
}

String getSpeakerInfo() {
  if (speakerMode == SPEAKER_MODE_OFF) {
    info1 = "OFF";
  } else if (speakerMode == SPEAKER_MODE_ON) {
    info1 = "ON";
  } else if (speakerMode == SPEAKER_MODE_ALARMS) {
    info1 = "ALARMS";
  } else if (speakerMode == SPEAKER_MODE_CRITICAL) {
    info1 = "CRITICAL";
  }
  return info1;
}

String readDateAndTime() {
  DateTime now = RTC.now();
  int ih = now.hour();
  int im = now.minute();
  int id = now.day();
  int iM = now.month();
  int iy = now.year();
  
  String sh = String(ih);
  String sm = String(im);
  String sd = String(id);
  String sM = String(iM);
  String sy = String(iy);
  
  if (ih < 10) sh = "0" + sh;
  if (im < 10) sm = "0" + sm;
  if (id < 10) sd = "0" + sd;
  if (iM < 10) sM = "0" + sM;
  
  return sh + ":" + sm + " " + sd + "." + sM + "." + sy;
}

int readDateDay() {
  DateTime now = RTC.now();
  return now.day();
}

int readDateMonth() {
  DateTime now = RTC.now();
  return now.month();
}

int readDateYear() {
  DateTime now = RTC.now();
  return now.year();
}

int readTimeHour() {
  DateTime now = RTC.now();
  return now.hour();
}

int readTimeMinute() {
  DateTime now = RTC.now();
  return now.minute();
}

// === UTILITY METHODS ===
void toneManual(int frequency, int duration)
{
  unsigned long period = 1000000/frequency;
  unsigned long length;
  boolean state = false;
  for (length = 0; length < (long) duration * 1000; length += period) {
    state = !state;
    digitalWrite(PIN_SOUND, state);
    /* The 50uS correspond to the time the rest of the loop body takes.
     * It seems about right, but has not been tuned precisely for
     * a 16MHz ATMega. */
    delayMicroseconds(period - 50);
  }
}



//- lights (L) and ventilation (V) can be controlled manualy (M) or auto (A)
//- if L and V are A then are turned ON / OFF in next order of priorities:
//	I.   if alarms are OFF then L and V mode is set to M
//	II.  if alarms are CRITICAL then:
//			- critical alarms are checked and L and V are turned ON/OFF based on rules
//			- if no critical limits reached then L and V are turned ON/OFF based on L auto times
//	III. if alarms are ON then:
//			- critical alarms are checked and rules have priority
//			- low/high limits alarms are checkd and rules have priority
//			- if no alarm is turned on (no limits reached) then L and V are turned based on L auto times
//
//
//RULE: 
//	If L and/or V mode is A then Alarms cannot be set to OFF! - if alarms are off they are set automaticaly to ON!
//	If Alarms are set to OFF then L and V cannot be set to A! - if L/V are set to A they will be reset to M!

