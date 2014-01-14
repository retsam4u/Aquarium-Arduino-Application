// === LCD Settings Menu ===
// Menu

boolean menuChanged = false;

MenuSystem menuSystem;

Menu mRoot("3A Settings");
Menu m1("Date and Time");
Menu m1_m1("Set date");
MenuItem m1_m1_i1("day:");
MenuItem m1_m1_i2("month:");
MenuItem m1_m1_i3("year:");
Menu m1_m2("Set time");
MenuItem m1_m2_i1("hour:");
MenuItem m1_m2_i2("minute:");
MenuItem m1_m2_i3("mode:");
Menu m2("Aquarium lights");
MenuItem m2_i1("control:");
MenuItem m2_i2("mode:");
Menu m2_m3("Auto");
MenuItem m2_m3_i1("on1 hour:");
MenuItem m2_m3_i2("on1 minute:");
MenuItem m2_m3_i3("off1 hour:");
MenuItem m2_m3_i4("off1 minute:");
MenuItem m2_m3_i5("on2 hour:");
MenuItem m2_m3_i6("on2 minute:");
MenuItem m2_m3_i7("off2 hour:");
MenuItem m2_m3_i8("off2 minute:");
Menu m3("Aquarium vent");
MenuItem m3_i1("control:");
MenuItem m3_i2("mode:");
MenuItem i3("Speaker:");
MenuItem i4("Alarms:");
Menu m5("LCD");
MenuItem m5_i1("control:");
MenuItem m5_i2("mode:");
MenuItem m5_i3("timeout:");
Menu m6("Water temp");
MenuItem m6_i1("Lim. CLow:");
MenuItem m6_i2("Act. CLow:");
MenuItem m6_i3("Lim. Low:");
MenuItem m6_i4("Act. Low:");
MenuItem m6_i5("Lim. High:");
MenuItem m6_i6("Act. High:");
MenuItem m6_i7("Lim. CHigh:");
MenuItem m6_i8("Act. CHigh:");
Menu m7("Water level");
MenuItem m7_i1("Lim. CLow:");
MenuItem m7_i2("Act. CLow:");
MenuItem m7_i3("Lim. Low:");
MenuItem m7_i4("Act. Low:");
MenuItem m7_i5("Lim. High:");
MenuItem m7_i6("Act. High:");
MenuItem m7_i7("Lim. CHigh:");
MenuItem m7_i8("Act. CHigh:");

