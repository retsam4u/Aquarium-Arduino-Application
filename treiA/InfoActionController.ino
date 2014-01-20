#define KEY_INFO_PAUSE           KEY_PLAY_PAUSE
#define KEY_INFO_PREV            KEY_PREV
#define KEY_INFO_NEXT            KEY_NEXT
#define KEY_INFO_LIGHTS_ON_OFF   KEY_0
#define KEY_INFO_VENT_ON_OFF     KEY_1

void checkInfoIRCommand(unsigned long command) {
    if (command == KEY_INFO_PAUSE) {
        keyBeep();
        pausePlayDisplay();
    }
    
    if (command == KEY_INFO_PREV) {
        keyBeep();
        prevInfoPage();
    }
    
    if (command == KEY_INFO_NEXT) {
        keyBeep();
        nextInfoPage();
    }

    if (command == KEY_INFO_LIGHTS_ON_OFF) {
        keyBeep();
        if (aquariumLightsStatus == AQUARIUM_LIGHT_STATUS_ON) {
            turnAquariumLightsOff();
            
        } else if (aquariumLightsStatus == AQUARIUM_LIGHT_STATUS_OFF) {
            turnAquariumLightsOn();
        }
        gotoInfoPage(LCD_INFO_AQ_LIGHT);
    }
    
    if (command == KEY_INFO_VENT_ON_OFF) {
        keyBeep();
        if (aquariumVentStatus == AQUARIUM_VENT_STATUS_ON) {
            turnAquariumVentOff();
            
        } else if (aquariumVentStatus == AQUARIUM_VENT_STATUS_OFF) {
            turnAquariumVentOn();
        }
        gotoInfoPage(LCD_INFO_AQ_VENTILATION);
    }

}

