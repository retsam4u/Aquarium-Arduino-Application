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

