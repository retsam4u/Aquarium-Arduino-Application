#define KEY_INFO_PAUSE           KEY_PLAY_PAUSE
#define KEY_INFO_PREV            KEY_PREV
#define KEY_INFO_NEXT            KEY_PREV
#define KEY_INFO_LIGHTS_ON_OFF   KEY_0
#define KEY_INFO_VENT_ON_OFF     KEY_1

void checkInfoIRCommand(unsigned long command) {
    if (command == KEY_INFO_PAUSE) { // play/pause
        pausePlayDisplay();
    }
    
    if (command == KEY_INFO_PREV) { // prev
        prevInfoPage();
    }
    
    if (command == KEY_INFO_NEXT) { // next
        nextInfoPage();
    }

    if (command == KEY_INFO_LIGHTS_ON_OFF) { // key 0
      if (aquariumLightsStatus == AQUARIUM_LIGHT_STATUS_ON) {
        aquariumLightsStatus = AQUARIUM_LIGHT_STATUS_OFF;
        digitalWrite(PIN_RELAY_CH_1, LOW);
      } else if (aquariumLightsStatus == AQUARIUM_LIGHT_STATUS_OFF) {
        aquariumLightsStatus = AQUARIUM_LIGHT_STATUS_ON;
        digitalWrite(PIN_RELAY_CH_1, HIGH);
      }
      gotoInfoPage(LCD_INFO_AQ_LIGHT);
    }
    
    if (command == KEY_INFO_VENT_ON_OFF) { // key 1
      if (aquariumVentStatus == AQUARIUM_VENT_STATUS_ON) {
        aquariumVentStatus = AQUARIUM_VENT_STATUS_OFF;
        digitalWrite(PIN_RELAY_CH_2, LOW);
      } else if (aquariumVentStatus == AQUARIUM_VENT_STATUS_OFF) {
        aquariumVentStatus = AQUARIUM_VENT_STATUS_ON;
        digitalWrite(PIN_RELAY_CH_2, HIGH);
      }
      gotoInfoPage(LCD_INFO_AQ_VENTILATION);
    }

}

