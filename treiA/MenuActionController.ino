#define KEY_MENU_BACK       KEY_MUTE
#define KEY_MENU_SELECT     KEY_PLAY_PAUSE
#define KEY_MENU_PREVIOUS   KEY_PREV
#define KEY_MENU_NEXT       KEY_NEXT

void checkSettingsMenuIRCommand(unsigned long command) {
    if (command == KEY_MENU_BACK) {
        menuGoBack();
    }
    
    if (command == KEY_MENU_SELECT) {
        menuSelect();
    }
    
    if (command == KEY_MENU_PREVIOUS) {
        menuPrevious();
    }
    
    if (command == KEY_MENU_NEXT) {
        menuNext();
    }
}

