#define KEY_MENU_CANCEL     KEY_MUTE
#define KEY_MENU_SAVE       KEY_PLAY_PAUSE
#define KEY_MENU_PREVIOUS   KEY_PREV
#define KEY_MENU_NEXT       KEY_NEXT

void checkSettupIRCommand(unsigned long command) {
    if (command == KEY_MUTE) { // mute = cancel
        lcd.setCursor(1, 1); lcd.print("   CANCELED   ");
        delay(1000);

        returnToSettupMenu();
    }
    
    if (command == KEY_PLAY_PAUSE) { // play/pause = save
        lcd.setCursor(1, 1); lcd.print(" SAVED: " + getDisplayedSettupData(getTemporarySettupDataNewValue(), getSettupMode()));
        delay(1000);
        
        setCurrentSettupDataValue(getTemporarySettupDataNewValue());

        returnToSettupMenu();
    }
    
    if (command == KEY_PREV) { // prev = -value
        setTemporarySettupDataNewValue(getTemporarySettupDataNewValue() - 1);
        setSettupChanged(true);
    }
    
    if (command == KEY_NEXT) { // next = +value
        setTemporarySettupDataNewValue(getTemporarySettupDataNewValue() + 1);
        setSettupChanged(true);
    }
}

// === Helper method ===
void returnToSettupMenu() {
    userInterfaceMode = UI_MODE_SETTINGS_MENU;
    setMenuChanged(true);
}
