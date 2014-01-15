int lcdDelayTime                   = 5000;
int lcdDelayTimeInPauseMode        = 1000;

// === LCD Information ===
int displayedPage = 0;
boolean pauseDisplay = false;
long lastDisplayTime = 0;

// === LCD info screen ===
const int LCD_INFO_MAX_SCREEN     = 14;

// === text constants for info and menu ===
const String lcdInfoDescrEN[] = {"Date and time", "Temperature", "Level", "Ext temperature", "Ext humidity", 
                               "Ext light", "Light", "Ventilation", "LCD", 
                               "Alarms", "Speaker", "Last feed time", "Next feed in", "About"};

void initInfoDisplay() {
    displayedPage = LCD_INFO_MAX_SCREEN;
}

void displayOnLCD() {
    long currentTime = millis();
    long timePassed = currentTime - lastDisplayTime;
    
    if (timePassed >= lcdDelayTime || lastDisplayTime == 0 || (pauseDisplay && timePassed >= lcdDelayTimeInPauseMode)) {
        lastDisplayTime = currentTime;
        
        if (!pauseDisplay) {
            displayedPage = displayedPage + 1;
        }
        if (displayedPage >= LCD_INFO_MAX_SCREEN) {
            displayedPage = LCD_INFO_DATE_TIME;
        }
        
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
                lcd_1 = " 3A - ver.2.0"; // + APP_VERSION_NUMBER;
                lcd_2 = "(c)2014 ValiP";
                break;
        }
    
        lcd.clear();
        lcd.setCursor(0, 0); lcd.print(lcd_1);
        lcd.setCursor(0, 1); lcd.print(lcd_2);
    }
}

void pausePlayDisplay() {
    pauseDisplay = !pauseDisplay;
}

void gotoInfoPage(int infoPage) {
    lastDisplayTime = 0;
    displayedPage = infoPage;
    if (!pauseDisplay) displayedPage = infoPage - 1;
}

void prevInfoPage() {
    lastDisplayTime = 0;
    if (!pauseDisplay) {
        displayedPage = displayedPage - 2;
    } else {
        displayedPage = displayedPage - 1;
    }
    if (displayedPage == -2) displayedPage = LCD_INFO_REMAIN_TO_FEED;
    if (displayedPage == -1) displayedPage = LCD_INFO_DESPRE;
}

void nextInfoPage() {
    lastDisplayTime = 0;
    if (!pauseDisplay) {
        displayedPage = displayedPage + 0;
    } else {
        displayedPage = displayedPage + 1;
    }
    if (displayedPage >= LCD_INFO_MAX_SCREEN) displayedPage = LCD_INFO_DATE_TIME;
}
