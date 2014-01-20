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


// IN  : date, time, water temp, water level, room temp, room hum, ext light, alarm status, speaker status
// OUT : aq lights, ventilation, feeder, lcd light, speaker, 
// LIMITS / TRIGERS : date/time limits, temp limits, level limits, external light level, settings for alarms and speaker

boolean aquariumLightsStatusChanged = false;

void runRules() {
    boolean ok = true;
    
    if (ok) ok = monitorWaterTempLimits();
    
    if (ok) ok = monitorWaterLevelLimits();
    
    if (ok) {
       monitorTimeForAquariumLights();
       monitorLcdTimeOut();
    }
    
    monitorLcdBlink();
}


boolean monitorTimeForAquariumLights() {
    int hour = readTimeHour();
    int minute = readTimeMinute();
    if (!aquariumLightsStatusChanged) {
        boolean alarmsOffOrCritical = alarmsMode == ALARMS_MODE_CRITICAL || alarmsMode == ALARMS_MODE_OFF;
      
        // check start time
        if ((aquariumLightsOn1TimeHour == hour && aquariumLightsOn1TimeMinute == minute) || (aquariumLightsOn2TimeHour == hour && aquariumLightsOn2TimeMinute == minute)) {
            turnAquariumLightsOn();
            
            if (alarmsOffOrCritical && aquariumVentMode == AQUARIUM_VENT_MODE_AUTO) {
                turnAquariumVentOn();
            }
            
            if (lcdMode == LCD_MODE_AUTO) {
                turnLcdOn();
            }
            
            aquariumLightsStatusChanged = true;
        }
        
        // check stop time
        if ((aquariumLightsOff1TimeHour == hour && aquariumLightsOff1TimeMinute == minute) || (aquariumLightsOff2TimeHour == hour && aquariumLightsOff2TimeMinute == minute)) {
            turnAquariumLightsOff();
            
            if (alarmsOffOrCritical && aquariumVentMode == AQUARIUM_VENT_MODE_AUTO) {
                turnAquariumVentOff();
            }
            
            if (lcdMode == LCD_MODE_AUTO) {
                turnLcdOn();
            }
            
            aquariumLightsStatusChanged = true;
        }
    } else {
        if ((aquariumLightsOn1TimeHour == hour && aquariumLightsOn1TimeMinute+1 == minute) || (aquariumLightsOn2TimeHour == hour && aquariumLightsOn2TimeMinute+1 == minute) ||
            (aquariumLightsOff1TimeHour == hour && aquariumLightsOff1TimeMinute+1 == minute) || (aquariumLightsOff2TimeHour == hour && aquariumLightsOff2TimeMinute+1 == minute) ) {
            aquariumLightsStatusChanged = false;
        }
    }
}

boolean monitorLcdTimeOut() {
    if (lcdMode == LCD_MODE_AUTO) {
        if (lcdTimeoutStartTime != 0 && millis() - lcdTimeoutStartTime >= lcdTimeout * 1000) {
            turnLcdOff();
        }
    }
}

void monitorLcdBlink() {
    if (lcdBlinkTime > 0 && millis() - lastLcdBlinkTime >= lcdBlinkTime) {
        blinkLcd();
        lastLcdBlinkTime = millis();
    }
}

