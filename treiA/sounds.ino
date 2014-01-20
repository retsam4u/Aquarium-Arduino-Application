#include "pitches.h"

void initSounds() {
    pinMode(PIN_SOUND, OUTPUT);
}

void startupSoundCheckup() {
    toneManual(NOTE_C4, 500);
    delay(500);
}

void keyBeep() {
    if (lcdMode == LCD_MODE_AUTO && lcdTimeoutStartTime > 0) {
        lcdTimeoutStartTime = millis();
    }

    if (speakerMode == SPEAKER_MODE_ON) {
        toneManual(NOTE_C5, 100);
        delay(10);
    }
}

void toneManual(int frequency, int duration)
{
  unsigned long period = 1000000/frequency;
  unsigned long length;
  boolean state = false;
  for (length = 0; length < (long) duration * 1000; length += period) {
    state = !state;
    digitalWrite(PIN_SOUND, state);
    /* The 50uS correspond to the time the rest of the loop body takes.
     * It seems about right, but has not been tuned precisely for
     * a 16MHz ATMega. */
    delayMicroseconds(period - 50);
  }
}

