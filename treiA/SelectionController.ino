int settupMode = SETTUP_MODE_DATE_DAY;

int temporarySettupDataOldValue = 0;
int temporarySettupDataNewValue = 0;
boolean settupChanged = false;

// max 12 chars (4 are left to old value)
const String lcdSettupInfoEN[] = {
    "clock day  =", "clock month=", "clock year =", "clock hour =", "clock min  =", 
    "time mode  =", "lights ctrl=", "lights mode=", "AQL on1 H  =", "AQL on1 M  =", 
    "AQL off1 H =", "AQL off1 M =", "AQL on2 H  =", "AQL on2 M  =", "AQL off2 H =", 
    "AQL off2 M =", "vent ctrl  =", "vent mode  =", "speak.mode =", "alarms mode=", 
    "LCD ctrl   =", "LCD mode   =", "LCD timeout=", "temp.lim.\xCF\xC6=", "temp.act.\xCF\xC6=", 
    "temp.lim.\xC6 =", "temp.act.\xC6 =", "temp.lim.\xC5 =", "temp.act.\xC5 =", "temp.lim.\xCF\xC5=", 
    "temp.act.\xCF\xC5=", "lvl.lim.\xCF\xC6 =", "lvl.act.\xCF\xC6 =", "lvl.lim. \xC6 =", "lvl.act. \xC6 =", 
    "lvl.lim. \xC5 =", "lvl.act. \xC5 =", "lvl.lim.\xCF\xC5 =", "lvl.act.\xCF\xC5 ="};
    
// === Business ===
void initDisplaySettupControl() {
    setTemporarySettupDataOldValue(getCurrentSettupDataValue());
    setTemporarySettupDataNewValue(getTemporarySettupDataOldValue());
}

void displaySettupControl() {
    if (isSettupChanged()) {
        String info = lcdSettupInfoEN[getSettupMode()];
        String settupOldValue = getDisplayedSettupData(getTemporarySettupDataOldValue(), getSettupMode());
        String settupNewValue = getDisplayedSettupData(getTemporarySettupDataNewValue(), getSettupMode());
    
        lcd.clear();
        
        // first line is info: what is changing and old value
        lcd.setCursor(0, 0);
        lcd.print(info + settupOldValue);
        
        // second line is like: < value >
        lcd.setCursor(0, 1); lcd.print((String)((char)247)); // sau 200
        lcd.setCursor(6, 1); lcd.print(settupNewValue);
        lcd.setCursor(15,1); lcd.print((String)((char)246)); // sau 199
        
        setSettupChanged(false);
    }
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
    switch (settupMode) {
        case SETTUP_MODE_DATE_DAY      : writeDateDay(value);break;
        case SETTUP_MODE_DATE_MONTH    : writeDateMonth(value);break;
        case SETTUP_MODE_DATE_YEAR     : writeDateYear(value);break;
        case SETTUP_MODE_TIME_HOUR     : writeTimeHour(value);break;
        case SETTUP_MODE_TIME_MINUTE   : writeTimeMinute(value);break;
        case SETTUP_MODE_TIME_MODE     : timeMode = value;break;
        case SETTUP_MODE_AQL_CONTROL   : aquariumLightsStatus = value;break;
        case SETTUP_MODE_AQL_MODE      : aquariumLightsMode = value;break;
        case SETTUP_MODE_AQL_ON1_HOUR  : aquariumLightsOn1TimeHour = value;break;
        case SETTUP_MODE_AQL_ON1_MIN   : aquariumLightsOn1TimeMinute = value;break;
        case SETTUP_MODE_AQL_OFF1_HOUR : aquariumLightsOff1TimeHour = value;break;
        case SETTUP_MODE_AQL_OFF1_MIN  : aquariumLightsOff1TimeMinute = value;break;
        case SETTUP_MODE_AQL_ON2_HOUR  : aquariumLightsOn2TimeHour = value;break;
        case SETTUP_MODE_AQL_ON2_MIN   : aquariumLightsOn2TimeMinute = value;break;
        case SETTUP_MODE_AQL_OFF2_HOUR : aquariumLightsOff2TimeHour = value;break;
        case SETTUP_MODE_AQL_OFF2_MIN  : aquariumLightsOff2TimeMinute = value;break;
        case SETTUP_MODE_AQV_CONTROL   : aquariumVentStatus = value;break;
        case SETTUP_MODE_AQV_MODE      : aquariumVentMode = value;break;
        case SETTUP_MODE_SPEAKER_MODE  : speakerMode = value;break;
        case SETTUP_MODE_ALARMS_MODE   : alarmsMode = value;break;
        case SETTUP_MODE_LCD_CONTROL   : lcdStatus = value;break;
        case SETTUP_MODE_LCD_MODE      : lcdMode = value;break;
        case SETTUP_MODE_LCD_TIMEOUT   : lcdTimeout = value;break;
        case SETTUP_MODE_WT_LIM_CLOW   : waterTempCriticalLowLimit = value;break;
        case SETTUP_MODE_WT_ACT_CLOW   : break;
        case SETTUP_MODE_WT_LIM_LOW    : waterTempLowLimit = value;break;
        case SETTUP_MODE_WT_ACT_LOW    : break;
        case SETTUP_MODE_WT_LIM_HIGH   : waterTempHighLimit = value;break;
        case SETTUP_MODE_WT_ACT_HIGH   : break;
        case SETTUP_MODE_WT_LIM_CHIGH  : waterTempCriticalHighLimit = value;break;
        case SETTUP_MODE_WT_ACT_CHIGH  : break;
        case SETTUP_MODE_WL_LIM_CLOW   : waterLevelCriticalLowLimit = value;break;
        case SETTUP_MODE_WL_ACT_CLOW   : break;
        case SETTUP_MODE_WL_LIM_LOW    : waterLevelLowLimit = value;break;
        case SETTUP_MODE_WL_ACT_LOW    : break;
        case SETTUP_MODE_WL_LIM_HIGH   : waterLevelHighLimit = value;break;
        case SETTUP_MODE_WL_ACT_HIGH   : break;
        case SETTUP_MODE_WL_LIM_CHIGH  : waterLevelCriticalHighLimit = value;break;
        case SETTUP_MODE_WL_ACT_CHIGH  : break;
    }
    applyControl();
}

