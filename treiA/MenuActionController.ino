#define KEY_MENU_BACK       KEY_MUTE
#define KEY_MENU_SELECT     KEY_PLAY_PAUSE
#define KEY_MENU_PREVIOUS   KEY_PREV
#define KEY_MENU_NEXT       KEY_NEXT

void checkSettingsMenuIRCommand(unsigned long command) {
    if (command == KEY_MENU_BACK) {
        keyBeep();
        menuGoBack();
    }
    
    if (command == KEY_MENU_SELECT) {
        keyBeep();
        menuSelect();
    }
    
    if (command == KEY_MENU_PREVIOUS) {
        keyBeep();
        menuPrevious();
    }
    
    if (command == KEY_MENU_NEXT) {
        keyBeep();
        menuNext();
    }
}

void onSelect(MenuItem* menuItem) {
    setSettupMode(menuItem->get_action_code());
    userInterfaceMode = UI_MODE_SETTUP;
    initDisplaySettupControl();
}


