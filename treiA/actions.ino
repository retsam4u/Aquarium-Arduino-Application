// ===================
// === LCD ACTIONS ===
// ===================
void turnLcdOn() {
    lcdStatus = LCD_STATUS_ON;
    lcd.backlight();
    if (lcdMode == LCD_MODE_AUTO) {
        lcdTimeoutStartTime = millis();
    }
}

void turnLcdOff() {
    lcdStatus = LCD_STATUS_OFF;
    lcd.noBacklight();
    if (lcdMode == LCD_MODE_AUTO) {
        lcdTimeoutStartTime = 0;
    }
}

void applyLcdStatus() {
    if (lcdStatus == LCD_STATUS_ON) {
        turnLcdOn();
    } else if (lcdStatus == LCD_STATUS_OFF) {
        turnLcdOff();
    }
}

void turnLcdBlinkOn() {
    lcdBlinkTime = 5000;
    lastLcdBlinkTime = millis();
}

void turnLcdBlinkOff() {
    lcdBlinkTime = 0;
}

void blinkLcd() {
    int initialLcdStatus = lcdStatus;
    turnLcdOff(); delay(50);
    turnLcdOn(); delay(100);
    turnLcdOff(); delay(100);
    turnLcdOn(); delay(100);
    turnLcdOff(); delay(50);
    lcdStatus = initialLcdStatus;
    applyLcdStatus();
}

// ===============================
// === AQUARIUM LIGHTS ACTIONS ===
// ===============================
void turnAquariumLightsOn() {
    aquariumLightsStatus = AQUARIUM_LIGHT_STATUS_ON;
    digitalWrite(PIN_RELAY_CH_1, HIGH);
}

void turnAquariumLightsOff() {
    aquariumLightsStatus = AQUARIUM_LIGHT_STATUS_OFF;
    digitalWrite(PIN_RELAY_CH_1, LOW);
}

void applyAquariumLightsStatus() {
    if (aquariumLightsStatus == AQUARIUM_LIGHT_STATUS_ON) {
        turnAquariumLightsOn();
    } else if (aquariumLightsStatus == AQUARIUM_LIGHT_STATUS_OFF) {
        turnAquariumLightsOff();
    }
}

void turnLightsBlinkOn() {
    aquariumLightsBlinkTime = 60*1000;
}

void turnLightsBlinkOff() {
    aquariumLightsBlinkTime = 0;
}

void blinkAquariumLightsWithOff() {
    turnAquariumLightsOff(); delay(5000);
    turnAquariumLightsOn(); delay(6000);
    turnAquariumLightsOff(); delay(5000);
    turnAquariumLightsOn(); delay(6000);
    turnAquariumLightsOff();
}

void blinkAquariumLightsWithOn() {
    turnAquariumLightsOff(); delay(5000);
    turnAquariumLightsOn(); delay(6000);
    turnAquariumLightsOff(); delay(5000);
    turnAquariumLightsOn(); delay(6000);
    turnAquariumLightsOff(); delay(5000);
    turnAquariumLightsOn();
}

// =============================
// === AQUARIUM VENT ACTIONS ===
// =============================
void turnAquariumVentOn() {
    aquariumVentStatus = AQUARIUM_VENT_STATUS_ON;
    digitalWrite(PIN_RELAY_CH_2, HIGH);
}

void turnAquariumVentOff() {
    aquariumVentStatus = AQUARIUM_VENT_STATUS_OFF;
    digitalWrite(PIN_RELAY_CH_2, LOW);
}

void applyAquariumVentStatus() {
    if (aquariumVentStatus == AQUARIUM_VENT_STATUS_ON) {
        turnAquariumVentOn();
    } else if (aquariumVentStatus == AQUARIUM_VENT_STATUS_OFF) {
        turnAquariumVentOff();
    }
}

// =======================
// === SPEAKER ACTIONS ===
// =======================
void turnSpeakerBeepOff() {
    speakerBeepTime = 0;
}

void turnSpeakerBeepOnContinuous() {
    speakerBeepTime = 1000;
}

void turnSpeakerBeepOn5Sec() {
    speakerBeepTime = 5*1000;
}

void turnSpeakerBeepOn1Min() {
    speakerBeepTime = 60*1000;
}

void turnSpeakerBeepOn5Min() {
    speakerBeepTime = 5*60*1000;
}