// returns on 4 chars the displayed text for a value in specific mode
String getDisplayedSettupData(int value, int settupMode) {
    String displayedValue = "    ";
    switch (settupMode) {
        // int that are displayed as numbers (as they are)
        case SETTUP_MODE_DATE_DAY      : 
        case SETTUP_MODE_DATE_YEAR     : 
        case SETTUP_MODE_TIME_HOUR     : 
        case SETTUP_MODE_TIME_MINUTE   : 
        case SETTUP_MODE_AQL_ON1_HOUR  : 
        case SETTUP_MODE_AQL_ON1_MIN   : 
        case SETTUP_MODE_AQL_OFF1_HOUR : 
        case SETTUP_MODE_AQL_OFF1_MIN  : 
        case SETTUP_MODE_AQL_ON2_HOUR  : 
        case SETTUP_MODE_AQL_ON2_MIN   : 
        case SETTUP_MODE_AQL_OFF2_HOUR : 
        case SETTUP_MODE_AQL_OFF2_MIN  : 
        case SETTUP_MODE_LCD_TIMEOUT   : 
            displayedValue = String(value);
            break;
    
        case SETTUP_MODE_WT_LIM_CLOW   : 
        case SETTUP_MODE_WT_LIM_LOW    : 
        case SETTUP_MODE_WT_LIM_HIGH   : 
        case SETTUP_MODE_WT_LIM_CHIGH  : 
            displayedValue = String(value) + (String)((char)178) + "C";
            break;
        case SETTUP_MODE_WL_LIM_CLOW   : 
        case SETTUP_MODE_WL_LIM_LOW    : 
        case SETTUP_MODE_WL_LIM_HIGH   : 
        case SETTUP_MODE_WL_LIM_CHIGH  : 
            displayedValue = String(value) + "%";
            // used to search lcd special characters : 
            //displayedValue = String(value) + " = "  + (String)((char)value);
            break;
        
        // month - for now displayed as number
        case SETTUP_MODE_DATE_MONTH    :
            displayedValue = String(value);
            break;    
            
        // control = ON / OFF
        case SETTUP_MODE_AQL_CONTROL   : 
        case SETTUP_MODE_AQV_CONTROL   : 
        case SETTUP_MODE_LCD_CONTROL   : 
            if (value == AQUARIUM_LIGHT_STATUS_ON || value == AQUARIUM_VENT_STATUS_ON || value == LCD_STATUS_ON) {
                displayedValue = "On  ";
            } else if (value == AQUARIUM_LIGHT_STATUS_OFF || value == AQUARIUM_VENT_STATUS_OFF || value == LCD_STATUS_OFF) {
                displayedValue = "Off ";
            } else {
                displayedValue = "ERR ";
            }
            break;
        
        // modes = MANUAL / AUTO
        case SETTUP_MODE_AQL_MODE      : 
        case SETTUP_MODE_AQV_MODE      : 
        case SETTUP_MODE_LCD_MODE      : 
            if (value == AQUARIUM_LIGHT_MODE_AUTO || value == AQUARIUM_VENT_MODE_AUTO || value == LCD_MODE_AUTO) {
                displayedValue = "Auto";
            } else if (value == AQUARIUM_LIGHT_MODE_MANUAL || value == AQUARIUM_VENT_MODE_MANUAL || value == LCD_MODE_MANUAL) {
                displayedValue = "Man.";
            } else {
                displayedValue = "ERR ";
            }
            break;
    
        // modes = 12h / 24h
        case SETTUP_MODE_TIME_MODE     : 
            if (value == TIME_MODE_24H) {
                displayedValue = "24H ";
            } else if (value == TIME_MODE_AM_PM) {
                displayedValue = "AmPm";
            } else {
                displayedValue = "ERR ";
            }
            break;
        
        // modes = on / alarms / critical / off
        case SETTUP_MODE_SPEAKER_MODE  : 
            if (value == SPEAKER_MODE_OFF) {
                displayedValue = "Off ";
            } else if (value == SPEAKER_MODE_ON) {
                displayedValue = "On  ";
            } else if (value == SPEAKER_MODE_ALARMS) {
                displayedValue = "Alrm";
            } else if (value == SPEAKER_MODE_CRITICAL) {
                displayedValue = "Crit";
            }else {
                displayedValue = "ERR ";
            }
            break;
            
        // modes = on / critical / off
        case SETTUP_MODE_ALARMS_MODE   : 
            if (value == ALARMS_MODE_OFF) {
                displayedValue = "Off ";
            } else if (value == ALARMS_MODE_ON) {
                displayedValue = "On  ";
            } else if (value == ALARMS_MODE_CRITICAL) {
                displayedValue = "Crit";
            }else {
                displayedValue = "ERR ";
            }
            break;
    
        // actions
        case SETTUP_MODE_WT_ACT_CLOW   : 
        case SETTUP_MODE_WT_ACT_LOW    : 
        case SETTUP_MODE_WT_ACT_HIGH   : 
        case SETTUP_MODE_WT_ACT_CHIGH  : 
        case SETTUP_MODE_WL_ACT_CLOW   : 
        case SETTUP_MODE_WL_ACT_LOW    : 
        case SETTUP_MODE_WL_ACT_HIGH   : 
        case SETTUP_MODE_WL_ACT_CHIGH  : 
            displayedValue = "n/a ";
            break;
    }
    return displayedValue;
}


