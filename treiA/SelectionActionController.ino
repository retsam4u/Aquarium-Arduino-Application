#define KEY_MENU_CANCEL     KEY_MUTE
#define KEY_MENU_SAVE       KEY_PLAY_PAUSE
#define KEY_MENU_PREVIOUS   KEY_PREV
#define KEY_MENU_NEXT       KEY_NEXT

void checkSettupIRCommand(unsigned long command) {
    if (command == KEY_MUTE) { // mute = cancel
        keyBeep();
        lcd.setCursor(1, 1); lcd.print("   CANCELED   ");
        delay(1000);

        returnToSettupMenu();
    }
    
    if (command == KEY_PLAY_PAUSE) { // play/pause = save
        keyBeep();
        lcd.setCursor(1, 1); lcd.print(" SAVED: " + getDisplayedSettupData(getTemporarySettupDataNewValue(), getSettupMode()));
        delay(1000);
        
        setCurrentSettupDataValue(getTemporarySettupDataNewValue());

        returnToSettupMenu();
    }
    
    if (command == KEY_PREV || command == KEY_NEXT) { // prev = -value ; next = +value
        keyBeep();
        int newValue = getTemporarySettupDataNewValue();
        
        // increment or decrement value
        if (command == KEY_PREV) newValue -= getSettupValueStep();
        if (command == KEY_NEXT) newValue += getSettupValueStep();

        // check limits
        if (newValue < getSettupValueLimitLow()) {
            newValue = getSettupValueLimitHigh();
        }
        if (newValue > getSettupValueLimitHigh()) {
            newValue = getSettupValueLimitLow();
        }
        
        // set new value
        setTemporarySettupDataNewValue(newValue);
        setSettupChanged(true);
    }
    
}

// === Helper method ===
void returnToSettupMenu() {
    userInterfaceMode = UI_MODE_SETTINGS_MENU;
    setMenuChanged(true);
}
