#include "gol.h"
#include "text.h"
// ==================== ARDUINO BITMASK INFO ====================
// dataA = 2           PD2   pin 1 on CN0/CN1  active HIGH
// dataB = 3           PD3   pin 2 on CN0/CN1  active HIGH
// dataC = 4           PD4   pin 3 on CN0/CN1  active HIGH
// G2AU38_ENABLE = 5   PD5   pin 4 on CN0/CN1  active LOW top
// G2AU37_ENABLE = 6   PD6   pin 5 on CN0/CN1  active LOW botom
// G1_ENABLE = 7       PD7   pin 6 on CN0/CN1  active HIGH

// G2B_ENABLE = 8      PB0   pin 7 on CN0/CN1  active LOW
// serialClock = 9     PB1   pin 8 on CN0/CN1 B1 for 9  rising edge
// serialData = 10     PB2   pin 9 on CN0/CN1 B2 for 10  active HIGH


unsigned long frameTimer;  //timer for frame
const int pixleLength = 85;  //how many pixles in serial string
int displayBuffer[pixleLength] = {0}; //buffer to store the active frame


void setup() {
  Serial.begin(115200);
  // DDRB and DDRD 1 is output 0 is input
  DDRD = B11111110; PORTD = B11100010;
  DDRB = B00000111; PORTB = B00000000;
  clearDisplay();  //reset the display
  clearBuffer(); invertBuffer();  //make the display all on
}



void loop() {
  write_string_small_top(displayBuffer, "top");
  write_string_small_bottom(displayBuffer, "bottom");
  updateDisplay(500);
}



void updateDisplay(int holdTime) {  //holds the display on for increments of 10ms
  frameTimer = millis();
  while (millis() - frameTimer < holdTime) {
    for (int Y = 0; Y < 16; Y++) {
      PORTD = PORTD | B01100000;  //G2AU38, G2AU37 ENABLE HIGH
      for (int X = 0; X < pixleLength; X++) {
        if (displayBuffer[X] >> Y & 1) PORTB = PORTB | B00000100;
        else PORTB = PORTB & B11111011;
        PORTB = PORTB | B00000010;  //pin 8 on CN0/CN1 B1 for 9
        PORTB = PORTB & B11111101;  //pin 8 on CN0/CN1 B1 for 9
      }
      PORTD = PORTD & B11100011;
      PORTD = PORTD | (Y & 7) << 2;  //set the data bits
      if (Y < 8) PORTD = PORTD & B11011111;  //G2AU38 ENABLE LOW
      else PORTD = PORTD & B10111111;  //G2AU37 ENABLE LOW
      delayMicroseconds(507);  //507 for 100Hz rate 920 for 60Hz
    }
  }
  PORTD = PORTD | B01100000;  //disables the display so last line isnt on
}



void clearDisplay() {
  PORTD = B11100010;
  PORTB = B00000000;
  for (int X = 0; X < pixleLength; X++) {
    PORTB = PORTB | B00000010;  //clock high
    PORTB = PORTB & B11111101;  //clock low
  }
}



void clearBuffer() {
  for (int i = 0; i < pixleLength; i++) {
    displayBuffer[i] = 0x0000;  //invert colors
  }
}



void invertBuffer() {
  for (int i = 0; i < pixleLength; i++) {
    displayBuffer[i] = ~displayBuffer[i];  //invert colors
  }
}