int getSettupValueLimitLow() {
    int value = 0;
    switch (settupMode) {
        case SETTUP_MODE_DATE_DAY      : value = 1;break;
        case SETTUP_MODE_DATE_MONTH    : value = 1;break;
        case SETTUP_MODE_DATE_YEAR     : value = 2014;break;
        
        case SETTUP_MODE_TIME_HOUR     : 
        case SETTUP_MODE_AQL_ON1_HOUR  : 
        case SETTUP_MODE_AQL_OFF1_HOUR : 
        case SETTUP_MODE_AQL_ON2_HOUR  : 
        case SETTUP_MODE_AQL_OFF2_HOUR : 
            value = 0;
            break;
        
        case SETTUP_MODE_TIME_MINUTE   : 
        case SETTUP_MODE_AQL_ON1_MIN   : 
        case SETTUP_MODE_AQL_OFF1_MIN  : 
        case SETTUP_MODE_AQL_ON2_MIN   : 
        case SETTUP_MODE_AQL_OFF2_MIN  : 
            value = 0;
            break;
    
        case SETTUP_MODE_TIME_MODE     : value = TIME_MODE_24H;break;
        case SETTUP_MODE_AQL_CONTROL   : value = AQUARIUM_LIGHT_STATUS_OFF;break;
        case SETTUP_MODE_AQL_MODE      : value = AQUARIUM_LIGHT_MODE_AUTO;break;
        case SETTUP_MODE_AQV_CONTROL   : value = AQUARIUM_VENT_STATUS_OFF;break;
        case SETTUP_MODE_AQV_MODE      : value = AQUARIUM_VENT_MODE_AUTO;break;
        case SETTUP_MODE_SPEAKER_MODE  : value = SPEAKER_MODE_OFF;break;
        case SETTUP_MODE_ALARMS_MODE   : value = ALARMS_MODE_ON;break;
        case SETTUP_MODE_LCD_CONTROL   : value = LCD_STATUS_OFF;break;
        case SETTUP_MODE_LCD_MODE      : value = LCD_MODE_AUTO;break;
    
        case SETTUP_MODE_LCD_TIMEOUT   : 
            value = 10;
            break;
    
        case SETTUP_MODE_WT_LIM_CLOW   : 
        case SETTUP_MODE_WT_LIM_LOW    : 
        case SETTUP_MODE_WT_LIM_HIGH   : 
        case SETTUP_MODE_WT_LIM_CHIGH  : 
            value = 15;
            break;
    
        case SETTUP_MODE_WL_LIM_CLOW   : 
        case SETTUP_MODE_WL_LIM_LOW    : 
        case SETTUP_MODE_WL_LIM_HIGH   : 
        case SETTUP_MODE_WL_LIM_CHIGH  : 
            value = 10;
            break;
    
        case SETTUP_MODE_WT_ACT_CLOW   : 
        case SETTUP_MODE_WT_ACT_LOW    : 
        case SETTUP_MODE_WT_ACT_HIGH   : 
        case SETTUP_MODE_WT_ACT_CHIGH  : 
        case SETTUP_MODE_WL_ACT_CLOW   : 
        case SETTUP_MODE_WL_ACT_LOW    : 
        case SETTUP_MODE_WL_ACT_HIGH   : 
        case SETTUP_MODE_WL_ACT_CHIGH  : 
            value = ALARM_NO_ACTION;
            break;
    }
    return value;
}