boolean monitorWaterTempLimits() {
    boolean ok = true;
    int waterTemp = readWaterTemp();
    if (alarmsMode == ALARMS_MODE_CRITICAL || alarmsMode == ALARMS_MODE_ON) {
        if (waterTemp < waterTempCriticalLowLimit) { // waterTempCriticalLowAlarmAction 1-4
            runAlarm(waterTempCriticalLowAlarmAction1);
            runAlarm(waterTempCriticalLowAlarmAction2);
            runAlarm(waterTempCriticalLowAlarmAction3);
            runAlarm(waterTempCriticalLowAlarmAction4);
            ok = false;
            
        } else if (waterTemp < waterTempLowLimit) {
            if (alarmsMode == ALARMS_MODE_ON) {
                runAlarm(waterTempLowAlarmAction1);
                runAlarm(waterTempLowAlarmAction2);
                runAlarm(waterTempLowAlarmAction3);
                runAlarm(waterTempLowAlarmAction4);
                ok = false;
            }
            
        } else if (waterTemp > waterTempCriticalHighLimit) {
            runAlarm(waterTempCriticalHighAlarmAction1);
            runAlarm(waterTempCriticalHighAlarmAction2);
            runAlarm(waterTempCriticalHighAlarmAction3);
            runAlarm(waterTempCriticalHighAlarmAction4);
            ok = false;

        } else if (waterTemp > waterTempHighLimit) {
            if (alarmsMode == ALARMS_MODE_ON) {
                runAlarm(waterTempHighAlarmAction1);
                runAlarm(waterTempHighAlarmAction2);
                runAlarm(waterTempHighAlarmAction3);
                runAlarm(waterTempHighAlarmAction4);
                ok = false;
            }
        }
    }
    return ok;
}

boolean monitorWaterLevelLimits() {
    boolean ok = true;
    int waterLevel = readWaterLevel();
    if (alarmsMode == ALARMS_MODE_CRITICAL || alarmsMode == ALARMS_MODE_ON) {
        if (waterLevel < waterLevelCriticalLowLimit) { // waterLevelCriticalLowAlarmAction 1-4
            runAlarm(waterLevelCriticalLowAlarmAction1);
            runAlarm(waterLevelCriticalLowAlarmAction2);
            runAlarm(waterLevelCriticalLowAlarmAction3);
            runAlarm(waterLevelCriticalLowAlarmAction4);
            ok = false;
            
        } else if (waterLevel < waterLevelLowLimit) {
            if (alarmsMode == ALARMS_MODE_ON) {
                runAlarm(waterLevelLowAlarmAction1);
                runAlarm(waterLevelLowAlarmAction2);
                runAlarm(waterLevelLowAlarmAction3);
                runAlarm(waterLevelLowAlarmAction4);
                ok = false;
            }
            
        } else if (waterLevel > waterLevelCriticalHighLimit) {
            runAlarm(waterLevelCriticalHighAlarmAction1);
            runAlarm(waterLevelCriticalHighAlarmAction2);
            runAlarm(waterLevelCriticalHighAlarmAction3);
            runAlarm(waterLevelCriticalHighAlarmAction4);
            ok = false;

        } else if (waterLevel > waterLevelHighLimit) {
            if (alarmsMode == ALARMS_MODE_ON) {
                runAlarm(waterLevelHighAlarmAction1);
                runAlarm(waterLevelHighAlarmAction2);
                runAlarm(waterLevelHighAlarmAction3);
                runAlarm(waterLevelHighAlarmAction4);
                ok = false;
            }
        }
    }
    return ok;
}



void runAlarm(int alarmCode) {
    switch (alarmCode) {
        case ALARM_NO_ACTION        : break;
        case ALARM_VENT_ON          : turnAquariumVentOn(); break;
        case ALARM_VENT_OFF         : turnAquariumVentOff(); break;
        case ALARM_LCD_ON           : turnLcdOn(); break;
        case ALARM_LCD_OFF          : turnLcdOff(); break;
        case ALARM_LCD_BLINK        : blinkLcd(); break;
        case ALARM_BEEP_0SEC        : turnSpeakerBeepOnContinuous(); break;
        case ALARM_BEEP_5SEC        : turnSpeakerBeepOn5Sec(); break;
        case ALARM_BEEP_1MIN        : turnSpeakerBeepOn1Min(); break;
        case ALARM_BEEP_5MIN        : turnSpeakerBeepOn5Min(); break;
        case ALARM_AQL_ON           : turnAquariumLightsOn(); break;
        case ALARM_AQL_OFF          : turnAquariumLightsOff(); break;
        case ALARM_AQL_BLINK_OFF    : blinkAquariumLightsWithOff(); break;
        case ALARM_AQL_BLINK_ON     : blinkAquariumLightsWithOn(); break;
    }
}