void initSettupMenu() {
    mRoot.add_menu(&m1);
    m1.add_menu(&m1_m1);
    m1_m1.add_item(&m1_m1_i1, &onSelect, SETTUP_MODE_DATE_DAY);
    m1_m1.add_item(&m1_m1_i2, &onSelect, SETTUP_MODE_DATE_MONTH);
    m1_m1.add_item(&m1_m1_i3, &onSelect, SETTUP_MODE_DATE_YEAR);
    m1.add_menu(&m1_m2);
    m1_m2.add_item(&m1_m2_i1, &onSelect, SETTUP_MODE_TIME_HOUR);
    m1_m2.add_item(&m1_m2_i2, &onSelect, SETTUP_MODE_TIME_MINUTE);
    m1_m2.add_item(&m1_m2_i3, &onSelect, SETTUP_MODE_TIME_MODE);
    mRoot.add_menu(&m2);
    m2.add_item(&m2_i1, &onSelect, SETTUP_MODE_AQL_CONTROL);
    m2.add_item(&m2_i2, &onSelect, SETTUP_MODE_AQL_MODE);
    m2.add_menu(&m2_m3);
    m2_m3.add_item(&m2_m3_i1, &onSelect, SETTUP_MODE_AQL_ON1_HOUR);
    m2_m3.add_item(&m2_m3_i2, &onSelect, SETTUP_MODE_AQL_ON1_MIN);
    m2_m3.add_item(&m2_m3_i3, &onSelect, SETTUP_MODE_AQL_OFF1_HOUR);
    m2_m3.add_item(&m2_m3_i4, &onSelect, SETTUP_MODE_AQL_OFF1_MIN);
    m2_m3.add_item(&m2_m3_i5, &onSelect, SETTUP_MODE_AQL_ON2_HOUR);
    m2_m3.add_item(&m2_m3_i6, &onSelect, SETTUP_MODE_AQL_ON2_MIN);
    m2_m3.add_item(&m2_m3_i7, &onSelect, SETTUP_MODE_AQL_OFF2_HOUR);
    m2_m3.add_item(&m2_m3_i8, &onSelect, SETTUP_MODE_AQL_OFF2_MIN);
    mRoot.add_menu(&m3);
    m2.add_item(&m3_i1, &onSelect, SETTUP_MODE_AQV_CONTROL);
    m2.add_item(&m3_i2, &onSelect, SETTUP_MODE_AQV_MODE);
    mRoot.add_item(&i3, &onSelect, SETTUP_MODE_SPEAKER_MODE);
    mRoot.add_item(&i4, &onSelect, SETTUP_MODE_ALARMS_MODE);
    mRoot.add_menu(&m5);
    m5.add_item(&m5_i1, &onSelect, SETTUP_MODE_LCD_CONTROL);
    m5.add_item(&m5_i2, &onSelect, SETTUP_MODE_LCD_MODE);
    m5.add_item(&m5_i3, &onSelect, SETTUP_MODE_LCD_TIMEOUT);
    mRoot.add_menu(&m6);
    m6.add_item(&m6_i1, &onSelect, SETTUP_MODE_WT_LIM_CLOW);
    m6.add_item(&m6_i2, &onSelect, SETTUP_MODE_WT_ACT_CLOW);
    m6.add_item(&m6_i3, &onSelect, SETTUP_MODE_WT_LIM_LOW);
    m6.add_item(&m6_i4, &onSelect, SETTUP_MODE_WT_ACT_LOW);
    m6.add_item(&m6_i5, &onSelect, SETTUP_MODE_WT_LIM_HIGH);
    m6.add_item(&m6_i6, &onSelect, SETTUP_MODE_WT_ACT_HIGH);
    m6.add_item(&m6_i7, &onSelect, SETTUP_MODE_WT_LIM_CHIGH);
    m6.add_item(&m6_i8, &onSelect, SETTUP_MODE_WT_ACT_CHIGH);
    mRoot.add_menu(&m7);
    m7.add_item(&m7_i1, &onSelect, SETTUP_MODE_WL_LIM_CLOW);
    m7.add_item(&m7_i2, &onSelect, SETTUP_MODE_WL_ACT_CLOW);
    m7.add_item(&m7_i3, &onSelect, SETTUP_MODE_WL_LIM_LOW);
    m7.add_item(&m7_i4, &onSelect, SETTUP_MODE_WL_ACT_LOW);
    m7.add_item(&m7_i5, &onSelect, SETTUP_MODE_WL_LIM_HIGH);
    m7.add_item(&m7_i6, &onSelect, SETTUP_MODE_WL_ACT_HIGH);
    m7.add_item(&m7_i7, &onSelect, SETTUP_MODE_WL_LIM_CHIGH);
    m7.add_item(&m7_i8, &onSelect, SETTUP_MODE_WL_ACT_CHIGH);
  
    menuSystem.set_root_menu(&mRoot);
}

void setMenuChanged(boolean changed) {
    menuChanged = changed;
}

boolean isMenuChanged() {
    return menuChanged;
}

void menuGoBack() {
    menuSystem.back();
    menuChanged = true;
}

void menuSelect() {
    menuSystem.select();
    menuChanged = true;
    settupChanged = true;
}

void menuPrevious() {
    menuSystem.prev();
    menuChanged = true;
}

void menuNext() {
    menuSystem.next();
    menuChanged = true;
}

char* getCurrentMenuDescription() {
    Menu const* cp_menu = menuSystem.get_current_menu();
    return cp_menu->get_name();
}

char* getSelectionMenuDescription() {
    Menu const* cp_menu = menuSystem.get_current_menu();
    return cp_menu->get_selected()->get_name();
}