int getSettupValueLimitHigh() {
    int value = 0;
    switch (settupMode) {
        case SETTUP_MODE_DATE_DAY      : value = 31;break;
        case SETTUP_MODE_DATE_MONTH    : value = 12;break;
        case SETTUP_MODE_DATE_YEAR     : value = 2024;break;
        
        case SETTUP_MODE_TIME_HOUR     : 
        case SETTUP_MODE_AQL_ON1_HOUR  : 
        case SETTUP_MODE_AQL_OFF1_HOUR : 
        case SETTUP_MODE_AQL_ON2_HOUR  : 
        case SETTUP_MODE_AQL_OFF2_HOUR : 
            value = 23;
            break;
        
        case SETTUP_MODE_TIME_MINUTE   : 
        case SETTUP_MODE_AQL_ON1_MIN   : 
        case SETTUP_MODE_AQL_OFF1_MIN  : 
        case SETTUP_MODE_AQL_ON2_MIN   : 
        case SETTUP_MODE_AQL_OFF2_MIN  : 
            value = 59;
            break;
    
        case SETTUP_MODE_TIME_MODE     : value = TIME_MODE_AM_PM;break;
        case SETTUP_MODE_AQL_CONTROL   : value = AQUARIUM_LIGHT_STATUS_ON;break;
        case SETTUP_MODE_AQL_MODE      : value = AQUARIUM_LIGHT_MODE_MANUAL;break;
        case SETTUP_MODE_AQV_CONTROL   : value = AQUARIUM_VENT_STATUS_ON;break;
        case SETTUP_MODE_AQV_MODE      : value = AQUARIUM_VENT_MODE_MANUAL;break;
        case SETTUP_MODE_SPEAKER_MODE  : value = SPEAKER_MODE_CRITICAL;break;
        case SETTUP_MODE_ALARMS_MODE   : value = ALARMS_MODE_CRITICAL;break;
        case SETTUP_MODE_LCD_CONTROL   : value = LCD_STATUS_ON;break;
        case SETTUP_MODE_LCD_MODE      : value = LCD_MODE_MANUAL;break;
    
        case SETTUP_MODE_LCD_TIMEOUT   : 
            value = 120;
            break;
    
        case SETTUP_MODE_WT_LIM_CLOW   : 
        case SETTUP_MODE_WT_LIM_LOW    : 
        case SETTUP_MODE_WT_LIM_HIGH   : 
        case SETTUP_MODE_WT_LIM_CHIGH  : 
            value = 35;
            break;
    
        case SETTUP_MODE_WL_LIM_CLOW   : 
        case SETTUP_MODE_WL_LIM_LOW    : 
        case SETTUP_MODE_WL_LIM_HIGH   : 
        case SETTUP_MODE_WL_LIM_CHIGH  : 
            value = 120;
            break;
    
        case SETTUP_MODE_WT_ACT_CLOW   : 
        case SETTUP_MODE_WT_ACT_LOW    : 
        case SETTUP_MODE_WT_ACT_HIGH   : 
        case SETTUP_MODE_WT_ACT_CHIGH  : 
        case SETTUP_MODE_WL_ACT_CLOW   : 
        case SETTUP_MODE_WL_ACT_LOW    : 
        case SETTUP_MODE_WL_ACT_HIGH   : 
        case SETTUP_MODE_WL_ACT_CHIGH  : 
            value = ALARM_AQL_BLINK_ON;
            break;
    }
    return value;
}

