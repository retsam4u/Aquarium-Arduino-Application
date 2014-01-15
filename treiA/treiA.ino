/*
* Aquarium Application for Arduino
* ver. 2.0
* Changes:
* > version 2.0:
*     - MAJOR: restructure project: brake main ino file (treiA.ino) into multiple files!
*     - create value-description pairs for variable values that have meaning 
*       (ex. for aquariumLightsStatus make pairs: AQUARIUM_LIGHT_STATUS_OFF=0-"OFF";AQUARIUM_LIGHT_STATUS_ON=1-"ON")
*     - make value limits (ex. for setting time->minutes limits are: 0 and 59)
* > version 1.3:
*     - make change value work in selection menu (write setCurrentSettupDataValue method)
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
#include "settup_modes.h"
#include <MenuSystem.h>

//#include <InfoController.ino>
//#include <InfoActionController.ino>

//#include <MenuController.ino>
//#include <MenuActionController.ino>

//#include <SelectionController.ino>
//#include <SelectionActionController.ino>


#define APP_VERSION_NUMBER "2.0"

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

// ==================================
// === internal program variables ===
// ==================================
const int UI_MODE_INFO           = 0;
const int UI_MODE_SETTINGS_MENU  = 1;
const int UI_MODE_SETTUP         = 2;
int userInterfaceMode      = UI_MODE_INFO;


String info1 = "N/A";
String info2 = "N/A";


void setup() {
    RTC.begin();
    
    lcd.begin(16,2);
    
    dht.begin();
    
    // Start the ir receiver
    pinMode(PIN_IR_SENSOR, INPUT);
    irrecv.enableIRIn();
    
    pinMode(PIN_RELAY_CH_1, OUTPUT);
    pinMode(PIN_RELAY_CH_2, OUTPUT);
    
    // Info setup
    initInfoDisplay();
    
    // Menu setup
    initSettupMenu();
    
    pinMode(PIN_SOUND, OUTPUT);
    toneManual(NOTE_C4, 200);
    delay(200);
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
        
        gotoInfoPage(LCD_INFO_LCD_STATUS);
    }
    
    // switch mode between INFO and SETTINGS MENU
    if (command == KEY_MODE) { // mode
        if (userInterfaceMode == UI_MODE_INFO) {
            userInterfaceMode = UI_MODE_SETTINGS_MENU;
            setMenuChanged(true);
        } else if (userInterfaceMode == UI_MODE_SETTINGS_MENU) {
            userInterfaceMode = UI_MODE_INFO;
        }
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

void writeDateDay(int value) {
}

void writeDateMonth(int value) {
}

void writeDateYear(int value) {
}

void writeTimeHour(int value) {
}

void writeTimeMinute(int value) {
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