int getSettupValueStep() {
    int value = 0;
    switch (settupMode) {
        case SETTUP_MODE_DATE_DAY      : 
        case SETTUP_MODE_DATE_MONTH    : 
        case SETTUP_MODE_DATE_YEAR     : 
    
        case SETTUP_MODE_TIME_HOUR     : 
        case SETTUP_MODE_AQL_ON1_HOUR  : 
        case SETTUP_MODE_AQL_OFF1_HOUR : 
        case SETTUP_MODE_AQL_ON2_HOUR  : 
        case SETTUP_MODE_AQL_OFF2_HOUR : 
        
        case SETTUP_MODE_TIME_MINUTE   : 
        case SETTUP_MODE_AQL_ON1_MIN   : 
        case SETTUP_MODE_AQL_OFF1_MIN  : 
        case SETTUP_MODE_AQL_ON2_MIN   : 
        case SETTUP_MODE_AQL_OFF2_MIN  : 
    
        case SETTUP_MODE_TIME_MODE     :
        case SETTUP_MODE_AQL_CONTROL   : 
        case SETTUP_MODE_AQL_MODE      : 
        case SETTUP_MODE_AQV_CONTROL   : 
        case SETTUP_MODE_AQV_MODE      : 
        case SETTUP_MODE_SPEAKER_MODE  : 
        case SETTUP_MODE_ALARMS_MODE   : 
        case SETTUP_MODE_LCD_CONTROL   : 
        case SETTUP_MODE_LCD_MODE      :
            value = 1;
            break; 
    
        case SETTUP_MODE_LCD_TIMEOUT   : 
            value = 10;
            break;
    
        case SETTUP_MODE_WT_LIM_CLOW   : 
        case SETTUP_MODE_WT_LIM_LOW    : 
        case SETTUP_MODE_WT_LIM_HIGH   : 
        case SETTUP_MODE_WT_LIM_CHIGH  : 
            value = 1;
            break;
    
        case SETTUP_MODE_WL_LIM_CLOW   : 
        case SETTUP_MODE_WL_LIM_LOW    : 
        case SETTUP_MODE_WL_LIM_HIGH   : 
        case SETTUP_MODE_WL_LIM_CHIGH  : 
            value = 5;
            break;
    
        case SETTUP_MODE_WT_ACT_CLOW   : 
        case SETTUP_MODE_WT_ACT_LOW    : 
        case SETTUP_MODE_WT_ACT_HIGH   : 
        case SETTUP_MODE_WT_ACT_CHIGH  : 
        case SETTUP_MODE_WL_ACT_CLOW   : 
        case SETTUP_MODE_WL_ACT_LOW    : 
        case SETTUP_MODE_WL_ACT_HIGH   : 
        case SETTUP_MODE_WL_ACT_CHIGH  : 
            value = 1;
            break;
    }
    return value;
}

// === Getters and Setters ===
void setTemporarySettupDataOldValue(int value) {
    temporarySettupDataOldValue = value;
}

int getTemporarySettupDataOldValue() {
    return temporarySettupDataOldValue;
}

void setTemporarySettupDataNewValue(int value) {
    temporarySettupDataNewValue = value;
}

int getTemporarySettupDataNewValue() {
    return temporarySettupDataNewValue;
}

void setSettupMode(int mode) {
    settupMode = mode;
}

int getSettupMode() {
    return settupMode;
}

void setSettupChanged(boolean changed) {
    settupChanged = changed;
}

boolean isSettupChanged() {
    return settupChanged;
}

// === Util / Helper / Internal / Private ===


